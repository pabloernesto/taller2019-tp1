#! /bin/bash

# selects a random port in the range 1000..9000
port=`echo "1000 + $RANDOM * 8000 / 32767" | bc`
tmp_file=`mktemp -q`

./aux_test_socket_server_hw $port >$tmp_file &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

echo hello world! | nc -N localhost $port
client=$!

wait $client $server

ret=`cat $tmp_file`

if [ "$ret" = "hello world!" ]; then
  echo "test_socket_server_hw passed"
else
  echo "test_socket_server_hw failed: got $ret"
fi

rm $tmp_file