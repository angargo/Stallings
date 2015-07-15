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
#include <fstream>
#include <sstream>

using namespace stallings;
using namespace std;

map<string, Subgroup> sgs;

void input(istream&);

void NotDefined(const string& name) {
	cout << "Subgroup " << name << " is not defined." << endl;
}

void SubgroupCommand(istream& in) {
	string name;
	int n;
	in >> name >> n;
	vector<Element> base(n);
	for (Element& ele : base) in >> ele;
	sgs[name] = Subgroup(base);
	cout << name << " = " << sgs[name] << endl;
}

void MemberCommand(istream& in) {
	string list;
	getline(in, list);
	Element element;
	in >> element;
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

void IntersectionCommand(istream& in) {
	string name1, name2, name3;
	in >> name1 >> name2 >> name3;
	if (sgs.count(name1) and sgs.count(name2)) {
		Subgroup inter = Subgroup::Intersection(sgs[name1], sgs[name2]);
		sgs[name3] = inter;
		cout << name3 << " = " << sgs[name3] << endl;
	} else {
		if (sgs.count(name1) == 0) NotDefined(name1);
		if (sgs.count(name2) == 0) NotDefined(name2);
	}
}

void IndexCommand(istream& in) {
	string name;
	in >> name;
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

void GraphCommand(istream& in) {
	string name;
	in >> name;
	if (sgs.count(name)) {
		cout << "Graph of subgroup " << name << endl;
		Subgroup& sg = sgs[name];
		sg.ShowStallingsGraph();
	} else NotDefined(name);
}

void FringeCommand(istream& in) {
	string name;
	in >> name;
	if (sgs.count(name)) {
		cout << "Fringe of subgroup " << name << endl;
		Subgroup& sg = sgs[name];
		vector<Subgroup> fringe = sg.GetFringe();
		for (const Subgroup& fsg : fringe) cout << fsg << endl;
	} else NotDefined(name);
}

void ImportCommand(istream& in) {
	string file;
	in >> file;
	ifstream fin(file);
	cout.setstate(ios_base::failbit);
	input(fin);
	cout.clear();
	fin.close();
}

void ListCommand(istream& in) {
	for (const pair<string, Subgroup>& p : sgs) {
		cout << endl << p.first << " " << p.second << endl;
	}
}

void ClearCommand(istream& in) {
	sgs.clear();
}

void input(istream& in) {
	string s;
	cout << "> ";
	while (in >> s) {
		if (s == "subgroup") SubgroupCommand(in);
		else if (s == "member") MemberCommand(in);
		else if (s == "intersection") IntersectionCommand(in);
		else if (s == "index") IndexCommand(in);
		else if (s == "graph") GraphCommand(in);
		else if (s == "fringe") FringeCommand(in);
		else if (s == "import") ImportCommand(in);
		else if (s == "list") ListCommand(in);
		else if (s == "clear") ClearCommand(in);
		else if (s == "exit") break;
		else cout << s << ": unknown command" << endl;
		cout << endl << "> ";
	}
}

int main() {
	input(cin);
}
