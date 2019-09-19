#! /bin/bash
testname=test_tp_client_put_badindex
source testing.sh

tempfile network_input
# 00 00 00 2E is hexa for 46 big-endian
echo "0000000: 0000 002E" | xxd -revert >$network_input
echo -ne "Error en los índices. Rango soportado: [1,9]\n" >>$network_input

port=`random_port`
tempfile network_output
nc -l $port <$network_input >$network_output &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

tempfile standard_output
tempfile standard_error
./tp client localhost $port >$standard_output 2>$standard_error << EOF
put 4 in 0,2
exit
EOF
client=$!

wait $client $server


tempfile expected_network
echo -n P >$expected_network
echo "0000000: 00 02 04" | xxd -reverse >>$expected_network

# test success condition
delta=`diff -q $network_output $expected_network`
if [ "$delta" != "" ]; then
  fail "net expected '$exnet'\n\tgot '$net'"
fi

tempfile empty_file
delta=`diff -q $standard_output $empty_file`
if [ "$delta" != "" ]; then
  fail "expected empty output, got \"`cat $standard_output`\""
fi

tempfile expected_stderr
echo -ne "Error en los índices. Rango soportado: [1,9]\n" >$expected_stderr
delta=`diff -q $standard_error $expected_stderr`
if [ "$delta" != "" ]; then
  fail "`got\nhd $standard_error`\n\texpected\n`hd $expected_stderr`"
fi

pass
