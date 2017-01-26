#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;

//TODO: change iterators to use auto; change functions to accept constant references; comment

class State {
	//Class for states within automata
	unordered_map<char, vector<State*>*> outgoing;
	unordered_map<char, vector<State*>*> incoming;
	bool final = false;
	int name;

public:
	
	State(int id) {
		name = id;
	}
	
	~State() {
		for (auto i = outgoing.begin(); i != outgoing.end();++i) {
			delete get<1>(*i);
		}
		for (auto i = incoming.begin(); i != incoming.end(); ++i) {
			delete get<1>(*i);
		}
	}

	int getName() {
		return name;
	}
	void setName(int id) {
		name = id;
	}
	bool isFinal() {
		return final;
	}
	void setFinal(bool status) {
		final = status;
	}
	
	void addOutgoing(char c, State* s) { //adds c-link from this state to state s
		if (outgoing.count(c) == 0) {
			vector<State*> * temp = new vector<State*>;
			temp->push_back(s);
			outgoing[c] = temp;
		} else { //checks if link already exists, if so skips adding
			for (auto i : (*outgoing[c])) {
				if (i == s) {
					return;
				}
			}
			outgoing[c]->push_back(s);
		}
	}
	
	void replaceOutgoing(char c, State* s) { //replaces any other c-links
		vector<State*> * temp = new vector<State*>;
		temp->push_back(s);
		if (outgoing.count(c) != 0){
			delete outgoing[c];
		}
		outgoing[c] = temp;
	}
	
	void setOutgoing(unordered_map<char, vector<State*>*> newOutgoing) {
		outgoing = newOutgoing;
	};
	
	void addIncoming(char c, State* s) { //adds c-link from state s state to this state
		if (incoming.count(c) == 0) {
			vector<State*> * temp = new vector<State*>;
			temp->push_back(s);
			incoming[c] = temp;
		} else { //checks if link already exists, if so skips adding
			for (auto i : (*incoming[c])) {
				if (i == s) {
					return;
				}
			}
			incoming[c]->push_back(s);
		}
	}
	
	void replaceIncoming(char c, State* s) { //replaces any other c-links
		vector<State*> * temp = new vector<State*>;
		temp->push_back(s);
		if (incoming.count(c) != 0){
			delete incoming[c];
		}
		incoming[c] = temp;
	}
	
	void setIncoming(unordered_map<char, vector<State*>*> newIncoming) {
		incoming = newIncoming;
	}
	
	unordered_map<char, vector<State*>*> getOutgoing() {
		return outgoing;
	}
	
	unordered_map<char, vector<State*>*> getIncoming() {
		return incoming;
	}
	
	vector<State*> * getOutgoing(char a) {
		if (outgoing.count(a) != 0) {
			return outgoing[a];
		} else {
			return nullptr;
		}
	}
	
	vector<State*> * getIncoming(char a) {
		if (incoming.count(a) != 0) {
			return incoming[a];
		} else {
			return nullptr;
		}
	}

	void removeOutgoing(char c, State* s) {
		(*outgoing[c]).erase(remove((*outgoing[c]).begin(), (*outgoing[c]).end(), s), (*outgoing[c]).end());
	}

	void removeIncoming(char c, State* s) {
		(*incoming[c]).erase(remove((*incoming[c]).begin(), (*incoming[c]).end(), s), (*incoming[c]).end());
	}

	void printOutgoing() {
		char c;		
		for (auto i = outgoing.begin(); i!= outgoing.end(); ++i) {
			c = get<0>(*i);
			for (auto j = (*get<1>(*i)).begin(); j!=(*get<1>(*i)).end(); ++j) {
				cout << "State " << name << " links to state " << (*j)->getName() << " " << *j << " with " << c << endl;
			}
		}
	}
	
	void printIncoming() {
		char c;
		for (auto i : incoming) {
			c = get<0>(i);
			for (auto j : *get<1>(i)) {
				cout << "State " << j->getName() << ": " << j << " links to state " << name << " with " << c << endl;
			}
		}
	}
	
};

class NFA {
	vector<State*> states;
	State * startState;
	vector<State*> finalStates;
	unordered_set<char> alphabet = {'a','b','c','d','e'};

public:
	
	NFA(unordered_set<char> alpha) {
		alphabet = alpha;
		int howManyStates;
		int start;
		int currentFinState;
		int originState;
		int destState;
		char letter;
		vector<int> finStates;
		vector<tuple<int,char,int>> transitions;
		cout << "How many states? (they will be indexed starting from 0)" << endl;
		cin >> howManyStates;
		cout << "Which is the starting state?" << endl;
		cin >> start;
		cout << "Enter, one by one, all final states, or -1 to finish" << endl;
		cin >> currentFinState;
		while (currentFinState != -1) {
			finStates.push_back(currentFinState);
			cout << "Enter, one by one, all final states, or -1 to finish" << endl;
			cin >> currentFinState;
		}
		cout << "Now enter each transition, or -1 to finish (use z for epsilon)." << endl;
		cout << "Origin state: ";
		cin >> originState;
		while (originState != -1) {
			cout << "Destination state: ";
			cin >> destState;
			cout << "Transition character: ";
			cin >> letter;
			transitions.push_back(make_tuple(originState, letter, destState));
			cout << "Now enter another transition, or -1 to finish." << endl;
			cout << "Origin state: ";
			cin >> originState;
		}
		
		State * temp = nullptr;
		for (int i = 0; i < howManyStates; ++i) {
			temp = new State(i);
			states.push_back(temp);
		}
		startState = states[start];
		for (auto i = finStates.begin(); i != finStates.end(); ++i) {
			states[(*i)]->setFinal(true);
			finalStates.push_back(states[(*i)]);
		}
		State * source = nullptr;
		State * dest = nullptr;
		for (auto i = transitions.begin(); i != transitions.end(); ++i) {
			source = states[get<0>(*i)];
			dest = states[get<2>(*i)];
			source->addOutgoing(get<1>(*i), dest);
			dest->addIncoming(get<1>(*i), source);
		}
	}
	
	NFA(int howManyStates, int start) {
		State * temp = nullptr;
		for (int i = 0; i < howManyStates; ++i) {
			temp = new State(i);
			states.push_back(temp);
		}
		startState = states[start];
	}
	
	NFA(int howManyStates, int start, vector<int> finStates) {
		State * temp = nullptr;
		for (int i = 0; i < howManyStates; ++i) {
			temp = new State(i);
			states.push_back(temp);
		}
		startState = states[start];
		
		for (auto i = finStates.begin(); i != finStates.end(); ++i) {
			states[(*i)]->setFinal(true);
			finalStates.push_back(states[(*i)]);
		}
	}
	
	NFA(int howManyStates, int start, vector<int> finStates, vector<tuple<int, char, int>> transitions) {
		State * temp = nullptr;
		for (int i = 0; i < howManyStates; ++i) {
			temp = new State(i);
			states.push_back(temp);
		}
		startState = states[start];
		for (auto i = finStates.begin(); i != finStates.end(); ++i) {
			states[(*i)]->setFinal(true);
			finalStates.push_back(states[(*i)]);
		}
		State * source = nullptr;
		State * dest = nullptr;
		for (auto i = transitions.begin(); i != transitions.end(); ++i) {
			source = states[get<0>(*i)];
			dest = states[get<2>(*i)];
			source->addOutgoing(get<1>(*i), dest);
			dest->addIncoming(get<1>(*i), source);
		}
	}
	
	NFA(char single) {
		State * temp;
		for (int i = 0; i < 2; i++) {
			temp = new State(i);
			states.push_back(temp);
		}
		startState = states[0];
		states[1]->setFinal(true);
		finalStates.push_back(states[1]);
		addLink(states[0], single, states[1]);
	}
	
	NFA(const NFA &object) {
		State * temp;
		for (auto i : object.states) {
			temp = new State(*i);
			states.push_back(temp);
			if (temp->isFinal()) {
				finalStates.push_back(temp);
			}
			if (i==object.startState) {
				startState = temp;
			}
			alphabet = object.alphabet;
		}
		for (auto i : states) {
			unordered_map<char, vector<State*>*> tempOutgoing;
			for (auto j : i->getOutgoing()) {
				char c = get<0>(j);
				vector<State*> * tempVec = new vector<State*>;
				for (auto k : *(get<1>(j))) {
					tempVec->push_back(states[k->getName()]);
				}
				tempOutgoing[c] = tempVec;
			}
			i->setOutgoing(tempOutgoing);
			
			unordered_map<char, vector<State*>*> tempIncoming;
			for (auto j : i->getIncoming()) {
				char c = get<0>(j);
				vector<State*> * tempVec = new vector<State*>;
				for (auto k : *(get<1>(j))) {
					tempVec->push_back(states[k->getName()]);
				}
				tempIncoming[c] = tempVec;
			}
			i->setIncoming(tempIncoming);
			
		}
	}
	
	void setAlphabet(unordered_set<char> alpha) {
		alphabet = alpha;
	}
	
	unordered_set<char> getAlphabet() {
		return alphabet;
	}

	vector<State*> getStates() {
		return states;
	}
	
	State * getStartState() {
		return startState;
	}
	
	vector<State*> getFinalStates() {
		return finalStates;
	}
	
	void addFinalState(State * next) {
		finalStates.push_back(next);
	}
	
	void replaceLink(State* source, char c, State* dest) {
		source->replaceOutgoing(c, dest);
		dest->replaceIncoming(c, source);
	}
	
	void addLink(State* source, char c, State* dest) {
		source->addOutgoing(c, dest);
		dest->addIncoming(c, source);
	}
	
	void addLink(int source, char c, int dest) {
		states[source]->addOutgoing(c, states[dest]);
		states[dest]->addIncoming(c, states[source]);
	}
	
	void removeLink(State* source, char c, State* dest) {
		source->removeOutgoing(c, dest);
		dest->removeIncoming(c, source);
	}
	
	void removeLink(int source, char c, int dest) {
		states[source]->removeOutgoing(c, states[dest]);
		states[dest]->removeIncoming(c, states[source]);
	}
	
	State * addState(bool start, bool final, vector<tuple<char,int>> out, vector<tuple<char,int>> in) {
		State * newState = new State(states.size());
		states.push_back(newState);
		State * other;
		if (start) {
			startState = newState;
		}
		newState->setFinal(final);
		if (final) {
			finalStates.push_back(newState);
		}
		for (auto i = out.begin(); i != out.end(); ++i) {
			other = states[get<1>(*i)];
			newState->addOutgoing(get<0>(*i), other);
			other->addIncoming(get<0>(*i), newState);
		}
		for (auto i = in.begin(); i != in.end(); ++i) {
			other = states[get<1>(*i)];
			newState->addIncoming(get<0>(*i), other);
			other->addOutgoing(get<0>(*i), newState);
		}
		return newState;
	}
	
	void star() {
		vector<tuple<char,int>> temp;
		for (auto i = finalStates.begin(); i != finalStates.end(); ++i) {
			tuple<char,int> t('z', (*i)->getName());
			temp.push_back(t);
		}
		tuple<char,int> t('z', startState->getName());
		vector<tuple<char,int>> out = {t};
		
		addState(true, true, out, temp);
	}
	
	void plus(NFA * other) {
		vector<State*> otherStates = other->getStates();
		State * otherStartState = other->getStartState();
		vector<State*> otherFinalStates = other->getFinalStates();
		
		for (auto i = otherStates.begin(); i != otherStates.end(); ++i) {
			(*i)->setName(states.size());
			states.push_back(*i);
			if ((*i)->isFinal()) {
				finalStates.push_back((*i));
			}
		}
		
		State * temp = new State(states.size());
		states.push_back(temp);
		addLink(temp, 'z', startState);
		addLink(temp, 'z', otherStartState);

		startState = temp;
	}
	
	void concat(NFA * other) {
		vector<State*> otherStates = other->getStates();
		State * otherStartState = other->getStartState();
		
		for (auto i = otherStates.begin(); i != otherStates.end(); ++i) {
			(*i)->setName(states.size());
			states.push_back(*i);
		}
		
		for (auto i = finalStates.begin(); i != finalStates.end(); ++i) {
			(*i)->setFinal(false);
			addLink(*i, 'z', otherStartState);
		}
		
		finalStates = other->getFinalStates();
	}
	
	void complement() { //SHOULD ONLY BE CALLED ON DFAs
		vector<State*> newFinal;
		for (auto i = states.begin(); i != states.end(); ++i) {
			if ((*i)->isFinal()) {
				(*i)->setFinal(false);
			} else {
				(*i)->setFinal(true);
				newFinal.push_back(*i);
			}
		}
		finalStates = newFinal;
	}
	
	void completeConnections() {
		State * sink = findSink();
		for (auto i : states) {
			for (auto j : alphabet) {
				if (i->getOutgoing().count(j) == 0 && j != 'z') {
					addLink(i, j, sink);
				}
			}
		}
	}
	
	State * findSink() {
		for (auto i : states) {
			if (!i->isFinal() && i->getOutgoing().empty()) {
				return i;
			}
		}
		vector<tuple<char,int>> null;
		return addState(0, 0, null, null);
	}
	
	NFA * createIntersection(NFA * other) {
		vector<State*> otherStates = other->getStates();
		int m = states.size();
		int n = otherStates.size();
		vector <int> finals;
		for (auto i = finalStates.begin(); i != finalStates.end(); ++i) {
			for (auto j = other->finalStates.begin(); j != other->finalStates.end(); ++j) {
				finals.push_back(n*(*i)->getName() + (*j)->getName());
			}
		}
		return createPairwiseNFA(other, finals);
	}
	
	NFA * createPairwiseNFA(NFA * other, vector<int> finals) {
		vector<State*> otherStates = other->getStates();
		int m = states.size();
		int n = otherStates.size();
		NFA * inters = new NFA(m*n, (n * startState->getName()) + other->getStartState()->getName(), finals);
		unordered_map<char,vector<State*>*> transitions1;
		unordered_map<char,vector<State*>*> transitions2;
		char c;
		for (auto i : states) {
			transitions1 = i->getOutgoing();
			for (auto j : otherStates) {
				transitions2 = j->getOutgoing();
				for (auto k : transitions1) {
					c = get<0>(k);
					if (c=='z') {
						for (auto p : (*get<1>(k))) {
							inters->addLink(n*i->getName() + j->getName(), c, n* p->getName() + j->getName());
						}
					}
					else if (transitions2.count(c) != 0) {
						for (auto p : (*get<1>(k))) {
							for (auto q : (*transitions2[c])) {
								inters->addLink(n*i->getName() + j->getName(), c, n* p->getName() + q->getName());
							}
						}
					}
				}
				for (auto k : transitions2) {
					c = get<0>(k);
					if (c=='z') {
						for (auto q : (*get<1>(k))) {
							inters->addLink(n*i->getName() + j->getName(), c, n* i->getName() + q->getName());
						}
					}
				}
			}
		}

		inters->removeUnreachableStates();
		
		return inters;
	}
	
	bool isLinear(State * state, char c) {
		unordered_map<char,vector<State*>*> transitions = state->getIncoming();
		if (transitions.count(c)!=1) {
			return false;
		}
		for (auto i : transitions) {
			if (get<0>(i) == c) {
				if ((*get<1>(i)).size() != 1) {
					return false;
				}
			} else {
				if ((*get<1>(i)).size() != 0) {
					return false;
				}
			}
		}
		return true;
	}
	
	void simplify() {
		set<State*> store;
		bool changed = false;
		for (auto i : states) {
			if (isLinear(i, 'z') && i!=startState) {
				store.insert(i);
				changed = true;
			}
		}
		for (auto i : store) {
			collapse(i);
		}
		if (changed) {
			removeUnreachableStates();
		}
	}
	
	void collapse(State * second) {
		State * origin = (*((second->getIncoming())['z'])).front();
		unordered_map<char,vector<State*>*> transitions = second->getOutgoing();
		for (auto i : transitions) {
			for (auto j : (*(get<1>(i)))) {
				addLink(origin, get<0>(i), j);
				removeLink(second, get<0>(i), j);
			}
		}
		removeLink(origin, 'z', second);
		if (second->isFinal() && !origin->isFinal()) {
			origin->setFinal(true);
			finalStates.push_back(origin);
		}
		second->printOutgoing();
		//removeState(second);
	}
	
	bool hasEpsilon() {
		for (auto i = states.begin(); i != states.end(); ++i) {
			if ((*i)->getOutgoing().count('z') != 0) {
				return true;
			}
		}
		return false;
	}
	
	bool hasMulti() {
		for (auto i = states.begin(); i != states.end(); ++i) {
			unordered_map<char, vector<State*>*> transitions = (*i)->getOutgoing();
			for (auto j = transitions.begin(); j != transitions.end(); ++j) {
				if (get<1>(*j)->size() > 1) {
					return true;
				}
			}
		}
		return false;
	}
	
	void removeLinks(State* current) {
		unordered_map<char, vector<State*>*> transitions = current->getOutgoing();
		for (auto i = transitions.begin(); i != transitions.end(); ++i) {
			for (auto j = get<1>(*i)->begin(); j != get<1>(*i)->end(); ++j) {
				(*j)->removeIncoming(get<0>(*i), current);
			}
		}
		transitions = current->getIncoming();
		for (auto i = transitions.begin(); i != transitions.end(); ++i) {
			for (auto j = get<1>(*i)->begin(); j != get<1>(*i)->end(); ++j) {
				(*j)->removeOutgoing(get<0>(*i), current);
			}
		}
	}
	
	void removeState(State* current) { //consider case where deleting start state!
		removeLinks(current);
		current->printOutgoing();
		states[current->getName()] = states[states.size() - 1];
		states[current->getName()]->setName(current->getName());
		states.erase(states.begin() + states.size() - 1);
		current->printOutgoing();
		if (current->isFinal()) {	//check to make sure this isn't dodgy
			for (auto i = finalStates.begin(); i != states.end();) {
				if ((*i) == current) {
					states.erase(i);
				} else {
					++i;
				}
			}
		}
		current->printOutgoing();
		delete current;
	}

	string exampleAccepted() {
		bool * explored = new bool[states.size()];
		for (int i = 0; i < states.size(); ++i) {
			explored[i] = 0;
		}
		string path = "";
		queue<tuple<State*, string>> store;
		State * current = nullptr;
		store.push(make_tuple(startState, path));
		explored[startState->getName()] = 1;
		if (startState->isFinal()) {
			return ("z");
		}
		unordered_map<char, vector<State*>*> transitions;
		
		while (!store.empty()) {
			current = get<0>(store.front());
			path = get<1>(store.front());
			store.pop();
			transitions = current->getOutgoing();
			for (auto i : transitions) {
				for (auto j : (*get<1>(i))) {
					if (explored[j->getName()] == 0) {
						if (j->isFinal()) {
							return (path + get<0>(i));
						} else {
							if (get<0>(i) == 'z') {
								store.push(make_tuple(j, path));
							} else {
								store.push(make_tuple(j, path + get<0>(i)));
							}
							explored[j->getName()] = 1;
						}
					}
				}
			}
		}
		cout << "This shouldn't happen" << endl;
		return "";
	}

	void removeUnreachableStates() {
		bool * linked = new bool[states.size()];
		for (int i = 0; i < states.size(); ++i) {
			linked[i] = 0;
		}
		stack<State*> store;
		State * current = nullptr;
		unordered_map<char, vector<State*>*> transitions;
		
		store.push(startState);
		linked[startState->getName()] = true;
		
		while (!store.empty()) {
			current = store.top();
			store.pop();
			transitions = current->getOutgoing();
			for (auto i = transitions.begin(); i != transitions.end(); ++i) {
				for (auto j = get<1>(*i)->begin(); j != get<1>(*i)->end(); ++j) {
					current = (*j);
					if (!linked[current->getName()]) {
						linked[current->getName()] = true;
						store.push(current);
					}
				}
			}
		}
		
		int numUnlinked = 0;
		int lIndex = 0;
		int rIndex = states.size()-1;
		State * temp;
		
		while (lIndex <= rIndex) {
			if (!linked[rIndex]) {
				rIndex--;
				numUnlinked++;
			} else if (linked[lIndex]) {
				lIndex++;
			} else {
				temp = states[rIndex];
				states[rIndex] = states[lIndex];
				states[lIndex] = temp;
				states[lIndex]->setName(lIndex);
				numUnlinked++;
				lIndex++;
				rIndex--;
			}
		}
		
		for (int i = 0; i < numUnlinked; ++i) {
			removeLinks(states[states.size()-1-i]);
		}
		for (int i = 0; i < numUnlinked; ++i) {
			temp = states[states.size()-1];
			states.erase(states.end()-1);
			delete temp;
		}
		vector<State*> tempFinal;
		for (auto i = states.begin(); i != states.end(); i++) {
			if ((*i)->isFinal()) {
				tempFinal.push_back(*i);
			}
		}
		finalStates = tempFinal;
		delete[] linked;
		simplify();
	}

	void print() {
	    cout << "Number of states: " << states.size() << endl;
	    cout << "Start State:";
	    cout << " " << startState->getName();
	    cout << endl << "Final States:";
	    for (auto i = finalStates.begin(); i != finalStates.end(); ++i) {
	        cout << " " << (*i)->getName();
	    }
	    cout << endl << "Links: " << endl;;
		for (auto i = states.begin(); i != states.end(); ++i) {
			(*i)->printOutgoing();
		}
	}
	
	void deleteStates() {
		for (auto i : states) {
			delete i;
		}
	}
	
	bool evaluate (string s) {
		for (auto i : getReachable(startState, 'z')) {
			if (evaluate(s, i)) {
				return true;
			}
		}
		return false;
	}
	
	bool evaluate (string s, State * start) {
		if (s.empty()) {
			for (auto i : getReachable(start, 'z')) {
				if (i->isFinal()) {
					return true;
				}
			}
		} else if (s.size()==1) {
			for (auto i : getReachable(start, s[0])) {
				if (i->isFinal()) {
					return true;
				}
			}
		} else {			
			for (auto i : getReachable(start, s[0])) {
				if (evaluate(s.substr(1), i)) {
					return true;
				}
			}
		}
		return false;
	}
	
	unordered_set<State*> getReachable(State * active, char c) {
		unordered_set<State*> reached;
		stack<State*> store;
		State * current;
	
		if (active->getOutgoing().count(c) != 0) {
			for (auto i : *(active->getOutgoing()[c])) {
				store.push(i);
			}
		}
	
		while (!store.empty()) {
			current = store.top();
			store.pop();
			reached.insert(current);
			if (current->getOutgoing().count('z') != 0) {
				for (auto i : *(current->getOutgoing()['z'])) {
					if (reached.count(i) == 0) {
						store.push(i);
					}
				}
			}
		}

		if (c=='z') {
			reached.insert(active);
		}
		return reached;
	}
};

NFA * combineWithIntersection(NFA * first, NFA * second) {
	return first->createIntersection(second);
}

int toInt(bool * current, int length) { //fix: this would be quicker with bit operations, but idk about unsigned vs signed ints
	int start = 0;
	int inc = 1;
	for (int i = 0; i < length; ++i) {
		if (current[length-1-i]) {
			start += inc;
		}
		inc *= 2;
	}
	return start;
}

void toArray(int now, bool * current, int length) { //look up bitwise
	unsigned int temp = now;
	for (int i = 0; i < length; ++i) {
		current[length-1-i] = temp&1;
		temp = temp >> 1;
	}
}

void getNextStateSet(bool * current, bool * nextStep, int length, NFA * first, char c) {
	for (int i = 0; i < length; ++i) {
		nextStep[i] = 0;
	}
	State * active;
	for (int i = 0; i < length; ++i) {
		if (current[length-1-i]) {
			active = first->getStates()[i];
			for (auto j : first->getReachable(active, c)) {
				nextStep[length - 1 - j->getName()] = 1;
			}
		}
	}
}

void createStateSetLink(bool * current, bool * nextStep, int length, char c, NFA * first, NFA * output) {
	State * source = output->getStates()[toInt(current, length)];
	State * dest = output->getStates()[toInt(nextStep, length)];
	output->addLink(source, c, dest);
}

NFA * createDFA(NFA * first) {
	NFA * output;
	if (!first->hasEpsilon() && !first->hasMulti()) { //checks if it's already essentially a DFA
		output = new NFA(*first);
		output->completeConnections(); //adds all connections to make it explicitly a DFA
	} else {
		int length = first->getStates().size(); //
		bool * current = new bool[length];
		bool * nextStep = new bool[length];
		for (int i = 0; i < length; ++i) {
			current[i] = 0;
			nextStep[i] = 0;
		}
		for (auto i : first->getReachable(first->getStartState(), 'z')) {
			current[length - 1 - i->getName()] = 1;
		}
		output = new NFA(pow(2, length), toInt(current, length));
		
		for (auto i : output->getStates()) {
			unsigned int num = i->getName();
			for (int j = 0; j < length; ++j) {
				if (num&1 && first->getStates()[j]->isFinal() && !i->isFinal()) {
					i->setFinal(true);
					output->addFinalState(i);
				}
				num = num >> 1;
			}
		}
		
		stack<int> store;
		store.push(toInt(current, length));
		int now;
		unordered_set<int> alreadyDone;
		while (!store.empty()) {
			now = store.top();
			store.pop();
			if (alreadyDone.count(now) == 0) {
				alreadyDone.insert(now);
				toArray(now, current, length);
				for (auto i : first->getAlphabet()) {
					if (i != 'z') {
						getNextStateSet(current, nextStep, length, first, i);
						createStateSetLink(current, nextStep, length, i, first, output);
						store.push(toInt(nextStep, length));
					}
				}
			}
		}
		delete[] current;
		delete[] nextStep;
		output->removeUnreachableStates();
	}
	return output;
}

NFA * getComplement(NFA * first) {
	NFA * output = createDFA(first);
	output->complement();
	return output;
}

bool checkEquivalence(NFA * first, NFA * second) {
	NFA * smaller;
	NFA * bigger;
	if (first->getStates().size() < second->getStates().size()) {	//makes sure the exponential code doesn't run unnecessarily
		smaller = first;
		bigger = second;
	} else {
		smaller = second;
		bigger = first;
	}
	NFA * temp2 = getComplement(smaller);
	NFA * temp3 = combineWithIntersection(temp2, bigger);
	temp2->deleteStates();
	delete temp2;
	
	temp3->removeUnreachableStates();
	
	if (temp3->getFinalStates().size()!=0) {
		cout << "These are not equivalent." << endl;
		cout << "Example failure: " << temp3->exampleAccepted() << endl;
		temp3->deleteStates();
		delete temp3;
		return false;
	}
	
	temp3->deleteStates();
	delete temp3;
	
	NFA * temp4 = getComplement(bigger);
	NFA * temp5 = combineWithIntersection(temp4, smaller);
	temp5->removeUnreachableStates();
	
	temp4->deleteStates();
	delete temp4;
	
	if (temp5->getFinalStates().size()!=0) {
		cout << "These are not equivalent." << endl;
		cout << "Example failure: " << temp5->exampleAccepted() << endl;
		temp5->deleteStates();
		delete temp5;
		return false;
	}
	cout << "These are equivalent." << endl;
	
	temp5->deleteStates();
	delete temp5;
	return true;
}

NFA * combineNFAs(char op, NFA * first, NFA * second) {
	if (op == '*') {
		first->star();
	} else if (op == '+') {
		first->plus(second);
		delete second;
	} else if (op == '.') {
		first->concat(second);
		delete second;
	}
	return first;
}
