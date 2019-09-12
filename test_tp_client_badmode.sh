#! /bin/bash
testname=test_tp_client_badmode
source testing.sh

tempfile stdout
./tp client >$stdout

tempfile exout
echo -ne "Uso: ./tp client <host> <puerto>\n" >$exout
delta=`diff $stdout $exout`
if [ "$delta" != "" ]; then
  fail "expected \"`cat $exout`\" but got \"`cat $stdout`\""
fi

pass
