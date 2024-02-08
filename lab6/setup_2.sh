#!/bin/bash

PROG_NUM='2'

echo "Compiling program ${PROG_NUM}"
g++ -c prog${PROG_NUM}.cpp

echo 'Linking...'
g++ -o prog${PROG_NUM} prog${PROG_NUM}.o -lpthread

chmod +x ./prog${PROG_NUM} 

echo "Starting program ${PROG_NUM} execution..."
echo '=================================================='
echo ''

./prog${PROG_NUM}

echo ''
echo '=================================================='
echo "Program ${PROG_NUM} finished!"

echo ''
echo 'Cleaning directory, deleting .o and executable...'
rm ./prog${PROG_NUM}.o
rm ./prog${PROG_NUM}

echo 'Done!'
