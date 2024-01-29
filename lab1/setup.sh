#!/bin/bash

echo 'Compiling...'
g++ -c lab.cpp

echo 'Linking...'
g++ -o lab lab.o -lpthread

chmod +x ./lab

echo 'Starting program execution...'
echo '=================================================='
echo ''

./lab

echo ''
echo '=================================================='
echo 'Program finished!'

echo ''
echo 'Cleaning directory, deleting .o and executable...'
rm ./lab.o
rm ./lab

echo 'Done!'
