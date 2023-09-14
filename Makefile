CXXFLAGS:=-g -Wall -I/usr/local/include/
LDFLAGS=-lm -larmadillo
OBJ=solver.o altproj.o game.o

all: $(OBJ)
	$(CXX) main.cpp -o sudoku $(OBJ) $(CXXFLAGS) $(LDFLAGS)

debug: $(OBJ)
	$(CXX) debug.cpp -o debug $(OBJ) $(CXXFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm $(OBJ) sudoku
