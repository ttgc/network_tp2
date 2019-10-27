# Network Programming Lab 2

To clone this repo use the command : 

`git clone --recursive https://github.com/VALERE91/network_tp2`

# Build on linux
After cloning repository, run the following commands in a terminal to build the application :
```
cd repo
git submodule init external/
git submodule update
mkdir output
cd output/
cmake ..
cmake --build .
```
