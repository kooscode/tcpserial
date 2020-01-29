#Standard make file
#kdupreez@hotmail.com

APP=tcpserial
CXX=g++
SRC_MAIN=src
OUT_DIR=bin
CPP_STD=c++14

#extra paths
INC_MAIN=lib

#include folders
INC=$(INC_MAIN)
INC_PARAMS=$(foreach d, $(INC), -I$d)

#additional sources files
S1=$(INC_MAIN)/error_base.cpp
S2=$(INC_MAIN)/thread_rx.cpp
S3=$(INC_MAIN)/thread_base.cpp
S4=$(INC_MAIN)/basicserial.cpp
SRC_EXT=$(S1) $(S2) $(S3) $(S4)

#lib paths
LP1=
LP2=
LIB_PATH=$(LP1) $(LP2)
LIB_PATH_PARAMS=$(foreach d, $(LIB_PATH), -L$d)

#link libs
L1=serialport
L2=pthread
L3=
L4=

LIBS=$(L1) $(L2) $(L3) $(L4)
LIBS_PARAMS=$(foreach d, $(LIBS), -l$d)

#compile
$(APP): $(SRC_MAIN)/$(APP).cpp 
	test -d bin || mkdir -p bin
	$(CXX) -std=$(CPP_STD) $(SRC_MAIN)/$(APP).cpp $(SRC_EXT) -o $(OUT_DIR)/$(APP) $(INC_PARAMS) $(LIB_PATH_PARAMS) $(LIBS_PARAMS) -g -O0

clean:
	rm bin/$(APP)
	
