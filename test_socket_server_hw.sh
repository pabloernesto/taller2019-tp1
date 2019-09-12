#! /bin/bash
testname=test_socket_server_hw
source testing.sh

port=`random_port`
tempfile stdout
./aux_test_socket_server_hw $port >$stdout &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

echo hello world! | nc -N localhost $port
client=$!

wait $client $server

tempfile expected_output
echo 'hello world!' >$expected_output
delta=`diff -q $stdout $expected_output`
if [ "$delta" != "" ]; then
  fail "got $stdout"
fi

pass
