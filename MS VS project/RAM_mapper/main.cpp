#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "RAM_mapper.h"

using namespace std;

int main() {
	cout << "-------------------------------------------------------\n";
	cout << "RAM mapper for ECE1756 by Zhiyuan Gu\n";

	int Q = 0;

	cout << "Please enter architecture No.(1,2,3,4,5) = \n";
	cin >> Q;
	while (Q > 5 || Q < 1) {
		cout << "Please enter architecture No.(1,2,3,4,5) = \n";
		cin >> Q;
	}

	// instantiate RAM mapper
	RAM_mapper RM;

	// mapping for question 1
	if (Q == 1)
		RM.mapping();

	// mapping for question 2
	if (Q == 2)
		RM.mapping2();

	// mapping for question 3
	if (Q == 3)
		RM.mapping3();

	// mapping for question 4
	if (Q == 4)
		RM.mapping4();

	// mapping for question 5
	if (Q == 5)
		RM.mapping5();


	if (Q == 1) {
		// call checker program
		cout << "-------------------------------------------------------\n";
		cout << "Calling checker program...\n";
		system("checker.exe -d -t logical_rams.txt logic_block_count.txt mapping_result.txt");
	}

	if (Q == 4) {
		// call checker program
		cout << "-------------------------------------------------------\n";
		cout << "Calling checker program...\n";
		int size1 = 4, size2 = 32;
		int maxW1 = 16, maxW2 = 32;
		int ratio0 = 660, ratio1 = 12, ratio2 = 36;
		string S100 = to_string(100);
		string S1 = to_string(1);
		string Sratio0 = to_string(ratio0);
		string Ssize1 = to_string(size1 * 1024);
		string SmaxW1 = to_string(maxW1);
		string Sratio1 = to_string(ratio1);
		string Ssize2 = to_string(size2 * 1024);
		string SmaxW2 = to_string(maxW2);
		string Sratio2 = to_string(ratio2);
		stringstream call_line;
		call_line << "checker.exe -t "
			<< " -l " << Sratio0 << " " << S100
			<< " -b " << Ssize1 << " " << SmaxW1 << " " << Sratio1 << " " << S1
			<< " -b " << Ssize2 << " " << SmaxW2 << " " << Sratio2 << " " << S1
			<< " logical_rams.txt logic_block_count.txt mapping_result.txt";
		cout << call_line.str().c_str();
		system(call_line.str().c_str());
	}

	if (Q == 5) {
		// call checker program
		cout << "-------------------------------------------------------\n";
		cout << "Calling checker program...\n";
		int size1 = 4, size2 = 64;
		int maxW1 = 16, maxW2 = 32;
		int ratio0 = 603, ratio1 = 10, ratio2 = 36;
		string S100 = to_string(100);
		string S1 = to_string(1);
		string Sratio0 = to_string(ratio0);
		string Ssize1 = to_string(size1 * 1024);
		string SmaxW1 = to_string(maxW1);
		string Sratio1 = to_string(ratio1);
		string Ssize2 = to_string(size2 * 1024);
		string SmaxW2 = to_string(maxW2);
		string Sratio2 = to_string(ratio2);
		stringstream call_line;
		call_line << "checker.exe "
			<< " -l " << Sratio0 << " " << S100
			<< " -b " << Ssize1 << " " << SmaxW1 << " " << Sratio1 << " " << S1
			<< " -b " << Ssize2 << " " << SmaxW2 << " " << Sratio2 << " " << S1
			<< " logical_rams.txt logic_block_count.txt mapping_result.txt";
		cout << call_line.str().c_str();
		system(call_line.str().c_str());
	}

	system("pause");
	return 0;
}