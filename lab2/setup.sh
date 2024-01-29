#!/bin/bash

echo -n 'Whitch program to run? Input 1, 2 or 3:'
read PROG_NUMBER
echo ''

if [[ ! $PROG_NUMBER =~ ^[0-9]+$ ]]
then
    echo 'You should enter a positive integer!'
    exit 1
elif [[ $PROG_NUMBER -ge 4 ]]
then
    echo 'You should enter a number less than 4!'
    exit 1
elif [[ $PROG_NUMBER -le 0 ]]
then
    echo 'You should enter a number bigger than 0!'
    exit 1
fi

echo "Compiling program number ${PROG_NUMBER}"
g++ -o lab.o -c lab_${PROG_NUMBER}.cpp

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
