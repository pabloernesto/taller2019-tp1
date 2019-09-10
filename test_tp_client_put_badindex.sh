#! /bin/bash
testname=test_tp_client_put_badindex

# selects a random port in the range 1000..9000
port=`echo "1000 + $RANDOM * 8000 / 32767" | bc`
network_output=`mktemp -q`
standard_output=`mktemp -q`
cleanup="rm $network_output $standard_output"

network_input=`mktemp -q`
cleanup+=" $network_input"
# 00 00 00 2E is hexa for 46 big-endian
echo "0000000: 0000 002E" | xxd -revert >$network_input
echo -ne "Error en los índices. Rango soportado: [1,9]\n" >>$network_input

nc -l $port <$network_input >$network_output &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

./tp client localhost $port >$standard_output << EOF
put 4 in 0,2
exit
EOF
client=$!

wait $client $server


expected_network=`mktemp -q`
echo -n P >$expected_network
echo "0000000: 00 02 04" | xxd -reverse >>$expected_network
cleanup+=" $expected_network"

# test success condition
delta=`diff -q $network_output $expected_network`
if [ "$delta" != "" ]; then
  echo "$testname failed:"
  echo -e "\tnet expected '$exnet'"
  echo -e "\tgot '$net'"
  $cleanup
  exit
fi

expected_stdout=`mktemp -q`
echo -ne "Error en los índices. Rango soportado: [1,9]\n" >$expected_stdout
cleanup+=" $expected_stdout"

delta=`diff -q $standard_output $expected_stdout`
if [ "$delta" != "" ]; then
  echo "$testname failed: bad stdout"
  hd $standard_output
  hd $expected_stdout
  $cleanup
  exit
fi

echo "$testname passed"
$cleanup
