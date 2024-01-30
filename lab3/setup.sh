#!/bin/bash

echo 'Whitch way to create a channel?'
echo '1) pipe(), blocking R/W'
echo '2) pipe2(), non-blocking R/W'
echo '3) pipe() + fcntl(), non-blocking R/W'
echo -n 'Input 1, 2 or 3: '
read ARG
echo ''

if [[ ! $ARG =~ ^[0-9]+$ ]]
then
    echo 'You should enter a positive integer!'
    exit 1
elif [[ $ARG -ge 4 ]]
then
    echo 'You should enter a number less than 4!'
    exit 1
elif [[ $ARG -le 0 ]]
then
    echo 'You should enter a number bigger than 0!'
    exit 1
fi

echo "Compiling program..."
g++ -c lab.cpp

echo 'Linking...'
g++ -o lab lab.o -lpthread

chmod +x ./lab 

echo 'Starting program execution...'
echo '=================================================='
echo ''

./lab "${ARG}"

echo ''
echo '=================================================='
echo 'Program finished!'

echo ''
echo 'Cleaning directory, deleting .o and executable...'
rm ./lab.o
rm ./lab

echo 'Done!'
