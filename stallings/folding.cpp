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

#include <folding.hpp>
#include <cassert>

namespace stallings {

using namespace std;

void Folding::Show() const {
	graph.Show();
	cout << "Folding edges " << u << "-" << v << " and ";
	cout << u << "-" << w << ", with label " << (label > 0 ? "" : "-") << char(abs(label) + 'a' - 1) << "." << endl;
}

Path Folding::RaisePath(const Path& path) const {
	Path newpath;
	int pos = 0; // Start at the base node
	for (const Edge& edge : path) {
		int next = edge.v;
		if (next >= w and w != v) ++next; // Indexes greater than w are displaced because of erasing w
		if (next == v or next == w) {
			if ((not graph.HasExactEdge(pos, edge.label, v)) and not graph.HasExactEdge(pos, edge.label, w)) {
				// Lets go back through the folding for free
				int ori = v, dest = w;
				if (pos == w) swap(ori, dest);
				assert(pos == ori);
				newpath.push_back(Edge(u, -label));
				newpath.push_back(Edge(dest, label));
				pos = dest;
			}
			bool b1 = graph.HasExactEdge(pos, edge.label, v);
			bool b2 = graph.HasExactEdge(pos, edge.label, w);
			assert(b1 or b2);
			if (b1) pos = v;
			else pos = w;
			newpath.push_back(Edge(pos, edge.label));
		}
		else {
			if (not graph.HasExactEdge(pos, edge.label, next)) {
				// Lets go back through the folding for free
				int ori = v, dest = w;
				if (pos == w) swap(ori, dest);
				assert(pos == ori);
				newpath.push_back(Edge(u, -label));
				newpath.push_back(Edge(dest, label));
				pos = dest;
			}
			assert(graph.HasExactEdge(pos, edge.label, next));
			newpath.push_back(Edge(next, edge.label));
			pos = next;
		}
	}
	if (pos != 0) {
		assert(pos == w and v == 0);
		newpath.push_back(Edge(u, -label));
		newpath.push_back(Edge(0, label));
	}

	// Clean back edges
	bool goback = false;
	do {
		goback = false;
		int prev = 0;
		int pos = newpath[0].v;
		for (int i = 1; i < int(newpath.size()); ++i) {
			if (newpath[i].v == prev and newpath[i].label == -newpath[i - 1].label) {
				Path another;
				for (int j = 0; j < int(newpath.size()); ++j) {
					if (j != i and j != i - 1) another.push_back(newpath[j]);
				}
				swap(another, newpath);
				goback = true;
				break;
			}
			prev = pos;
			pos = newpath[i].v;
		}
	} while (goback);

	return newpath;
}

}  // namespace stallings
