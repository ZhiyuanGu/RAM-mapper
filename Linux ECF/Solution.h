#include <iostream>
#include "Architecture.h"

using namespace std;

// solution of mapping logical ram to one of the physical rams
class Solution {
private:
	int extraLUT;// number of additional LUT needed
	int physicalRamID;// ID is a number you assigned to this group of physical RAM.
	int S, P;// how many combined in series/parallel
	int number;//physical ram number
	physicalRamType type;// physical ram type
	int W, D;// W and D are the width and depth configuration that the physical RAM is in
	double area;// total ram area of this solution
public:
	Solution(int xLUT, int ID, int s, int p, int n, physicalRamType t, int w, int d, double a);
	//natural order to put smaller area solution first
	bool operator<(const Solution& S) {
		return area < S.area;
	}

	int getExtraLUT() { return extraLUT; }
	int getID() { return physicalRamID; }
	int getS() { return S; }
	int getP() { return P; }
	int getNumber() { return number; }
	physicalRamType getType() { return type; }
	int getW() { return W; }
	int getD() { return D; }
	double getArea() { return area; }
};
