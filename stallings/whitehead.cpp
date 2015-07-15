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


}  // namespace stallings
