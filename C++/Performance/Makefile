EXE = performance

CXX = c++
CXXFLAGS = -Wall -Wextra -g -std=c++11 -O3

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): Performance.o
	$(CXX) $^ -o $(EXE)

clean:
	rm -rf Performance.o $(EXE) *~

.PHONY: clean
