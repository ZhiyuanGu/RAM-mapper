#include <iostream>
#include <vector>
#include "Solution.h"

using namespace std;

enum RamMode {
	SimpleDualPort,
	SinglePort,
	ROM,
	TrueDualPort
};


class logicRAM {
private:
	unsigned int RamID;
	RamMode mode;
	int depth;
	int width;

	// store all possible solutions
	vector<Solution> Solutions;

public:
	logicRAM(unsigned int id, RamMode m, unsigned int d, unsigned int w) :RamID(id), mode(m), depth(d), width(w) {}
	unsigned int getID() { return RamID; }
	RamMode getMode() { return mode; }
	int getDepth() { return depth; }
	int getWidth() { return width; }
	vector<Solution>& getSolutions() { return Solutions; }
	// test: print logic RAM information
	void printLogicRAM();
	// add one solution 
	void addSolution(int xLUT, int ID, int s, int p, int n, physicalRamType t, int w, int d, double a) { Solutions.emplace_back(xLUT, ID, s, p, n, t, w, d, a); }
};
