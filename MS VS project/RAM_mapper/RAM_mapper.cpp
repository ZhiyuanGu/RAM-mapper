#include "RAM_mapper.h"

// the whole process of mapping for Stratix IV-like architecture
void RAM_mapper::mapping() {
	// start point to record running time
	clock_t start_time = clock();

	// calling methods in RAM mapper
	parseFile();
	//printLogicRAMs();
	setArchitecture1();
	//printArchitecture();
	mapToAll();
	//printSolutions();
	//printLimitingFactor();
	balance();
	//cout << "Geometric Average: " << geometricAverage() << endl;
	writeFile();

	// end point to record running time
	clock_t end_time = clock();
	cout << "-------------------------------------------------------\n";
	cout << "Total CPU time is: " << static_cast<double>(end_time - start_time) / (CLOCKS_PER_SEC) << "s" << endl;
}

// the whole process of mapping for architecture with no LUTRAM and 1 type of BRAM
void RAM_mapper::mapping2() {
	int kb;
	vector<double> minGA(8, 1e20);
	vector<int> maxW_record(8, 0);
	vector<int> ratio_reocrd(8, 0);

	parseFile();
	for (int k = 0; k < 8; k++) {
		kb = (int)(pow(2, k));
		for (int maxW = 2; maxW < 512; maxW *= 2) {
			for (int ratio = 1; ratio < 40; ratio++) {
				setArchitecture2(kb, maxW, ratio);
				mapToAll();
				balance();
				if (geometricAverage() < minGA[k]) {
					minGA[k] = geometricAverage();
					maxW_record[k] = maxW;
					ratio_reocrd[k] = ratio;
				}
			}
		}
	}
	for (int k = 0; k < 8; k++) {
		kb = (int)(pow(2, k));
		cout << "Size = " << kb << ", maxW = " << maxW_record[k] << ", ratio = " << ratio_reocrd[k] << "   Smallest GA: " << minGA[k] << endl;
	}

}

// the whole process of mapping for architecture with 50% LUTRAM and 1 type of BRAM
void RAM_mapper::mapping3() {
	int kb;
	vector<double> minGA(8, 1e20);
	vector<int> maxW_record(8, 0);
	vector<int> ratio_reocrd(8, 0);
	int m[8] = { 4,4,8,8,16,16,32,32 };
	int r[8] = { 2,2,4,4,8,8,16,16 };
	parseFile();
	for (int k = 0; k < 8; k++) {
		kb = (int)(pow(2, k));
		for (int maxW = m[k]; maxW < m[k] * 8; maxW *= 2) {
			for (int ratio = r[k]; ratio < r[k] * 4; ratio++) {
				setArchitecture3(kb, maxW, ratio);
				mapToAll();
				balance();
				if (geometricAverage() < minGA[k]) {
					minGA[k] = geometricAverage();
					maxW_record[k] = maxW;
					ratio_reocrd[k] = ratio;
				}
			}
		}
	}
	for (int k = 0; k < 8; k++) {
		kb = (int)(pow(2, k));
		cout << "Size = " << kb << ", maxW = " << maxW_record[k] << ", ratio = " << ratio_reocrd[k] << "   Smallest GA: " << minGA[k] << endl;
	}

}

// the whole process of mapping for my own designed architecture
void RAM_mapper::mapping4() {
	int r1, r2, m1, m2;
	double r0;
	double GA = 1e20;

	int kb1 = 4, kb2 = 32;
	parseFile();
	int LoopCount = 0;
	for (double ratio0 = 6.6; ratio0 <= 6.6; ratio0 += 0.01) {
		for (int maxW1 = 16; maxW1 <= 16; maxW1 *= 2) {
			for (int ratio1 = 12; ratio1 <= 12; ratio1 += 1) {
				for (int maxW2 = 32; maxW2 <= 32; maxW2 *= 2) {
					for (int ratio2 = 36; ratio2 <= 36; ratio2 += 1) {
						cout << "Loop count: " << LoopCount++ << endl;
						setArchitecture4(kb1, maxW1, ratio1, kb2, maxW2, ratio2, ratio0);
						mapToAll();
						balance();
						writeFile();
						if (geometricAverage() < GA) {
							GA = geometricAverage();
							r0 = ratio0;
							r1 = ratio1;
							r2 = ratio2;
							m1 = maxW1;
							m2 = maxW2;
						}
					}
				}
			}
		}
	}

	cout << "LUTRAM ratio: " << r0 << endl
		<< " BRAM1: " << "size = " << kb1 << ", maxW = " << m1 << ", ratio = " << r1 << endl
		<< " BRAM2: " << "size = " << kb2 << ", maxW = " << m2 << ", ratio = " << r2 << endl
		<< " Smallest GA: " << GA << endl;
}

// the whole process of mapping for my own designed architecture with MTJ
void RAM_mapper::mapping5() {
	int r1, r2, m1, m2;
	double r0;
	double GA = 1e20;

	int kb1 = 4, kb2 = 64;
	parseFile();
	int LoopCount = 0;
	for (double ratio0 = 6.03; ratio0 <= 6.03; ratio0 += 0.01) {
		for (int maxW1 = 16; maxW1 <= 16; maxW1 *= 2) {
			for (int ratio1 = 10; ratio1 <= 10; ratio1 += 2) {
				for (int maxW2 = 32; maxW2 <= 32; maxW2 *= 2) {
					for (int ratio2 = 36; ratio2 <= 36; ratio2 += 1) {
						cout << "Loop count: " << LoopCount++ << endl;
						setArchitecture5(kb1, maxW1, ratio1, kb2, maxW2, ratio2, ratio0);
						mapToAll();
						balance();
						writeFile();
						if (geometricAverage() < GA) {
							GA = geometricAverage();
							r0 = ratio0;
							r1 = ratio1;
							r2 = ratio2;
							m1 = maxW1;
							m2 = maxW2;
						}
					}
				}
			}
		}
	}

	cout << "LUTRAM ratio: " << r0 << endl
		<< " BRAM1: " << "size = " << kb1 << ", maxW = " << m1 << ", ratio = " << r1 << endl
		<< " BRAM2: " << "size = " << kb2 << ", maxW = " << m2 << ", ratio = " << r2 << endl
		<< " Smallest GA: " << GA << endl;
}

// parse input files
void RAM_mapper::parseFile() {
	cout << "-------------------------------------------------------\n";
	cout << "Start parsing files...";
	ifstream infile_ram("logical_rams.txt");
	ifstream infile_logic_block("logic_block_count.txt");
	Circuits.clear();
	string line;
	// parse logic block count file and create circuits
	getline(infile_logic_block, line);// don't care the first line
	unsigned int n, lbc;
	while (infile_logic_block >> n >> lbc)
	{
		//cout << n << " " << lbc << endl;
		Circuit newCircuit(n, lbc);
		Circuits.push_back(newCircuit);
	}
	// parse logical ram file
	unsigned int c, ID, depth, width;
	string mode;
	getline(infile_ram, line);// don't care the first line
	while (getline(infile_ram, line))
	{
		istringstream iss(line);
		if (iss >> c >> ID >> mode >> depth >> width) {
			//cout << c << " " << ID << " " << mode << " " << depth << " " << width << endl;
			RamMode Mode;
			if (mode == "SimpleDualPort") {
				Mode = SimpleDualPort;
			}
			else if (mode == "SinglePort") {
				Mode = SinglePort;
			}
			else if (mode == "ROM") {
				Mode = ROM;
			}
			else if (mode == "TrueDualPort") {
				Mode = TrueDualPort;
			}
			else {
				cout << "Error: unknown ram mode!\n";
			}
			Circuits[c].addLogicalRAM(ID, Mode, depth, width);
		}
	}
	cout << "finished!\n";
}

// test: print all the logical RAMs
void RAM_mapper::printLogicRAMs() {
	for (auto& C : Circuits) {
		cout << "Circuit No." << C.getNumber() << " :\n";
		for (auto& LR : C.getLogicRAMs()) {
			LR.printLogicRAM();
		}
	}
}

// set stratixIV-like architecture
void RAM_mapper::setArchitecture1() {
	cout << "-------------------------------------------------------\n";
	cout << "Start setting architecture...";
	arch = Architecture(6, 10, 16, 35000.0);
	// add supported physical rams
	arch.addPhysicalRAM(0, LUTRAM, 10, 64, 2, 1);
	arch.addPhysicalRAM(1, BRAM8192b, 1, 8192, 32, 10);
	int d = 128 * 1024;
	arch.addPhysicalRAM(2, BRAM128kb, 1, d, 128, 300);
	cout << "finished!\n";
}

// set architecture with no LUTRAM and 1 type of BRAM
void RAM_mapper::setArchitecture2(int kb, int maxW, int ratio) {
	cout << "-------------------------------------------------------\n";
	cout << "Start setting architecture...";
	arch = Architecture(6, 10, 16, 35000.0);
	// add supported physical rams
	int d = kb * 1024;
	arch.addPhysicalRAM(0, varyingBRAM, 1, d, maxW, ratio);
	cout << "finished!\n";
}

// set architecture with 50% LUTRAM and 1 type of BRAM
void RAM_mapper::setArchitecture3(int kb, int maxW, int ratio) {
	cout << "-------------------------------------------------------\n";
	cout << "Start setting architecture...";
	arch = Architecture(6, 10, 16, 35000.0);
	// add supported physical rams
	arch.addPhysicalRAM(0, LUTRAM, 10, 64, 2, 1);
	int d = kb * 1024;
	arch.addPhysicalRAM(1, varyingBRAM, 1, d, maxW, ratio);
	cout << "finished!\n";
}

// set architecture with LUTRAM and 2 types of BRAMs
void RAM_mapper::setArchitecture4(int kb1, int maxW1, int ratio1, int kb2, int maxW2, int ratio2, double ratio0) {
	cout << "-------------------------------------------------------\n";
	cout << "Start setting architecture...";
	arch = Architecture(6, 10, 16, 35000.0);
	// add supported physical rams
	arch.addPhysicalRAM(0, LUTRAM, 10, 64, 2, ratio0);
	int d1 = kb1 * 1024;
	arch.addPhysicalRAM(1, varyingBRAM, 1, d1, maxW1, ratio1);
	int d2 = kb2 * 1024;
	arch.addPhysicalRAM(2, varyingBRAM, 1, d2, maxW2, ratio2);
	cout << "finished!\n";
}

// set architecture with LUTRAM and 2 types of BRAMs(MTJ)
void RAM_mapper::setArchitecture5(int kb1, int maxW1, int ratio1, int kb2, int maxW2, int ratio2, double ratio0) {
	cout << "-------------------------------------------------------\n";
	cout << "Start setting architecture...";
	arch = Architecture(6, 10, 16, 35000.0);
	// add supported physical rams
	arch.addPhysicalRAM(0, LUTRAM, 10, 64, 2, ratio0);
	int d1 = kb1 * 1024;
	arch.addPhysicalRAM(1, MTJ, 1, d1, maxW1, ratio1);
	int d2 = kb2 * 1024;
	arch.addPhysicalRAM(2, MTJ, 1, d2, maxW2, ratio2);
	cout << "finished!\n";
}

// test: print architecture
void RAM_mapper::printArchitecture() {
	arch.print();
}

// map all logical rams in all circuits to all possible physical rams
void RAM_mapper::mapToAll() {
	cout << "-------------------------------------------------------\n";
	cout << "Start mapping to all possible physical RAMs...";
	for (auto &C : Circuits) {
		C.mapToAll(arch);
	}

	cout << "finished!\n";
}

// test: print solutions
void RAM_mapper::printSolutions() {
	int c = 0;
	cout << "Print solutions for circuit " << c << endl;
	Circuits[c].printAllSolution();
}

// test: print limiting factor
void RAM_mapper::printLimitingFactor() {

	cout << "Print limiting factor " << endl;
	for (unsigned int i = 0; i < Circuits.size(); i++) {
		Circuit C = Circuits[i];
		cout << "Circuit No." << i << ", Total area: " << C.totalArea(arch) << endl;
	}
}

// balance use of physical rams
void RAM_mapper::balance() {
	cout << "-------------------------------------------------------\n";
	cout << "Start balancing use of physical rams...";
	for (auto &C : Circuits) {
		C.achieveBalance(arch);
	}

	cout << "finished!\n";
}

// calculate geometric average of total FPGA area
double RAM_mapper::geometricAverage() {
	double A = 1;
	for (auto &C : Circuits) {
		A *= C.getTotalArea() / (2e8);
	}
	A = pow(A, 1.0 / (double)(Circuits.size())) * 2e8;
	return A;
}

// write to output mapping file
void RAM_mapper::writeFile() {
	cout << "-------------------------------------------------------\n";
	cout << "Start writing to output mapping file...";
	ofstream outfile;
	outfile.open("mapping_result.txt", ios::out);
	for (auto &C : Circuits) {
		C.writeFile(outfile);
	}

	outfile.close();
	cout << "finished!\n";

}
