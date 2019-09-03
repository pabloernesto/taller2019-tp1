#! /bin/bash

port=1235
tmp_file=`mktemp -q`

nc -l $port >$tmp_file &
server=$!

sleep .1s

./aux_test_socket_client_hw $port
client=$!

wait $client $server

ret=`cat $tmp_file`

if [ "$ret" = "hello world!" ]; then
  echo "ALL TESTS PASSED"
else
  echo "test failed: got $ret"
fi

rm $tmp_file