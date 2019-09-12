#! /bin/bash

if [ ! -v testname ]; then
  echo "$0: testname not initialized, exiting"
  exit
fi

# Usage: tempfile x
# Will assign a file under /tmp to variable x and append it to the cleanup routine
function tempfile() {
  declare -g $1=`mktemp -q`
  cleanup="${cleanup:-rm} ${!1}"
}

# Usage: pass
function pass() {
  echo "$testname passed"
  $cleanup
  exit
}

# Usage: fail [msg]
function fail() {
  echo "$testname failed:"
  if [ -n "$1" ]; then echo -e "\t$1"; fi
  $cleanup
  exit
}

# Usage port=`random_port`
function random_port() {
  echo "1000 + $RANDOM * 8000 / 32767" | bc
}
