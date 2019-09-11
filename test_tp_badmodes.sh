#! /bin/bash
testname=test_tp_badmodes

stdout=`mktemp -q`
cleanup="rm $stdout"

exout=`mktemp -q`
echo -ne "Uso: ./tp server <puerto>\n" >$exout
cleanup+=" $exout"

./tp server >$stdout

delta=`diff $stdout $exout`
if [ "$delta" != "" ]; then
  echo "$testname failed:"
  echo -e "expected `cat $exout`"
  echo -e "but got `cat $stdout`"
  $cleanup
  exit
fi

echo "$testname passed"

# clean temporary files
$cleanup
