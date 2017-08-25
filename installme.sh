#!/usr/bin/env bash

sudo apt-get install dh-autoreconf
make -f Makefile.dist
./configure 
make
make install