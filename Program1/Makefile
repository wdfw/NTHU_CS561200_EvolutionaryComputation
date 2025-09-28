ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

CXX := g++
ifeq ($(detected_OS),Windows)
    CXX := g++
    CXXFLAGS := --std=c++17 -O3 -Wall
else
#     CXXFLAGS := --std=c++17 -O3 -Wall
    CXXFLAGS := --std=c++17 -O3 
endif

CODE_FILE := GAProgram
SRC := $(CODE_FILE)/libs/*.cpp $(CODE_FILE)/src/*.cpp
INCLUDE := $(CODE_FILE)/include/*.h
TARGET := ./$(CODE_FILE)/bin/main
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC) $(INCLUDE) 
	$(CXX) -I$(CODE_FILE)/include $(CXXFLAGS) -o $@ $(SRC)

test: all
	./$(TARGET)

clean:
ifeq ($(detected_OS),Windows)
	del /f /q $(OBJ) $(TARGET).exe
else
	rm -f $(TARGET)
endif

.PHONY: all clean
