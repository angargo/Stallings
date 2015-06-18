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

#ifndef FOLDING_HPP
#define FOLDING_HPP

#include <graph.hpp>

namespace stallings {

// Stores a graph and the folding applied to that graph.
// v <--(label)-- u --(label)--> w
// If v < w. Nodes with index >= w are displaced, otherwise v = w.

class Folding {
 public:
	Folding() : graph(), u(0), v(0), w(0), label(0) {}

	void Show() const;

	Path RaisePath(const Path& path) const;

	Graph graph;

	// Edges u-v and u-w both have the same label.
	int u, v, w, label;
};

}  // namespace stallings

#endif // FOLDING_HPP
