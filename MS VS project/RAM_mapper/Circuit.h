#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <climits>
#include "logicRAM.h"

using namespace std;

class Circuit {
private:
	unsigned int number;
	int logicBlockCount;
	// record the most and least popular physical rams for balancing
	int requiredMost;
	int requiredLeast;
	// record the solution swapped 
	int swappedID;// previous logic ram id chosen to change solution
	int swappedPR;// position of previous chosen solution
	vector<logicRAM> logicRAMs;
	vector<Solution> bestSolutions;
	// logic rams are grouped by the physical ram they choose to form best solution and represented by their index in logicRAMs
	vector<int> physicalRamCount;
	vector<vector<pair<double, int> > > physicalRamChosen;// first element in pair represent the influence when change the solution from best area one to second best one
	int TotalExtraLUT;
	double TotalArea;


public:
	Circuit(unsigned int n, int lbc) :number(n), logicBlockCount(lbc) { }

	void addLogicalRAM(unsigned int id, RamMode mode, unsigned int depth, unsigned int width);
	unsigned int getNumber() { return number; }
	double getTotalArea() { return TotalArea; }
	vector<logicRAM>& getLogicRAMs() { return logicRAMs; }

	// map all logical rams in the circuit to all possible physical rams
	void mapToAll(Architecture& arch);
	// test: print all solutions
	void printAllSolution();
	// find limiting factor and calculate total area
	double totalArea(Architecture& arch);
	// print circuit extra LUT information
	void printXLUT();
	// change one solution to balance physical RAM numbers, move logic rams using physical ram that is required most to the one that is required least
	bool balanceRAM(Architecture& arch);
	// update best solution
	void updateBest(Architecture& arch);
	// reverse back the last move raising total area
	void reverseMove();
	// repeat balancing until no gain can be achieved
	void achieveBalance(Architecture& arch);
	// write final result to output mapping file
	void writeFile(ofstream& outFile);
};
