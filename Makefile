OS=_LINUX
#_DARWIN, WIN32, _LINUX

INC = 
CFLAGS = 
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEFAULT = $(INC) -Isrc
CFLAGS_DEFAULT = $(CFLAGS) -DASIO_STANDALONE -std=c++11 -Wextra -Wall -D$(OS)
RESINC_DEFAULT = $(RESINC)
RCFLAGS_DEFAULT = $(RCFLAGS)
LIBDIR_DEFAULT = $(LIBDIR)
LIB_DEFAULT = $(LIB)
LDFLAGS_DEFAULT = $(LDFLAGS) -lboost_filesystem -lboost_system -lpthread -ljsoncpp
OBJDIR_DEFAULT = .objs
DEP_DEFAULT = 
OUT_DEFAULT = WebServer

ifndef OS
	$(error OS is not set)
endif
	


OBJ_DEFAULT = $(OBJDIR_DEFAULT)/src/main.o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiTuner/KinseiTuner.o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDeviceUtility.o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDevice/KinseiDevice.o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/SimpleSocket.o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/PassiveSocket.o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/ActiveSocket.o

all:	default
	

clean: clean_default

before_default: 
	@echo Operative System set to $(OS)
	test -d $(OBJDIR_DEFAULT)/src || mkdir -p $(OBJDIR_DEFAULT)/src
	test -d $(OBJDIR_DEFAULT)/src/Kinsei/KinseiTuner || mkdir -p $(OBJDIR_DEFAULT)/src/Kinsei/KinseiTuner
	test -d $(OBJDIR_DEFAULT)/src/Kinsei || mkdir -p $(OBJDIR_DEFAULT)/src/Kinsei
	test -d $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDevice || mkdir -p $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDevice
	test -d $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket || mkdir -p $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket

after_default: 

default: before_default out_default after_default

out_default: before_default $(OBJ_DEFAULT) $(DEP_DEFAULT)
	$(CXX) $(LIBDIR_DEFAULT) -o $(OUT_DEFAULT) $(OBJ_DEFAULT)  $(LDFLAGS_DEFAULT) $(LIB_DEFAULT)

$(OBJDIR_DEFAULT)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/main.cpp -o $(OBJDIR_DEFAULT)/src/main.o

$(OBJDIR_DEFAULT)/src/Kinsei/KinseiTuner/KinseiTuner.o: src/Kinsei/KinseiTuner/KinseiTuner.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/Kinsei/KinseiTuner/KinseiTuner.cpp -o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiTuner/KinseiTuner.o

$(OBJDIR_DEFAULT)/src/Kinsei/KinseiDeviceUtility.o: src/Kinsei/KinseiDeviceUtility.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/Kinsei/KinseiDeviceUtility.cpp -o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDeviceUtility.o

$(OBJDIR_DEFAULT)/src/Kinsei/KinseiDevice/KinseiDevice.o: src/Kinsei/KinseiDevice/KinseiDevice.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/Kinsei/KinseiDevice/KinseiDevice.cpp -o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDevice/KinseiDevice.o

$(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/SimpleSocket.o: src/Kinsei/KinseiCommon/clsocket/SimpleSocket.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/Kinsei/KinseiCommon/clsocket/SimpleSocket.cpp -o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/SimpleSocket.o

$(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/PassiveSocket.o: src/Kinsei/KinseiCommon/clsocket/PassiveSocket.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/Kinsei/KinseiCommon/clsocket/PassiveSocket.cpp -o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/PassiveSocket.o

$(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/ActiveSocket.o: src/Kinsei/KinseiCommon/clsocket/ActiveSocket.cpp
	$(CXX) $(CFLAGS_DEFAULT) $(INC_DEFAULT) -c src/Kinsei/KinseiCommon/clsocket/ActiveSocket.cpp -o $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket/ActiveSocket.o

clean_default: 
	rm -f $(OBJ_DEFAULT) $(OUT_DEFAULT)
	rm -rf $(OBJDIR_DEFAULT)/src
	rm -rf $(OBJDIR_DEFAULT)/src/Kinsei/KinseiTuner
	rm -rf $(OBJDIR_DEFAULT)/src/Kinsei
	rm -rf $(OBJDIR_DEFAULT)/src/Kinsei/KinseiDevice
	rm -rf $(OBJDIR_DEFAULT)/src/Kinsei/KinseiCommon/clsocket

.PHONY: before_default after_default clean_default

