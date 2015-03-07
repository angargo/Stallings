#include <graph.hpp>
#include <subgroup.hpp>

#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>

using namespace stallings;
using namespace std;

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
	string s;
	map<string, Subgroup> sgs;
	while (cin >> s) {
		if (s == "subgroup") {
			string name;
			int n;
			cin >> name >> n;
			vector<Element> base(n);
			for (Element& ele : base) cin >> ele;
			sgs[name] = Subgroup(base);
			cout << name << " = " << sgs[name] << endl << endl;
		} else if (s == "member") {
			string list;
			getline(cin, list);
			Element element;
			cin >> element;
			stringstream ss(list);
			string name;
			while (ss >> name) {
				if (sgs.count(name)) {
					Subgroup& sg = sgs[name];
					if (sg.Contains(element)) {
						cout << "(" << element << ")" << " is a member of " << name << endl;
						vector<int> comb = sg.GetCoordinates(element);
						Element p;
						for (const int c : comb) {
							Element ele = sg.GetBaseElement(c);
							cout << " (" << ele << ")";
							p = Subgroup::Product(p, ele);
						}
						cout << endl << "Product: " << p << endl << endl;
					} else cout << "(" << element << ")" << " is NOT a member of " << name << endl;
				} else {
					cout << "Subgroup " << name << " is not defined." << endl;
				}
			}
		} else if (s == "intersection") {
			string name1, name2, name3;
			cin >> name1 >> name2 >> name3;
			if (sgs.count(name1) and sgs.count(name2)) {
				cerr << "Intersecting " << name1 << " " << name2 << endl;
				Subgroup inter = Subgroup::Intersection(sgs[name1], sgs[name2]);
				sgs[name3] = inter;
				cout << name3 << " = " << sgs[name3] << endl << endl;
			} else {
				if (sgs.count(name1) == 0) cout << "Subgroup " << name1 << " is not defined." << endl;
				if (sgs.count(name2) == 0) cout << "Subgroup " << name2 << " is not defined." << endl;
			}
		} else {
			cout << s << ": unknown command" << endl << endl;
		}
	}
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

				Element p;
				cout << "Word:";
				for (const int c : comb) {
					cout << " ";
					Element ele = sg.GetBaseElement(c);
					//if (c < 0) cout << "("; //reverse(ele.begin(), ele.end());
					cout << ele;
					p = Subgroup::Product(p, ele);
					//if (c < 0) cout << ")-1";
				}
				cout << endl;
				cout << "Product: " << p << endl << endl;
			}
			else {
				cout << element << " is not a member of the group." << endl;
			}
		}
		cout << "*******************************************" << endl << endl;
	}
}
