#! /bin/bash
testname=test_tp_server_badmode
source testing.sh

tempfile stdout
./tp server >$stdout

tempfile exout
echo -ne "Uso: ./tp server <puerto>\n" >$exout
delta=`diff $stdout $exout`
if [ "$delta" != "" ]; then
  fail "expected `cat $exout` but got `cat $stdout`"
fi

pass
