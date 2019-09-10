#! /bin/bash
testname=test_tp_client_put

# selects a random port in the range 1000..9000
port=`echo "1000 + $RANDOM * 8000 / 32767" | bc`
network_output=`mktemp -q`
standard_output=`mktemp -q`
cleanup="rm $network_output $standard_output"

cat >board_display.txt << EOF
U===========U===========U===========U
U 3 | 4 | 5 U   |   | 8 U   | 1 |   U
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

# 00 00 02 D2 is hexa for 722 big-endian
echo "0000000: 0000 02D2" \
  | xxd -revert \
  | cat - board_display.txt \
  | nc -l $port >$network_output &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

./tp client localhost $port >$standard_output << EOF
put 4 in 1,2
exit
EOF
client=$!

wait $client $server


# test success condition
net=`cat $network_output | xxd`
exnet="P"
exnet+=`echo "0000000: 01 02 04" | xxd -revert`
exnet=`echo -n $exnet | xxd`
if [ "$net" != "$exnet" ]; then
  echo "$testname failed:"
  echo -e "\tnet expected '$exnet'"
  echo -e "\tgot '$net'"
  $cleanup
  exit
fi

delta=`diff -q board_display.txt $standard_output`
if [ "$delta" != "" ]; then
  echo "$testname failed: bad stdout"
  cat -A $standard_output
  cat -A board_display.txt
  $cleanup
  exit
fi

echo "$testname passed"
$cleanup
