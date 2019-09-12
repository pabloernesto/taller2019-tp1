#! /bin/bash
testname=tp_badmodes
source testing.sh

tempfile stdout

tempfile exout
echo -ne "Uso: ./tp server <puerto>\n" >$exout

./tp server >$stdout

delta=`diff $stdout $exout`
if [ "$delta" != "" ]; then
  fail "expected `cat $exout` but got `cat $stdout`"
fi

pass
