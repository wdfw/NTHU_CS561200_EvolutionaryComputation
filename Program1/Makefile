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
SRC := $(CODE_FILE)/libs/*.cpp $(CODE_FILE)/src/main.cpp
EXP_SRC := $(CODE_FILE)/libs/*.cpp $(CODE_FILE)/src/experienment.cpp
INCLUDE := $(CODE_FILE)/include/*.hpp
TARGET := ./$(CODE_FILE)/bin/main
EXP_TARGET := ./$(CODE_FILE)/bin/experienment
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC) $(INCLUDE) 
	$(CXX) -I$(CODE_FILE)/include $(CXXFLAGS) -o $@ $(SRC)

experienment: $(EXP_TARGET)

run_exp: experienment
	./$(EXP_TARGET)
	
$(EXP_TARGET): $(SRC) $(INCLUDE) $(EXP_SRC)
	$(CXX) -I$(CODE_FILE)/include $(CXXFLAGS) -o $@ $(EXP_SRC)

test: all
	./$(TARGET)
judge: $(TARGET) 
	cp $(TARGET) Judger
clean:
ifeq ($(detected_OS),Windows)
	del /f /q $(OBJ) $(TARGET).exe
else
	rm -f $(TARGET)
endif

.PHONY: all clean
