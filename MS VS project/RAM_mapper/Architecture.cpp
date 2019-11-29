#include "Architecture.h"

// test: print physical ram information
void physicalRAM::print() {
	cout << "RAM type: ";
	if (type == LUTRAM) cout << "LUTRAM";
	else if (type == BRAM8192b) cout << "BRAM8192b";
	else if (type == BRAM128kb) cout << "BRAM128kb";
	cout << ", min Width: " << W
		<< ", max Depth: " << D
		<< ", flexibility:" << x
		<< ", interval:" << interval
		<< ", area:" << area
		<< endl;
}

// test: print architecture information
void Architecture::print() {
	cout << "Print out architecture...\n";
	cout << "LUT size: " << k << endl;
	cout << "Number of LUTs in a logic block: " << N << endl;
	cout << "Number of maximum allowed serialized physical ram:" << SeriesLimit << endl;
	cout << "Supported physical RAMs:\n";
	for (auto& pr : physicalRAMs) {
		pr.print();
	}
}
