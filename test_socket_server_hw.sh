#! /bin/bash

port=1234
tmp_file=`mktemp -q`

./aux_test_socket_server_hw $port >$tmp_file &
echo hello world! | nc -N localhost $port

ret=`cat $tmp_file`

if [ "$ret" = "hello world!" ]; then
  echo "ALL TESTS PASSED"
else
  echo "test failed: got $ret"
fi

rm $tmp_file