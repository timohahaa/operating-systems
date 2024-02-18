#!/bin/bash

PROG_TYPE='server'

echo "Compiling ${PROG_TYPE}"
g++ -c ${PROG_TYPE}.cpp

echo 'Linking...'
g++ -o ${PROG_TYPE} ${PROG_TYPE}.o -lpthread

echo "Starting ${PROG_TYPE} execution..."
echo '=================================================='
echo ''

./${PROG_TYPE}

echo ''
echo '=================================================='
echo "Program ${PROG_TYPE} finished!"

echo ''
echo 'Cleaning directory, deleting .o and executable...'
rm ./${PROG_TYPE}.o
rm ./${PROG_TYPE}

echo 'Done!'
