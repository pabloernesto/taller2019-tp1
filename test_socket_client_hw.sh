#! /bin/bash
testname=test_socket_client_hw
source testing.sh

port=`random_port`
tempfile stdout
nc -l $port >$stdout &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

./aux_test_socket_client_hw $port
client=$!

wait $client $server

tempfile expected_output
echo -n 'hello world!' >$expected_output
delta=`diff -q $stdout $expected_output`
if [ "$delta" != "" ]; then
  fail "expected $expected_output"
fi

pass
