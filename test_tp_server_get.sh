#! /bin/bash
testname=test_tp_server_get

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

cat >board_display.txt << EOF
U===========U===========U===========U
U 3 |   | 5 U   |   | 8 U   | 1 |   U
U---+---+---U---+---+---U---+---+---U
U   |   | 7 U   |   |   U 5 |   | 8 U
U---+---+---U---+---+---U---+---+---U
U 1 | 2 |   U 7 | 5 |   U   | 9 |   U
U===========U===========U===========U
U   |   | 9 U   | 7 |   U   |   | 4 U
U---+---+---U---+---+---U---+---+---U
U   |   | 4 U 3 |   | 5 U 9 |   |   U
U---+---+---U---+---+---U---+---+---U
U 7 |   |   U   | 9 |   U 8 |   |   U
U===========U===========U===========U
U   | 3 |   U   | 4 | 6 U   | 5 | 7 U
U---+---+---U---+---+---U---+---+---U
U 4 |   | 6 U   |   |   U 1 |   |   U
U---+---+---U---+---+---U---+---+---U
U   | 7 |   U 5 |   |   U 6 |   | 9 U
U===========U===========U===========U
EOF
cleanup+=" board_display.txt"

echo "0000000: 0000 02D2" \
  | xxd -revert \
  | cat - board_display.txt >expected_network.txt
cleanup+=" expected_network.txt"

./tp server $port &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

echo "G" | nc localhost $port >$network_output
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
