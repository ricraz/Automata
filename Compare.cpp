#include <iostream>
#include <string>
#include <sstream>
#include "NFA.cpp"
#include "RegEx.cpp"

int main() {
	string input = "";
	string regex = "";
	NFA * comp1;
	NFA * comp2;
	string alphaInput = "";
	unordered_set<char> alphabet;
	int states;
	vector<tuple<State*, char, State*>> transitions;
	bool repeat = true;
	
	cout << "This program checks two finite automata or regular expressions for equality." << endl;
	cout << "Epsilon is represented by z, star by *, concatenation by . and union by +" << endl;
	cout << "What is the alphabet? Enter a list of letters and/or single digits:";
	getline(cin, alphaInput);
	for (int i = 0; i < alphaInput.length(); ++i) {
		if (alphaInput[i]!=',' && alphaInput[i]!=' ') {
			alphabet.insert(alphaInput[i]);
		}
	}
	cout << "Alphabet is: ";
	for (auto i : alphabet) {
		cout << i <<  " ";
	}
	cout << endl;
	while (repeat) {
	cout << "1st entry: Finite Automaton (F) or Regular Expression (R)?" << endl;
    getline(cin, input);
	while (input != "F" && input != "R") {
		cout << "Please enter a valid input. " << endl;
		cout << "1st entry: Finite Automaton (F) or Regular Expression (R)?" << endl;
	    getline(cin, input);
	}
	if (input == "R") {
		cout << "Enter regular expression now:" << endl;
		getline(cin, regex);
		SyntaxTree * tempRegex = createTree(regex);
		comp1 = convertToNFA(tempRegex);
		comp1->setAlphabet(alphabet);
	} else if (input == "F") {
		comp1 = new NFA(alphabet);
	}
	
	cout << "2nd entry: Finite Automaton (F) or Regular Expression (R)?" << endl;
    getline(cin, input);
	while (input != "F" && input != "R") {
		cout << "Please enter a valid input. " << endl;
		cout << "2nd entry: Finite Automaton (F) or Regular Expression (R)?" << endl;
	    getline(cin, input);
	}
	if (input == "R") {
		cout << "Enter regular expression now:" << endl;
		getline(cin, regex);
		SyntaxTree * tempRegex = createTree(regex);
		comp2 = convertToNFA(tempRegex);
		comp2->setAlphabet(alphabet);
		
	} else if (input == "F") {
		comp2 = new NFA(alphabet);
	}
	comp1->removeUnreachableStates();
	comp2->removeUnreachableStates();
	
	checkEquivalence(comp1, comp2);
	comp1->deleteStates();
	comp2->deleteStates();
	delete comp1;
	delete comp2;
	
	cout << "Compare another pair? Y/N" << endl;
	getline(cin, input);
	while (input != "Y" && input != "N") {
		cout << "Please enter Y or N" << endl;
		getline(cin, input);
	}
	if (input == "Y") {
		repeat = true;
	} else {
		repeat = false;
	}
	}	
}
