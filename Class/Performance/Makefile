EXE = performance

CXX = c++
CXXFLAGS = -Wall -Wextra -g -std=c++11 -O3

SRC = Performance.cpp
OBJ = $(SRC:.o=.cpp)

all: $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


$(EXE): Performance.o
	$(CXX) $^ -o $(EXE)

Performance.o: BST_performance.h

clean:
	rm -rf Performance.o $(EXE) */*~ *~

.PHONY: clean all format
