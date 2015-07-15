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

#include <whitehead.hpp>
#include <cassert>

using namespace std;

namespace stallings {

function<Element(const Element&)> Whitehead::GetWhitehead(int s, const set<int>& scut) {
	return [=](const Element& element) -> Element {
		Element res;
		for (const int& factor : element) {
			if (factor == s or factor == -s) res.push_back(factor);
			else {
				if (scut.count(-factor)) res.push_back(-s);
				res.push_back(factor);
				if (scut.count(factor)) res.push_back(s);
			}
		}
		return Subgroup::Reduce(res);
	};
}

bool Whitehead::Reduce(vector<Element>& base, int rank) {
	int oldsize = 0, bs = base.size();
	for (const Element& element : base) oldsize += element.size();
	//cerr << "Current size: " << oldsize << endl;

	int mxmask = 1 << (2 * rank);
	for (int m = 0; m < mxmask; ++m) {
		// [0, rank) = a, b, c, d, ...; [rank, 2 * rank) = -a, -b, -c, -d, ...
		set<int> scut;
		for (int i = 0; i < rank; ++i) if ((m & (1 << i))) scut.insert(i + 1);
		for (int i = rank; i < rank + rank; ++i) if ((m & (1 << i))) scut.insert(-(i - rank + 1));
		for (int s = -rank; s <= rank; ++s) {
			if (s == 0) continue;
			if (scut.count(s) and not scut.count(-s)) {
				//cerr << s << " {";
				//for (int k : scut) cerr << k << ",";
				//cerr << "}" << endl;
				auto phi = GetWhitehead(s, scut);
				vector<Element> b2(bs);
				int newsize = 0;
				for (int i = 0; i < bs; ++i) {
					b2[i] = phi(base[i]);
					//cerr << b2[i] << endl;
					newsize += b2[i].size();
				}
				//cerr << newsize << endl;
				if (newsize < oldsize) {
					swap(base, b2);
					return true;
				}
			}
		}
	}
	return false;
}

bool Whitehead::WhiteheadMinimizationProblem(vector<Element> base, int rank) {
	assert(rank < 13); // Just to put a limit.
	//cerr << "Minimizing with rank " << rank << endl;
	//for (const Element& element : base) cerr << element << endl;
	//cerr << endl;
	while (Reduce(base, rank)) {
		//cerr << "Reduced:" << endl;
		//for (const Element& element : base) cerr << element << endl;
		//cerr << endl;
	}
	int size = 0;
	for (const Element& element : base) size += element.size();
	return size == int(base.size());
}


}  // namespace stallings
