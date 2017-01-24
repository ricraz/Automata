#include <string>
#include <iostream>

using namespace std;

class SyntaxTree {
	char oper;
	SyntaxTree * left = nullptr;
	SyntaxTree * right = nullptr;
	SyntaxTree * parent = nullptr;
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
		~SyntaxTree(){
			delete left;
			delete right;
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
	int ind = 0;
	SyntaxTree * newTree = nullptr;
	SyntaxTree * newTree2 = nullptr;
	
	if (A.length() == 1) {
		newTree = new SyntaxTree(A[0]);
		return newTree;
	} else if (A.length() == 0){
		return nullptr;
	} else {
		if (A[0]=='(') {
			bracketCount ++;
			ind ++;
			while (bracketCount > 0) { //fix this to be safe for malformed regular expression
				if (A[ind]==')') {
					bracketCount--;
				} else if (A[ind]=='(') {
					bracketCount++;
				}
				ind++;
			}
			if (ind < A.length()) {
				string firstPart = A.substr(1, ind-2);
				if (A[ind]=='*') {
					if (ind + 1 < A.length()) {
						if (A[ind+1] == '+') {
							newTree2 = new SyntaxTree('*', makeSyntaxTree(firstPart));
							newTree = new SyntaxTree('+', newTree2, makeSyntaxTree(A.substr(ind+2, -1)));
						} else {
						    newTree2 = new SyntaxTree('*', makeSyntaxTree(firstPart));
						    newTree = new SyntaxTree('.', newTree2, makeSyntaxTree(A.substr(ind+1, -1)));
						}
					} else {
						newTree = new SyntaxTree('*', makeSyntaxTree(firstPart));
					}
				} else if (A[ind]=='+') {
					newTree = new SyntaxTree('+', makeSyntaxTree(firstPart), makeSyntaxTree(A.substr(ind+1, -1)));
				} else {
					newTree = new SyntaxTree('.', makeSyntaxTree(firstPart), makeSyntaxTree(A.substr(ind, -1)));
				}
				return newTree;
			} else {
				return makeSyntaxTree(A.substr(1, A.length()-2));
			}
		} else {
			while (ind < A.length() - 1) { //this works because last symbol can't be ( or +
				if (A[ind] == '(') {
					newTree = new SyntaxTree('.', makeSyntaxTree(A.substr(0, ind)), makeSyntaxTree(A.substr(ind, -1)));
					return newTree;
				} else if (A[ind] == '+') {
					newTree = new SyntaxTree('+', makeSyntaxTree(A.substr(0, ind)), makeSyntaxTree(A.substr(ind + 1, -1)));
					return newTree;
				}
				ind ++;
			}
			if (A[1] == '*') {
				SyntaxTree * newTree2 = new SyntaxTree('*', makeSyntaxTree(A.substr(0,1)));
				if (A.length() == 2) {
					newTree = newTree2;
				} else {
					newTree = new SyntaxTree('.', newTree2, makeSyntaxTree(A.substr(2, -1)));
				}
			} else {
				newTree = new SyntaxTree('.', makeSyntaxTree(A.substr(0,1)), makeSyntaxTree(A.substr(1, -1)));
			}
			return newTree;
		}
	}
}

NFA * convertToNFA(SyntaxTree * expression) {
	if (expression == nullptr) {			//what about epsilons?
		return nullptr;
	} else if (expression->getLeft() == nullptr) {
		NFA * singleton = new NFA(expression->getOp());
		return singleton;
	} else {
		return combineNFAs(expression->getOp(), convertToNFA(expression->getLeft()), convertToNFA(expression->getRight()));
	}
}

string preprocess (string A) {  //removes spaces, closed bracket pairs and double stars
	string newString = "";
	int ind = 0;
	while (ind < A.length()) {
		if (A[ind] != ' ') {
			newString += A[ind];
		}
		ind++;
	}
	ind = 1;
	while (ind < newString.length()) {
		if (newString[ind] == ')' && newString[ind-1] == '(') {
			newString.erase(newString.begin()+ind);
			newString.erase(newString.begin()+ind-1);
			ind--;
		} else if (newString[ind] == '*' && newString[ind-1] == '*') {
			newString.erase(newString.begin()+ind);
		} else {
			ind++;
		}
	}
	return newString;
}

SyntaxTree * createTree (string A) {
	return (makeSyntaxTree(preprocess(A)));
}
