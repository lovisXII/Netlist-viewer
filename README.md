# Introduction

This project has been developped by Louis Geoffroy Pitailler during the first semester in master SESI for the class MOBJ.\
The aim was to develop a Netlist viewver based on c++ and using Qt4.

# Required package on linux :

To run properly this project you will need :
* c++ 14 compiler
* Qt4
* cmake
* make
Here's a little script to setup your installation properly :
```sh
echo "Please be in sudo"
echo "This script will setup the environment needed to compile and execute this project"
apt install gcc
apt install cmake
apt install make
echo "Installing libxml2 package"

apt-get install libxml2
apt-get install libxml2-dev

echo "Installing Qt4 package :"

add-apt-repository ppa:rock-core/qt4
apt install qt4-default
apt install qt4-qmake
```

# How to compile it ?

To compile the project you will find a ``CMakeLists.txt`` file in src/.\
You need to create a build directory at the root where you download the file.\
Then you just have to do :
```sh
cd build/
cmake ../src
./tme810
```