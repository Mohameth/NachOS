#!/bin/bash


## TODO
rm DISK
./nachos-filesys -f
./nachos-filesys -D
./nachos-filesys -cp test_fileWrite test_fileWrite
./nachos-filesys -x test_fileWrite