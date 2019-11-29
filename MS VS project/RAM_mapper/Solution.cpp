#include "Solution.h"


Solution::Solution(int xLUT, int ID, int s, int p, int n, physicalRamType t, int w, int d, double a) {
	extraLUT = xLUT;// number of additional LUT needed
	physicalRamID = ID;// ID is a number you assigned to this group of physical RAM.
	S = s, P = p;// how many combined in series/parallel	
	number = n;
	type = t;// physical ram type
	W = w; D = d;// W and D are the width and depth configuration that the physical RAM is in
	area = a;// total ram area of this solution
}

