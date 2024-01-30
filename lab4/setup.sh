#!/bin/bash

echo -n 'Input some cli args for the parent to pass to the child (~5): '
read -a args

echo "Compiling programs..."
g++ -c parent.cpp
g++ -c child.cpp

echo 'Linking...'
g++ -o parent parent.o
g++ -o child child.o

chmod +x ./parent 
chmod +x ./child 

export PATH="${PATH}:$(pwd)"

echo 'Starting program execution...'
echo '=================================================='
echo ''

./parent "${args[@]}"

echo ''
echo '=================================================='
echo 'Program finished!'

echo ''
echo 'Cleaning directory, deleting .o and executable...'
rm ./parent.o
rm ./parent
rm ./child.o
rm ./child

echo 'Done!'
