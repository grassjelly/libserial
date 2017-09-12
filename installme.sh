#!/usr/bin/env bash
sudo apt-get install libboost-all-dev
sudo apt-get install dh-autoreconf
make -f Makefile.dist
./configure 
make
make install
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc
