/*
*   This file is part of Stallings-Calculator.
*
*   Stallings-Calculator is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NSMB Editor 5 is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Stallings-Calculator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <graph.hpp>
#include <subgroup.hpp>

#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>

using namespace stallings;
using namespace std;

map<string, Subgroup> sgs;

void NotDefined(const string& name) {
	cout << "Subgroup " << name << " is not defined." << endl;
}

void SubgroupCommand() {
	string name;
	int n;
	cin >> name >> n;
	vector<Element> base(n);
	for (Element& ele : base) cin >> ele;
	sgs[name] = Subgroup(base);
	cout << name << " = " << sgs[name] << endl;
}

void MemberCommand() {
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
					cout << "(" << ele << ")";
					p = Subgroup::Product(p, ele);
				}
				cout << endl << "Product: " << p << endl;
			} else cout << "(" << element << ")" << " is NOT a member of " << name << endl;
		} else NotDefined(name);
	}
}

void IntersectionCommand() {
	string name1, name2, name3;
	cin >> name1 >> name2 >> name3;
	if (sgs.count(name1) and sgs.count(name2)) {
		cerr << "Intersecting " << name1 << " " << name2 << endl;
		Subgroup inter = Subgroup::Intersection(sgs[name1], sgs[name2]);
		sgs[name3] = inter;
		cout << name3 << " = " << sgs[name3] << endl;
	} else {
		if (sgs.count(name1) == 0) NotDefined(name1);
		if (sgs.count(name2) == 0) NotDefined(name2);
	}
}

void IndexCommand() {
	string name;
	cin >> name;
	if (sgs.count(name)) {
		Subgroup& sg = sgs[name];
		int index = sg.Index();
		cout << "Subgroup " << name << " has index: ";
		if (index == Subgroup::INFINIT_INDEX) cout << "Infinite" << endl;
		else {
			cout << index << endl;
			vector<Element> repr = sg.GetCosets();
			for (const Element& ele : repr) cout << '(' << ele << ')' << endl;
		}
	} else NotDefined(name);
}

void GraphCommand() {
	string name;
	cin >> name;
	if (sgs.count(name)) {
		cout << "Graph of subgroup " << name << endl;
		Subgroup& sg = sgs[name];
		sg.ShowStallingsGraph();
	} else NotDefined(name);
}

void FringeCommand() {
	string name;
	cin >> name;
	if (sgs.count(name)) {
		cout << "Fringe of subgroup " << name << endl;
		Subgroup& sg = sgs[name];
		vector<Subgroup> fringe = sg.GetFringe();
		for (const Subgroup& fsg : fringe) cout << fsg << endl;
	} else NotDefined(name);
}

int main() {
	string s;
	while (cin >> s) {
		if (s == "subgroup") SubgroupCommand();
		else if (s == "member") MemberCommand();
		else if (s == "intersection") IntersectionCommand();
		else if (s == "index") IndexCommand();
		else if (s == "graph") GraphCommand();
		else if (s == "fringe") FringeCommand();
		else if (s == "exit") break;
		else cout << s << ": unknown command" << endl;
		cout << endl;
	}
}
