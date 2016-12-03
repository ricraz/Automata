#include <string>
#include <iostream>

using namespace std;

class SyntaxTree {
	char oper;
	SyntaxTree * left = NULL;
	SyntaxTree * right = NULL;
	SyntaxTree * parent = NULL;
	public:
		SyntaxTree(char leaf) {
			oper = leaf;
		}
		SyntaxTree(char op, SyntaxTree * lPointer) {
			oper = op;
			left = lPointer;
		}
		SyntaxTree(char op, SyntaxTree * lPointer, SyntaxTree * rPointer) {
			oper = op;
			left = lPointer;
			right = rPointer;
		}
		SyntaxTree * getLeft() {
			return left;
		}
		SyntaxTree * getRight() {
			return right;
		}
		char getOp() {
			return oper;
		}
};

void printSyntaxTree(SyntaxTree * A) {
	if (!A) {
		return;
	}
    if ((A->getLeft() || A->getRight()) && A->getOp() != '*') { //can we get rid of A->getRight() here? and below
        cout << "(";
    }
	printSyntaxTree(A->getLeft());
	cout << A->getOp();
	printSyntaxTree(A->getRight());
    if ((A->getLeft() || A->getRight()) && A->getOp() != '*') {
        cout << ")";
    }
}

SyntaxTree * makeSyntaxTree (string A) {
	int bracketCount = 0;
	int index = 0;
	SyntaxTree * newTree = NULL;
	SyntaxTree * newTree2 = NULL;
	
	if (A.length() == 1) {
		newTree = new SyntaxTree(A[0]);
		return newTree;
	} else if (A.length() == 0){
		return nullptr;		//does this work?
	} else {
		if (A[0]=='(') {
			bracketCount ++;
			index ++;
			while (bracketCount > 0) { //fix this to be safe for malformed regular expression
				if (A[index]==')') {
					bracketCount--;
				} else if (A[index]=='(') {
					bracketCount++;
				}
				index++;
			}
			if (index < A.length()) {
				string firstPart = A.substr(1, index-2);
				if (A[index]=='*') {
					if (index + 1 < A.length()) {
						if (A[index+1] == '+') {
							newTree2 = new SyntaxTree('*', makeSyntaxTree(firstPart));
							newTree = new SyntaxTree('+', newTree2, makeSyntaxTree(A.substr(index+2, -1)));
						} else {
						    newTree2 = new SyntaxTree('*', makeSyntaxTree(firstPart));
						    newTree = new SyntaxTree('.', newTree2, makeSyntaxTree(A.substr(index+1, -1)));
						}
					} else {
						newTree = new SyntaxTree('*', makeSyntaxTree(firstPart));
					}
				} else if (A[index]=='+') {
					newTree = new SyntaxTree('+', makeSyntaxTree(firstPart), makeSyntaxTree(A.substr(index+1, -1)));
				} else {
					newTree = new SyntaxTree('.', makeSyntaxTree(firstPart), makeSyntaxTree(A.substr(index, -1)));
				}
				return newTree;
			} else {
				return makeSyntaxTree(A.substr(1, A.length()-2));
			}
		}
		else {
			while (index < A.length() - 1) { //this works because last symbol can't be ( or +
				if (A[index] == '(') {
					newTree = new SyntaxTree('.', makeSyntaxTree(A.substr(0, index)), makeSyntaxTree(A.substr(index, -1)));
					return newTree;
				} else if (A[index+1] == '+') {
					newTree = new SyntaxTree('+', makeSyntaxTree(A.substr(0, index+1)), makeSyntaxTree(A.substr(index + 2, -1)));
					return newTree;
				}
				index ++;
			}
			if (A[1] == '*') {
				newTree = new SyntaxTree('*', makeSyntaxTree(A.substr(0,1)), makeSyntaxTree(A.substr(2, -1)));
			} else {
				newTree = new SyntaxTree('.', makeSyntaxTree(A.substr(0,1)), makeSyntaxTree(A.substr(1, -1)));
			}
			return newTree;
		}
	}
}
/*
NFA * convertToNFA(SyntaxTree * expression) {
	if (expression == NULL) {			//what about epsilons?
		return NULL;
	} else if (expression->getLeft == NULL) {
		NFA * singleton = new NFA(expression->getOp());
		return singleton;
	} else {
		return(convertToNFA(expression->getLeft())->combineNFAs(expression->getOp(), convertToNFA(expression->getRight())));
	}
}*/

string preprocess (string A) {  //removes spaces, closed bracket pairs and double stars
	string newString = "";
	int index = 0;
	while (index < A.length()) {
		if (A[index] != ' ') {
			newString += A[index];
		}
		index++;
	}
	index = 1;
	while (index < newString.length()) {
		if (newString[index] == ')' && newString[index-1] == '(') {
			newString.erase(newString.begin()+index);
			newString.erase(newString.begin()+index-1);
			index--;
		} else if (newString[index] == '*' && newString[index-1] == '*') {
			newString.erase(newString.begin()+index);
		} else {
			index++;
		}
	}
	cout << newString << endl;
	return newString;
}

SyntaxTree * createTree (string A) {
	return (makeSyntaxTree(preprocess(A)));
}

int main() {
    SyntaxTree * hi1 = createTree("(he+llo)*+(hi)");
    printSyntaxTree(hi1);
	cout << endl;
}