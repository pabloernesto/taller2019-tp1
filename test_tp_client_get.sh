#! /bin/bash
testname=test_tp_client_get

# selects a random port in the range 1000..9000
port=`echo "1000 + $RANDOM * 8000 / 32767" | bc`
network_output=`mktemp -q`
standard_output=`mktemp -q`

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

# TODO: 1234 is not the right prefix! send the length of the input
echo -n 1234 | cat - board_display.txt | nc -l $port >$network_output &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

./tp client localhost $port >$standard_output << EOF
get
exit
EOF
client=$!

wait $client $server


# test success condition
net=`cat $network_output`
if [ "$net" != "G" ]; then
  echo "$testname failed: net expected G, got '$ret'"
  rm board_display.txt $network_output $standard_output
  exit
fi

delta=`diff board_display.txt $standard_output`
if [ "$delta" == "" ]; then
  echo "$testname failed: net expected stdout was: \n"
  cat $standard_output
  rm board_display.txt $network_output $standard_output
  exit
fi

echo "$testname passed"

# clean temporary files
rm board_display.txt $network_output $standard_output
