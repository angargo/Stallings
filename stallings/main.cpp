#include <graph.hpp>
#include <subgroup.hpp>

#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>

using namespace stallings;
using namespace std;

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
				cout << "(" << element << ")" << " is a member of the group." << endl;
				vector<int> comb = sg.GetCoordinates(element);

				cout << "Coordinates:";
				for (const int& c : comb) cout << " " << c;
				cout << endl;

				cout << "Word:";
				for (const int c : comb) {
					cout << " ";
					Element ele = sg.GetBaseElement(c);
					//if (c < 0) cout << "("; //reverse(ele.begin(), ele.end());
					cout << ele;
					//if (c < 0) cout << ")-1";
				}
				cout << endl << endl;
			}
			else {
				cout << element << " is not a member of the group." << endl;
			}
		}
		cout << "*******************************************" << endl << endl;
	}
}
