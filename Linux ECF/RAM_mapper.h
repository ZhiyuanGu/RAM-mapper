#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "Circuit.h"

using namespace std;

class RAM_mapper {
private:
	// all the circuit with logic rams to map
	vector<Circuit> Circuits;
	// the physical architecture used to map
	Architecture arch;


public:
	// the whole process of mapping for Stratix IV-like architecture
	void mapping();
	// the whole process of mapping for architecture with no LUTRAM and 1 type of BRAM
	void mapping2();
	// the whole process of mapping for architecture with 50% LUTRAM and 1 type of BRAM
	void mapping3();
	// the whole process of mapping for my own designed architecture
	void mapping4();
	// the whole process of mapping for my own designed architecture with MTJ
	void mapping5();

	// parse input files
	void parseFile();
	// test: print all the logical RAMs
	void printLogicRAMs();

	// set stratixIV-like architecture
	void setArchitecture1();
	// set architecture with no LUTRAM and 1 type of BRAM
	void setArchitecture2(int kb, int maxW, int ratio);
	// set architecture with 50% LUTRAM and 1 type of BRAM
	void setArchitecture3(int kb, int maxW, int ratio);
	// set architecture with LUTRAM and 2 types of BRAMs
	void setArchitecture4(int kb1, int maxW1, int ratio1, int kb2, int maxW2, int ratio2, double ratio0);
	// set architecture with LUTRAM and 2 types of BRAMs(MTJ)
	void setArchitecture5(int kb1, int maxW1, int ratio1, int kb2, int maxW2, int ratio2, double ratio0);
	// test: print architecture
	void printArchitecture();

	// map all logical rams in all circuits to all possible physical rams
	void mapToAll();
	// test: print solutions
	void printSolutions();
	// test: print limiting factor
	void printLimitingFactor();
	// balance use of physical rams
	void balance();

	// calculate geometric average of total FPGA area
	double geometricAverage();
	// write to output mapping file
	void writeFile();

};
