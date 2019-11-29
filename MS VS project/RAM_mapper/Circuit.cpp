#include "Circuit.h"

void Circuit::addLogicalRAM(unsigned int id, RamMode mode, unsigned int depth, unsigned int width) {
	logicRAM newLR(id, mode, depth, width);
	logicRAMs.push_back(newLR);
}

// map all logical rams in the circuit to all possible physical rams
void Circuit::mapToAll(Architecture& arch) {
	// initialization
	TotalExtraLUT = 0;
	TotalArea = 0;
	physicalRamChosen.clear();
	physicalRamCount.clear();
	bestSolutions.clear();
	for (unsigned int i = 0; i < arch.getPhysicalRAMs().size(); i++) {// loop through all physical rams
		vector<pair<double, int> > temp;
		physicalRamChosen.push_back(temp);
		physicalRamCount.push_back(0);
	}
	// start mapping
	for (auto& lr : logicRAMs) {// loop through all logic rams
		lr.getSolutions().clear();
		// calculate and add solutions
		for (auto& pr : arch.getPhysicalRAMs()) {// loop through all physical rams
		// LUTRAM cannot do TrueDualPort
			if (lr.getMode() == TrueDualPort && pr.getType() == LUTRAM)
				continue;
			// check if depth is enough
			int logicRamD = lr.getDepth();
			if (logicRamD > arch.getSeriesLimit() * pr.getMaxD())
				continue;
			// find solution
			int extraLUT;// number of additional LUT needed
			int physicalRamID = lr.getID();// ID is a number you assigned to this group of physical RAM.
			int S, P;// how many combined in series/parallel
			physicalRamType type = pr.getType();// physical ram type
			int N = pr.getNumber();// physical ram type
			int W = pr.getMinW(), D = pr.getMaxD();// W and D are the width and depth configuration that the physical RAM is in
			if (logicRamD > D) {//whether or not need extra logic
				S = (logicRamD + D - 1) / D;
				P = (lr.getWidth() + W - 1) / W;
				int extraLUT_decoder = (S > 2) ? S : 1;
				int extraLUT_mux = (S + 1) / 3;
				extraLUT = logicRamD * (((lr.getMode() == ROM) ? 0 : extraLUT_decoder) + extraLUT_mux) * ((lr.getMode() == TrueDualPort) ? 2 : 1);
			}
			else {// logic ram depth <= max physical ram depth, no extra LUT required
				S = 1;
				extraLUT = 0;
				int F = pr.getFlexibility();
				if (lr.getMode() == TrueDualPort) F /= 2; // TrueDualPort not supported in largest width
				while (F > 1 && (logicRamD <= D / 2)) {// if the physical ram can be configured 2x wider and if the depth still meet requirement after configuration
					D = D / 2;
					W = W * 2;
					F = F / 2;
				}
				P = (lr.getWidth() + W - 1) / W;// calculate P after configuring physical RAM
			}
			double A = S * P * pr.getArea() + (double)(extraLUT) / (double)(arch.getN()) * arch.getLUTarea();// total ram area of this solution
			// test
			//cout << "SOLUTION CREATED!!\n";
			lr.addSolution(extraLUT, physicalRamID, S, P, N, type, W, D, A);// add solution to the logic ram

		}
		// sort according to area ascending order
		sort(lr.getSolutions().begin(), lr.getSolutions().end());
		// record solution with least area for each logic ram as initial best overall solution
		bestSolutions.push_back(lr.getSolutions()[0]);
		// record which logic ram used which physical ram 
		physicalRamChosen[lr.getSolutions()[0].getNumber()].push_back(pair<double, int>(1.0, lr.getID()));
		// record total number of each physical ram used
		physicalRamCount[lr.getSolutions()[0].getNumber()] += lr.getSolutions()[0].getP()*lr.getSolutions()[0].getS();
		// record total extra LUT used
		TotalExtraLUT += lr.getSolutions()[0].getExtraLUT();
	}
	/*
	for (int i = 0; i < 3; i++) {// loop through all physical rams
		cout << physicalRamChosen[i].size() << " ";
	}
	cout << endl;
	*/
}

// test: print all solutions in the required output format
void Circuit::printAllSolution() {
	for (auto& lr : logicRAMs) {// loop through all logic rams
		for (auto& S : lr.getSolutions()) {
			cout << number << " " << lr.getID() << " " << S.getExtraLUT() << " LW " << lr.getWidth() << " LD " << lr.getDepth() << " ID " << S.getID() << " S " << S.getS() << " P " << S.getP() << " Type " << S.getNumber() + 1 << " Mode ";
			if (lr.getMode() == SimpleDualPort) cout << "SimpleDualPort";
			else if (lr.getMode() == SinglePort) cout << "SinglePort";
			else if (lr.getMode() == ROM) cout << "ROM";
			else if (lr.getMode() == TrueDualPort) cout << "TrueDualPort";
			cout << " W " << S.getW() << " D " << S.getD()
				<< " Area: " << S.getArea()
				<< endl;
		}
	}

}

// print circuit extra LUT information
void Circuit::printXLUT() {
	cout << "total extra LUT: " << TotalExtraLUT << endl;
	for (auto& x : bestSolutions) {
		cout << x.getExtraLUT() << endl;
	}
}

// update best solution
void Circuit::updateBest(Architecture& arch) {
	logicRAM lastLRAM = logicRAMs[swappedID];
	// update solution with least area for each logic ram as initial best overall solution	
	bestSolutions[swappedID] = lastLRAM.getSolutions()[0];
	// update total number of each physical ram used
	physicalRamCount[lastLRAM.getSolutions()[0].getNumber()] += lastLRAM.getSolutions()[0].getP()*lastLRAM.getSolutions()[0].getS();
	physicalRamCount[lastLRAM.getSolutions()[swappedPR].getNumber()] -= lastLRAM.getSolutions()[swappedPR].getP()*lastLRAM.getSolutions()[swappedPR].getS();
	// update total extra LUT used
	TotalExtraLUT += lastLRAM.getSolutions()[0].getExtraLUT();
	TotalExtraLUT -= lastLRAM.getSolutions()[swappedPR].getExtraLUT();
	// update physicalRAMchosen information
	for (unsigned int i = 0; i < arch.getPhysicalRAMs().size(); i++) {// loop through all physical rams
		physicalRamChosen[i].clear();
	}
	for (unsigned int i = 0; i < logicRAMs.size(); i++) {// loop through all logic rams
		physicalRamChosen[logicRAMs[i].getSolutions()[0].getNumber()].push_back(pair<double, int>(1.0, logicRAMs[i].getID()));
	}
}

// find limiting factor and calculate total area
double Circuit::totalArea(Architecture& arch) {// compare the maximum logic blocks needed by each type of physical ram
	int mostLogicBlockCount = logicBlockCount + (TotalExtraLUT + arch.getN() - 1) / arch.getN();
	int leastLogicBlockCount = mostLogicBlockCount;
	int LUTRAMexist = (arch.getPhysicalRAMs()[0].getType() == LUTRAM);
	double LUTaverageArea = arch.getLUTarea();
	if (arch.getPhysicalRAMs()[0].getType() == LUTRAM) {// if LUTRAM exists in this architecture
		mostLogicBlockCount = max((int)(ceil((arch.getPhysicalRAMs()[0].getInterval() + 1.0) * (double)(physicalRamCount[0]))), (mostLogicBlockCount + physicalRamCount[0]));
		leastLogicBlockCount = mostLogicBlockCount;
		requiredMost = 0;
		requiredLeast = 0;
		LUTaverageArea = (1.0 / (arch.getPhysicalRAMs()[0].getInterval() + 1.0)) * arch.getPhysicalRAMs()[0].getArea() + (arch.getPhysicalRAMs()[0].getInterval() / (arch.getPhysicalRAMs()[0].getInterval() + 1.0)) * arch.getLUTarea();
	}
	for (unsigned int i = LUTRAMexist; i < arch.getPhysicalRAMs().size(); i++) {//loop through blockRAMs
		int newCount = physicalRamCount[i] * (int)(arch.getPhysicalRAMs()[i].getInterval());
		if (newCount > mostLogicBlockCount) {
			mostLogicBlockCount = newCount;
			requiredMost = i;
		}
		if (newCount < leastLogicBlockCount) {
			leastLogicBlockCount = newCount;
			requiredLeast = i;
		}
	}
	// calculate total area of the current best solution, first count LUT
	TotalArea = mostLogicBlockCount * LUTaverageArea;

	for (unsigned int i = LUTRAMexist; i < arch.getPhysicalRAMs().size(); i++) {
		TotalArea += mostLogicBlockCount / (int)(arch.getPhysicalRAMs()[i].getInterval()) * arch.getPhysicalRAMs()[i].getArea();
	}
	return TotalArea;
}

// change solutions one by one to balance physical RAM numbers, move logic rams using physical ram that is required most to the one that is required least
bool Circuit::balanceRAM(Architecture& arch) {
	//totalArea(arch);
	if (physicalRamChosen[requiredMost].size() == 0) return false;
	// find the solution to change
	unsigned int PR;// index of least required pram in the solution vector
	for (auto& n : physicalRamChosen[requiredMost]) {//n is the logic ram id recorded to have used the limiting resources		
		for (PR = 0; PR < logicRAMs[n.second].getSolutions().size(); PR++) {
			if (logicRAMs[n.second].getSolutions()[PR].getNumber() == requiredLeast)
				break;
		}
		if (PR < logicRAMs[n.second].getSolutions().size())//if found that solution
			n.first = logicRAMs[n.second].getSolutions()[PR].getArea() / logicRAMs[n.second].getSolutions()[0].getArea();
		else// if not found
			n.first = INT_MAX;
	}
	sort(physicalRamChosen[requiredMost].begin(), physicalRamChosen[requiredMost].end());// sort according to least area increase(%) after changing
	if (physicalRamChosen[requiredMost][0].first == INT_MAX) // if no solution that used least required PRAM can be found
		return false;
	int ramID = physicalRamChosen[requiredMost][0].second;// the id of logic ram to change solution
	for (PR = 0; PR < logicRAMs[ramID].getSolutions().size(); PR++) {// recalculate PR for the best choice to swap
		if (logicRAMs[ramID].getSolutions()[PR].getNumber() == requiredLeast)
			break;
	}
	if (PR >= logicRAMs[ramID].getSolutions().size())//new solution not found
		return false;
	// reocord swapping information
	swappedID = ramID;
	swappedPR = PR;
	// change solution by swap new chosen solution to the front
	swap(logicRAMs[ramID].getSolutions()[0], logicRAMs[ramID].getSolutions()[PR]);
	return true;
}

// reverse back the last move raising total area
void Circuit::reverseMove() {
	swap(logicRAMs[swappedID].getSolutions()[0], logicRAMs[swappedID].getSolutions()[swappedPR]);
}

// repeat balancing until no gain can be achieved
void Circuit::achieveBalance(Architecture& arch) {
	double oldArea, newArea;
	totalArea(arch);
	//cout << "Circuit No." << number << ", initial area: " << TotalArea;
	int round = 0;
	do {
		oldArea = TotalArea;
		bool succeedBalance = balanceRAM(arch);
		if (!succeedBalance) break;
		updateBest(arch);
		newArea = totalArea(arch);
		round++;
	} while (newArea < oldArea);
	// reverse to previous one because last move increased total area
	if (round > 0) {
		reverseMove();
		updateBest(arch);
	}
	newArea = totalArea(arch);
	//cout << ", final area: " << newArea << endl;

}

// write final result to output mapping file
void Circuit::writeFile(ofstream& outFile) {
	for (unsigned int i = 0; i < logicRAMs.size(); i++) {// loop through all logic rams
		logicRAM lr = logicRAMs[i];
		Solution S = bestSolutions[i];
		outFile << number << " " << lr.getID() << " " << S.getExtraLUT() << " LW " << lr.getWidth() << " LD " << lr.getDepth() << " ID " << S.getID()
			<< " S " << S.getS() << " P " << S.getP() << " Type " << S.getNumber() + 1 << " Mode ";
		if (lr.getMode() == SimpleDualPort) outFile << "SimpleDualPort";
		else if (lr.getMode() == SinglePort) outFile << "SinglePort";
		else if (lr.getMode() == ROM) outFile << "ROM";
		else if (lr.getMode() == TrueDualPort) outFile << "TrueDualPort";
		outFile << " W " << S.getW() << " D " << S.getD()
			<< endl;

	}
}



