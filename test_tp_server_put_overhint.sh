#! /bin/bash
testname=test_tp_server_put_overhint

# selects a random port in the range 1000..9000
port=`echo "1000 + $RANDOM * 8000 / 32767" | bc`
network_output=`mktemp -q`
cleanup="rm $network_output"

cat >board.txt << EOF
3 0 5 0 0 8 0 1 0
0 0 7 0 0 0 5 0 8
1 2 0 7 5 0 0 9 0
0 0 9 0 7 0 0 0 4
0 0 4 3 0 5 9 0 0
7 0 0 0 9 0 8 0 0
0 3 0 0 4 6 0 5 7
4 0 6 0 0 0 1 0 0
0 7 0 5 0 0 6 0 9
EOF
cleanup+=" board.txt"

# 24 is hexa for 36 decimal
echo "0000000: 0000 0024" | xxd -revert >expected_network.txt
echo -ne "La celda indicada no es modificable\n" >>expected_network.txt
cleanup+=" expected_network.txt"


# place a 4 over 1,1 which already contains a 3
network_input=`mktemp -q`
echo -n "P" >$network_input
echo "0000000: 01 01 04" | xxd -revert >>$network_input
cleanup+=" $network_input"


./tp server $port &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

cat $network_input | nc -N localhost $port >$network_output
client=$!

wait $client $server

# test network output
delta=`diff $network_output expected_network.txt`
if [ "$delta" != "" ]; then
  echo "$testname failed: unexpected network output"
  $cleanup
  exit
fi

echo "$testname passed"

# clean temporary files
$cleanup
