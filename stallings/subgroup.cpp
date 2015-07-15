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

#include <subgroup.hpp>

#include <cassert>
#include <cmath>
#include <cassert>
#include <sstream>
#include <stack>
#include <functional>
#include <queue>

using namespace std;

namespace stallings {

Subgroup::Subgroup() : has_base(false), is_folded(false) {
	stallings_graph = Graph(1);  // Base vertex.
}

Subgroup::Subgroup(const vector<Element>& base_) : base(base_),
		has_base(true), is_folded(false){
	stallings_graph = Graph(1);  // Base vertex.
	for (const Element& element : base) {
		AddElement(element, stallings_graph);
	}

	Fold();
}

Subgroup::Subgroup(const Graph& graph) : has_base(true), is_folded(false) {
	// Compute spanning tree
	vector<tuple<int, int, int>> not_used;
	Graph st;
	graph.ComputeSpanningTree(st, not_used);

	// Shortest path from every node to the root
	vector<Edge> prev;
	vector<int> dist;
	st.AllShortestPaths(prev, dist);

	vector<Element> path(graph.Size());
	for (int i = 0; i < graph.Size(); ++i) {
		// Path from i to the root
		int u = i;
		while (dist[u] > 0) {
			path[i].push_back(prev[u].label);
			u = prev[u].v;
		}
	}

	// Build graph by closing the cycles.
	stallings_graph = Graph(1);
	for (auto& t : not_used) {
		int u, v, label;
		tie(u, v, label) = t;
		Element b = Product(Product(Inverse(path[u]), Element(1, label)), path[v]);
		AddElement(b, stallings_graph);
		base.push_back(move(b));
	}

	Fold();
}

void Subgroup::ShowFoldings() const {
	if (not is_folded) cout << "The graph is not folded." << endl;
	for (const Folding& fold : foldings) fold.Show();
}

void Subgroup::ShowStallingsGraph() const {
	if (not is_folded) cout << "The graph is not folded." << endl;
	cout << "------------- Stallings Graph -------------" << endl;
	stallings_graph.Show();
	cout << "-------------------------------------------" << endl;
}

void Subgroup::ShowBase() const {
	if (not has_base) cout << "The subgroup base is not computed yet." << endl;
	else {
		cout << "------------- Subgroup's base -------------" << endl;
		cout << "Elements in the base: " << base.size() << endl;
		for (const Element& element : base) cout << element << endl;
		cout << "-------------------------------------------" << endl;
	}
}

int Subgroup::GetBaseSize() const {
	return base.size();
}

Element Subgroup::GetBaseElement(int idx) const {
	assert((idx > 0 and idx <= int(base.size())) or (idx < 0 and idx >= -int(base.size())));
	if (idx > 0) return base[idx - 1];
	return Inverse(base[-idx - 1]);
}

void Subgroup::AddElement(const Element& element, Graph& graph) {
	// Add a 'petal'.
	int num_factors = element.size();
	int u = 0;  // Start at the base vertex.
	for (int i = 0; i < num_factors; ++i) {
		// If it is the last factor, close the petal, otherwise add a new vertex.
		int v = 0;
		if (i < num_factors - 1) {
			v = graph.Size();
			graph.AddVertex();
		}
		graph.AddEdge(u, v, element[i]);
		u = v;
	}
}

void Subgroup::Fold() {
	if (is_folded) return;

	// Get coordinates
	coordinates.clear();
	for (int i = 0; i < int(stallings_graph[0].size()); ++i) {
		coordinates[stallings_graph[0][i]] = (i % 2 == 0 ? i / 2 + 1 : - i / 2 - 1);
	}

	// Do foldings
	Folding fold;
	while (stallings_graph.FindRepeatedEdge(fold.u, fold.v, fold.w, fold.label)) {
		DoFolding(fold);
	}
	is_folded = true;
}

void Subgroup::DoFolding(Folding& fold) {
	Graph::Swap(fold.graph, stallings_graph);
	Graph& oldgraph = fold.graph;
	
	if (fold.v == fold.w) {
		// Copy the whole graph without one edge u-w
		stallings_graph = Graph(oldgraph.Size());
		bool skip = false, skiprev = false;
		for (int i = 0; i < oldgraph.Size(); ++i) {
			for (const Edge& edge : oldgraph[i]) {
				if (skip or i != fold.u or edge.v != fold.w or edge.label != fold.label) {
					if (skiprev or i != fold.w or edge.v != fold.u or edge.label != -fold.label) {
						stallings_graph.AddSingleEdge(i, edge.v, edge.label);
					} else {
						skiprev = true;
					}
				} else {
					skip = true;
				}
			}
		}
		assert(skip and skiprev);
	} else {
		// Ensure fold.v < fold.w. As we'll merge them both into fold.v, if one of
		// them is the 0, the result will still be 0.
		if (fold.v > fold.w) swap(fold.v, fold.w);
		bool skip = false, skiprev = false;
		stallings_graph = Graph(oldgraph.Size() - 1);
		for (int i = 0; i < oldgraph.Size(); ++i) {
			int ni = i;
			if (ni == fold.w) ni = fold.v;
			else if (ni > fold.w) --ni;
			for (const Edge& edge : oldgraph[i]) {
				int nv = edge.v;
				if (nv == fold.w) nv = fold.v;
				else if (nv > fold.w) --nv;
				if (skip or i != fold.u or edge.v != fold.w or edge.label != fold.label) {
					if (skiprev or i != fold.w or edge.v != fold.u or edge.label != -fold.label) {
						stallings_graph.AddSingleEdge(ni, nv, edge.label);
					} else {
						skiprev = true;
					}
				} else {
					skip = true;
				}
			}
		}
		assert(skip and skiprev);
	}
	foldings.push_back(move(fold));
}

bool Subgroup::Contains(const Element& element) const {
	int node = 0;
	for (const int& factor : element) {
		int v;
		if (stallings_graph.HasEdge(node, factor, v)) {
			node = v;
		}
		else return false;
	}
	return node == 0;
}

Path Subgroup::GetPath(const Element& element) const {
	Path path;
	int node = 0;
	for (const int& factor : element) {
		int v;
		assert(stallings_graph.HasEdge(node, factor, v));
		node = v;
		path.push_back(Edge(v, factor));
	}
	return path;
}

vector<int> Subgroup::GetCoordinates(const Element& element) const {
	Path path = GetPath(element);
	for (int i = int(foldings.size()) - 1; i >= 0; --i) {
		//cout << path << endl;
		path = foldings[i].RaisePath(path);
	}
	//cout << "Final path: " << path << endl;

	vector<int> res;
	int pos = 0;
	for (int i = 0; i < int(path.size()); ++i) {
		if (pos == 0) {
			auto it = coordinates.find(path[i]);
			assert(it != coordinates.end());
			res.push_back(it->second);
		}
		pos = path[i].v;
	}
	return res;
}

int Subgroup::Index(int rank) const {
	assert(is_folded);
	for (int i = 0; i < stallings_graph.Size(); ++i) {
		if (int(stallings_graph.const_list(i).size()) != 2 * rank) return INFINIT_INDEX;
	}
	return stallings_graph.Size();
}

int Subgroup::Index() const {
	return Index(stallings_graph.MaxLabel());
}

vector<Element> Subgroup::GetCosets() const {
	assert(Index() != INFINIT_INDEX);
	assert(is_folded);
	vector<Edge> prev;
	vector<int> dist;
	stallings_graph.AllShortestPaths(prev, dist);
	vector<Element> cosets(stallings_graph.Size());
	for (int i = 0; i < stallings_graph.Size(); ++i) {
		cosets[i] = Element(dist[i]);
		int u = i;
		while (dist[u] > 0) {
			cosets[i][dist[u] - 1] = -prev[u].label;
			u = prev[u].v;
		}
	}
	return cosets;
}

vector<Subgroup> Subgroup::GetFringe() const {
	vector<Subgroup> result;
	vector<int> ss(stallings_graph.Size());

	function<void(int,int)> Backtracking = [this, &Backtracking, &ss, &result](int i, int subsets) -> void {
		if (i == int(stallings_graph.Size())) {
			Graph qt;
			stallings_graph.ComputeQuotient(qt, ss);
			Subgroup nsg(qt);
			// Check if this subgroup is different from the previous ones.
			for (const Subgroup& sgr : result) if (sgr.Equals(nsg)) return;
			result.push_back(move(nsg));
			return;
		}
		for (int j = 0; j < subsets; ++j) {
			ss[i] = j;
			Backtracking(i + 1, subsets);
		}
		ss[i] = subsets;
		Backtracking(i + 1, subsets + 1);
	};

	Backtracking(0, 0);

	return result;
}

vector<Subgroup> Subgroup::GetAlgebraicExtensions() const {
	vector<Subgroup> ae;
	vector<Subgroup> fringe = GetFringe();
	// The set of algebraic extensions is the set of subgroups that do not have any
	// free factor in the fringe (a Takahasi family). This can be done more efficiently.
	int n = fringe.size();
	for (int i = 0; i < n; ++i) {
		bool alg = true;
		for (int j = 0; j < n; ++j) {
			if (i == j) continue;
			if (fringe[j].IsFreeFactorOf(fringe[i])) {
				alg = false;
				break;
			}
		}
		if (alg) ae.push_back(fringe[i]);
	}
	return GetFringe();
}

bool Subgroup::Equals(const Subgroup& sg) const {
	assert(is_folded and sg.IsFolded());
	return stallings_graph.IsIsomorphic(sg.stallings_graph);
}

bool Subgroup::IsSubgroupOf(const Subgroup& sg) const {
	assert(is_folded and sg.IsFolded());
	for (const Element& element : base) {
		if (not sg.Contains(element)) return false;
	}
	return true;
}

bool Subgroup::IsFreeFactorOf(const Subgroup& sg) const {
	if (not IsSubgroupOf(sg)) return false;
	// TODO
	return true;
}

Element Subgroup::Inverse(const Element& element) {
	Element ele;
	for (int i = int(element.size()) - 1; i >= 0; --i) ele.push_back(-element[i]);
	return ele;
}

Element Subgroup::Product(const Element& a, const Element& b) {
	Element p(a.size() + b.size());
	int k = 0;
	for (const int& f : a) {
		if (k == 0 or p[k - 1] != -f) p[k++] = f;
		else if (k > 0) --k;
	}
	for (const int& f : b) {
		if (k == 0 or p[k - 1] != -f) p[k++] = f;
		else if (k > 0) --k;
	}
	p.resize(k);
	return p;
}

Subgroup Subgroup::Intersection(const Subgroup& H, const Subgroup& K) {
	assert(H.IsFolded());
	assert(K.IsFolded());
	Graph pb = Graph::PullBack(H.stallings_graph, K.stallings_graph);

	// Trimming
	vector<int> deg(pb.Size());
	for (int i = 0; i < pb.Size(); ++i) deg[i] = pb[i].size();
	for (int i = 0; i < pb.Size(); ++i) {
		int p = i;
		while (p != 0 and deg[p] == 1) {
			--deg[p];
			for (int j = 0; j < int(pb[p].size()); ++j) if (deg[pb[p][j].v] != 0) {
				p = pb[p][j].v; --deg[p];
				break;
			}
		}
	}
	// We remove from the graphs the vertex with deg == 0 (except the root)

	// TODO Connected components. Currently we only get the main cc.
	stack<int> st;
	st.push(0);
	deg[0] = -1; // -1 means it will be in the graph
	while (not st.empty()) {
		int u = st.top();
		st.pop();
		int e = pb[u].size();
		for (int i = 0; i < e; ++i) {
			if (deg[pb[u][i].v] > 0) { // Not removed and not seen
				deg[pb[u][i].v] = -1;
				st.push(pb[u][i].v);
			}
		}
	}

	int k = 0; // Nodes in the new graph.
	for (int i = 0; i < pb.Size(); ++i) {
		if (deg[i] == -1) deg[i] = k++;
		else deg[i] = -1;
	}

	Graph gHK;
	gHK.Resize(k);
	for (int i = 0; i < pb.Size(); ++i) {
		if (deg[i] != -1) {
			int u = deg[i];
			for (const Edge& edge : pb[i]) {
				if (deg[edge.v] != -1) {
					int v = deg[edge.v];
					gHK[u].push_back(Edge(v, edge.label));
				}
			}
		}
	}

	Subgroup HK(gHK);

	return HK;
}

}  // namespace stallings

ostream& operator<<(ostream& out, const stallings::Subgroup& sg) {
	out << "<" << endl;
	for (const stallings::Element& element : sg.GetBase()) out << "(" << element << ")," << endl;
	out << ">";
	return out;
}

ostream& operator<<(ostream& out, const stallings::Element& element) {
	if (element.empty()) out << 0;
	for (int i = 0; i < int(element.size()); ++i) {
		if (i) out << " ";
		if (element[i] < 0) out << '-';
		out << char(abs(element[i]) + 'a' - 1);
	}
	return out;
}

istream& operator>>(istream& in, stallings::Element& element) {
	element.clear();
	string line;
	do {
		getline(in, line);
	} while (line.empty());
	stringstream ss(line);
	string factor;
	while (ss >> factor) {
		int f = 1;
		int p = 0;
		if (factor[p] == '-') {
			f = -1;
			++p;
		}
		int num = 0;
		while ('0' <= factor[p] and factor[p] <= '9') num = 10 * num + int(factor[p++] - '0');
		if ((f == 1 and p == 0) or (f == -1 and p == 1)) num = 1;
		assert(p + 1 == int(factor.size()));
		assert('a' <= factor[p]);
		while (num--) element.push_back(f * int(factor[p] - 'a' + 1));
	}
	return in;
}
