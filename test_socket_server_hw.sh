#! /bin/bash

tmp_file=`mktemp -q`

./aux_test_socket_server_hw 1234 >$tmp_file &
echo hello world! | nc -N localhost 1234

ret=`cat $tmp_file`

if [ "$ret" = "hello world!" ]; then
  echo "ALL TESTS PASSED"
else
  echo "test failed: got $ret"; fi

rm $tmp_file