#include <string>
#include <iostream>

#include <tuple>
#include <vector>
#include <set>
#include <stack>

using namespace std;

class State {
	vector<tuple<State*, char>> incoming;
	vector<tuple<State*, char>> outgoing;
	bool start;
	bool final;
	int name;
public:
	State(int id) {
		name = id;
	}
	void setName(int id) {
		name = id;
	}
	void setStart(bool status) {
		start = status;
	}
	bool isStart() {
		return start;
	}
	void setFinal(bool status) {
		final = status;
	}
	bool isFinal() {
		return final;
	}
	void addIncoming(tuple<State*, char> link) {
		incoming.push_back(link);
	}
	void addOutgoing(tuple<State*, char> link) {
		outgoing.push_back(link);
	}
	int getName() {
		return name;
	}
	vector<tuple<State*, char>> getIncoming() {
		return incoming;
	}
	vector<tuple<State*, char>> getOutgoing() {
		return outgoing;
	}
	void removeOutgoing(State* remove) {
		for (int i = 0; i < outgoing.size(); i++) {
			if (get<0>(outgoing[i]) == remove) {
				cout << "Removing link between " << remove->getName() << " and " << name << endl;
				outgoing.erase(outgoing.begin()+i);
			}
		}
	}
	void removeIncoming(State* remove) {
		for (int i = 0; i < incoming.size(); i++) {
			if (get<0>(incoming[i]) == remove) {
				cout << "Removing link between " << remove->getName() << " and " << name << endl;
				incoming.erase(incoming.begin()+i);
			}
		}
	}
};

class DFA {
	int numStates;
	vector<State*> states;
	vector<State*> startStates;
	vector<State*> finalStates;
public:
	DFA(int howManyStates, vector<int> sStates, vector<int> fStates, vector<tuple<int, int, char>> transitions) {
		State * first = NULL;
		State * second = NULL;
		numStates = howManyStates;
		State * temp = NULL;
		for (int i = 0; i < numStates; i++) {
			temp = new State(i);
			states.push_back(temp);
		}
		for (int i = 0; i < sStates.size(); i++) {
			states[sStates[i]]->setStart(true);
			startStates.push_back(states[sStates[i]]);
		}
		for (int i = 0; i < fStates.size(); i++) {
			states[fStates[i]]->setFinal(true);
			finalStates.push_back(states[fStates[i]]);
		}
		for (int i = 0; i < transitions.size(); i++) {
			first = states[get<0>(transitions[i])];
			second = states[get<1>(transitions[i])];
			tuple<State*, char> tup1(second, get<2>(transitions[i]));
			tuple<State*, char> tup2(first, get<2>(transitions[i]));
			first->addOutgoing(tup1);
			second->addIncoming(tup2);
		}
	}
	
	void addState(bool start, bool final, vector<tuple<int,char>> outgoing, vector<tuple<int,char>> incoming) {
		State * temp = new State(numStates);
		State * current;
		temp->setStart(start);
		temp->setFinal(final);
		for (int i = 0; i < outgoing.size(); i++) {
			current = states[get<0>(outgoing[i])];
			temp->addOutgoing(make_tuple(current, get<1>(outgoing[i])));
			current->addIncoming(make_tuple(temp, get<1>(outgoing[i])));
		}
		for (int i = 0; i < incoming.size(); i++) {
			current = states[get<0>(incoming[i])];
			temp->addIncoming(make_tuple(current, get<1>(incoming[i])));
			current->addOutgoing(make_tuple(temp, get<1>(incoming[i])));
		}
		numStates++;
		states.push_back(temp);
	}
	
	void removeLinks(State* current) {
		State * current2;
		vector<tuple<State*, char>> transitions = current->getOutgoing();
		for (int j = 0; j < transitions.size(); j++) {
			current2 = get<0>(transitions[j]);
			current2->removeIncoming(current);
		}
		transitions = current->getIncoming();
		for (int j = 0; j < transitions.size(); j++) {
			current2 = get<0>(transitions[j]);
			current2->removeOutgoing(current);
		}
	}
	
	void removeState(State* current) {
		removeLinks(current);
		for (int i = 0; i < numStates; i++) {
			if (states[i] == current) {
				states.erase(states.begin() + i);
			}
		}
		for (int i = 0; i < startStates.size(); i++) {
			if (startStates[i] == current) {
				startStates.erase(startStates.begin() + i);
			}
		}
		for (int i = 0; i < finalStates.size(); i++) {
			if (finalStates[i] == current) {
				finalStates.erase(finalStates.begin() + i);
			}
		}
		numStates--;
		delete current;
	}
	
	void removeUnlinkedStates() {
		
		bool * linked = new bool[numStates];
		for (int i = 0; i < numStates; i++) {
			linked[i] = 0;
		}
		stack<State*> * store = new stack<State*>();
		State * current = NULL;
		State * current2 = NULL;
		vector<tuple<State*, char>> transitions;
		int numUnlinked = 0;
		int lIndex = 0;
		int rIndex = numStates-1;
		
		for (int i=0; i < startStates.size(); i++) {
			store->push(startStates[i]);
			linked[startStates[i]->getName()] = true;
		}
		while (!store->empty()) {
			current = store->top();
			store->pop();
			transitions = current->getOutgoing();
			for (int i = 0; i < transitions.size(); i++) {
				current2 = get<0>(transitions[i]);
				if (!linked[current2->getName()]) {
					linked[current2->getName()] = true;
					store->push(current2);
				}
			}
			
		}
		
		for (int i = 0; i < numStates; i++) {
			if (!linked[i]) {
				numUnlinked++;
			}
		}
		while (lIndex < rIndex) {
			if (!linked[lIndex]) {
				if (linked[rIndex]) {
					current = states[rIndex];
					states[rIndex] = states[lIndex];
					states[lIndex] = current;
					current->setName(lIndex);			//need to set name of soon-to-be-removed as well?
					rIndex--;
					lIndex++;
				} else {
					rIndex--;
				}
			} else {
				lIndex++;
			}
		}
		for (int i = 0; i < numUnlinked; i++) {
			removeLinks(states[numStates-1-i]);
		}
		
		vector<State*> tempStates;
		for (int i = 0; i < numStates-numUnlinked; i++) {
			tempStates.push_back(states[i]);
		}
		for (int i = numStates - numUnlinked; i < numStates; i++) {
			delete states[i];
		}
		states = tempStates;
		numStates = numStates - numUnlinked;
		vector<State*> tempFinal;
		for (int i = 0; i < numStates; i++) {
			if (states[i]->isFinal()) {
				tempFinal.push_back(states[i]);
			}
		}
		finalStates = tempFinal;
	}

	void printDFA() {
	    cout << "Number of states: " << numStates << endl;
	    cout << "Start States:";
	    for (int i = 0; i < startStates.size(); i++) {
	        cout << " " << startStates[i]->getName();
	    }
	    cout << endl << "Final States:";
	    for (int i = 0; i < finalStates.size(); i++) {
	        cout << " " << finalStates[i]->getName();
	    }
	    cout << endl << "Links: ";
		for (int i = 0; i < numStates; i++) {
			for (int j = 0; j < states[i]->getOutgoing().size(); j++) {
				cout << "State " << states[i]->getName() << " links to state " << get<0>(states[i]->getOutgoing()[j])->getName() << " with " << get<1>(states[i]->getOutgoing()[j]) << endl;
			}
		}
	}
};

int main() {
	vector<int> first = {1};
	vector<int> second = {0, 1,2,3, 4};
	vector<tuple<int,int,char>> third = {make_tuple(3,4,'d'), make_tuple(2,3,'f'), make_tuple(0,4,'g')};
	tuple<int,int,char> hi(1,2,'c');
	third.push_back(hi);
	DFA * myD = new DFA(5, first,second,third);
	myD->printDFA();
	myD->removeUnlinkedStates();
	myD->printDFA();
}