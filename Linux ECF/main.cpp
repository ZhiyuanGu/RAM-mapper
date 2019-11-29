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

	return 0;
}