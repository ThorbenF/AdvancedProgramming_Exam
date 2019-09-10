EXE = test

CXX = c++
CXXFLAGS = -Wall -Wextra -g -std=c++11

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): Test.o
	$(CXX) $^ -o $(EXE)

clean:
	rm -rf Test.o $(EXE) *~

.PHONY: clean
