#! /bin/bash
testname=test_tp_server_put_badindex
source testing.sh

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
cleanup="rm board.txt"

tempfile network_input
echo -n "P" >$network_input
echo "0000000: 0D 01 04" | xxd -revert >>$network_input

port=`random_port`
./tp server $port &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

tempfile network_output
cat $network_input | nc -N localhost $port >$network_output
client=$!

wait $client $server

tempfile expected_network
# 2E is hexa for 46 decimal1
echo "0000000: 0000 002E" | xxd -revert >$expected_network
echo -ne "Error en los índices. Rango soportado: [1,9]\n" >>$expected_network

# test network output
delta=`diff -q $network_output $expected_network`
if [ "$delta" != "" ]; then
  fail "expected `xxd -p $expected_network`\n\tbut got `xxd -p $network_output`"
fi

pass
