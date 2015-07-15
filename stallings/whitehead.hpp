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

#ifndef WHITEHEAD_HPP
#define WHITEHEAD_HPP

#include <functional>
#include <subgroup.hpp>
#include <set>

namespace stallings {

class Whitehead {
	public:
		static std::function<Element(const Element&)> GetWhitehead(int s, const std::set<int>& scut);
};

}  // namespace stallings

#endif // WHITEHEAD_HPP
