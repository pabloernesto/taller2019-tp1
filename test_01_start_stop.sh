#! /bin/bash
testname=test_01_start_stop
source testing.sh

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
cleanup="rm board.txt"

port=`random_port`
tempfile server_stderr
./tp server $port 2>$server_stderr &
server=$!

# this sleep prevents the client from attempting connection
# until the server is up
sleep .1s

tempfile client_stdin
tempfile client_stdout
tempfile client_stderr
./tp client localhost $port <$client_stdin >$client_stdout 2>$client_stderr
client=$!

wait $client $server

tempfile empty_file
delta=`diff $client_stdout $empty_file`
if [ "$delta" != "" ]; then
  fail "expected \"\", got \"`cat $client_stdout`\""
fi

delta=`diff $client_stderr $empty_file`
if [ "$delta" != "" ]; then
  fail "client stderr: \"`cat $client_stderr`\""
fi

delta=`diff $server_stderr $empty_file`
if [ "$delta" != "" ]; then
  fail "server stderr: \"`cat $server_stderr`\""
fi

pass
