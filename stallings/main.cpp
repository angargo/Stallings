#include "graph.hpp"
#include "subgroup.hpp"

#include <vector>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;
using std::vector;

using stallings::Element;
using stallings::Graph;
using stallings::Subgroup;

bool ReadElement(Element& element) {
	string line;
	if (getline(cin, line)) {
		stringstream ss(line);
		short factor;
		while (ss >> factor) element.push_back(factor);
		return true;
	}
	return false;
}

bool ReadBase(vector<Element>& base) {
	int n;
	if (cin >> n) {
		base = vector<Element>(n);
		for (Element& element : base) cin >> element;
		return true;
	}
	return false;
}

int main() {
	vector<Element> base;
	while (ReadBase(base)) {
		cout << endl;
		Subgroup sg(base);
		sg.ShowBase();
		cout << endl;
		
		//sg.ShowFoldings();
		//cout << endl;
		
		sg.ShowStallingsGraph();
		cout << endl;
		
		int q;
		cin >> q;
		for (int i = 0; i < q; ++i) {
			Element element;
			cin >> element;
			if (sg.Contains(element)) {
				cout << element << " is a member of the group." << endl;
			}
			else {
				cout << element << " is not a member of the group." << endl;
			}
		}
		cout << "*******************************************" << endl << endl;
	}
}
