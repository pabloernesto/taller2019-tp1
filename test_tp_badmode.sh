#! /bin/bash
testname=test_tp_client_badmode
source testing.sh

tempfile stdout
./tp localhost 1234 >$stdout

tempfile exout
echo -ne "Modo no soportado, el primer parámetro debe ser server o client\n" >$exout
delta=`diff $stdout $exout`
if [ "$delta" != "" ]; then
  fail "expected \"`cat $exout`\" but got \"`cat $stdout`\""
fi

pass
