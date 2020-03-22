CC=g++
OP=-g3
ARGS=-lX11 -lstdc++fs -lpython2.7 -std=c++1z
FILE=

test:$(FILE).cpp ../src/recognition/color.cpp ../src/recognition/state_main.cpp ../src/recognition/image_processing.cpp ../src/tools/debug/debug.cpp
	$(CC) -o $(FILE) $(FILE).cpp ../src/recognition/color.cpp ../src/recognition/state_main.cpp ../src/recognition/image_processing.cpp ../src/tools/debug/debug.cpp $(ARGS) `pkg-config --cflags opencv` `pkg-config --libs opencv`  $(OP)
