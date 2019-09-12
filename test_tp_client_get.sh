#! /bin/bash
testname=test_tp_client_get
source testing.sh

tempfile board_display
cat >$board_display << EOF
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

port=`random_port`
tempfile network_output
# 00 00 02 D2 is hexa for 722 big-endian
echo "0000000: 0000 02D2" \
  | xxd -revert \
  | cat - $board_display \
  | nc -l $port >$network_output &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

tempfile standard_output
./tp client localhost $port >$standard_output << EOF
get
exit
EOF
client=$!

wait $client $server


# test success condition
net=`cat $network_output`
if [ "$net" != "G" ]; then
  fail "expected G on network, got '$net'"
fi

delta=`diff $board_display $standard_output`
if [ "$delta" != "" ]; then
  fail "expected stdout was:"
fi

pass
