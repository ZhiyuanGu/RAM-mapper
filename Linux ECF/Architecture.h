#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// name of physical rams
enum physicalRamType {
	LUTRAM,
	BRAM8192b,
	BRAM128kb,
	varyingBRAM,
	MTJ
};

class physicalRAM {
private:
	int number;// physical ram number: 0,1,2...
	physicalRamType type;// physical ram type
	int W;// minimum width
	int D;// maximum depth when width is the minimum, or the capacitance of the physical ram when W = 1
	int x;// W*x = max width that a ram can provide when not using TrueDualPort mode
	double interval;//1 physical ram when have this number of logic blocks
	double area;// area for one of this physical ram

public:
	physicalRAM(int n, physicalRamType t, int w, int d, int x1, double i) :number(n), type(t), W(w), D(d), x(x1), interval(i) { area = (t == LUTRAM) ? 40000 : (9000 + ((t == MTJ) ? 1.25 : 5.0) * d + 90 * sqrt(d) + 600 * 2 * w * x1); }
	int getNumber() { return number; }
	physicalRamType getType() { return type; }
	int getMinW() { return W; }
	int getMaxD() { return D; }
	int getFlexibility() { return x; }
	double getInterval() { return interval; }
	double getArea() { return area; }
	// test: print physical ram information
	void print();
};


class Architecture {
private:
	int k;//input number of LUT
	int N;//number of LUTs in a logic block
	int SeriesLimit;//number of maximum allowed physical rams serialized
	double LUTarea;//area of a normal LUT
	vector<physicalRAM> physicalRAMs;// supported physical RAMs, put LUTRAM first if exist
public:
	Architecture(int k1 = 6, int n1 = 10, int sl = 16, double la = 35000) :k(k1), N(n1), SeriesLimit(sl), LUTarea(la) {}
	void addPhysicalRAM(int n, physicalRamType type, int minW, int maxD, int x, double i) { physicalRAMs.emplace_back(n, type, minW, maxD, x, i); }
	int getK() { return k; }
	int getN() { return N; }
	int getSeriesLimit() { return SeriesLimit; }
	double getLUTarea() { return LUTarea; }
	vector<physicalRAM>& getPhysicalRAMs() { return physicalRAMs; }
	// test: print architecture information
	void print();
};

