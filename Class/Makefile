EXE = test

CXX = c++
CXXFLAGS = -Wall -Wextra -g -std=c++11

SRC = Test.cpp
OBJ = $(SRC:.o=.cpp)

all: $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


$(EXE): Test.o
	$(CXX) $^ -o $(EXE)

Test.o: BST.h

clean:
	rm -rf Test.o $(EXE) */*~ *~

.PHONY: clean all format
