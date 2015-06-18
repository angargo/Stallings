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

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <stack>

using namespace std;

namespace stallings {

void Edge::Show() const {
	cout << "(" << v << "," << (label > 0 ? "" : "-") << char(abs(label) + 'a' - 1) << ")";
}

void Graph::AddEdge(int u, int v, int label) {
	max_label = max(abs(label), max_label);
	list[u].push_back(Edge(v, label));
	list[v].push_back(Edge(u, -label));
}

void Graph::AddSingleEdge(int u, int v, int label) {
	max_label = max(abs(label), max_label);
	list[u].push_back(Edge(v, label));
}

void Graph::AddVertex() {
	Resize(num_vertex + 1);
}

void Graph::Resize(int size) {
	assert(size >= num_vertex);
	num_vertex = size;
	list.resize(num_vertex);
}

bool Graph::FindRepeatedEdge(int& u, int& v, int& w, int& label) const {
	for (u = 0; u < num_vertex; ++u) {
		map<int, int> m;
		for (const Edge& edge : list[u]) {
			if (m.count(edge.label)) {  // Repeated edge
				v = edge.v;
				w = m[edge.label];
				label = edge.label;
				return true;
			} else m[edge.label] = edge.v;
		}
	}
	return false;
}

bool Graph::HasEdge(int u, int label, int& v) const {
	for (const Edge& edge : list[u]) {
		if (edge.label == label) {
			v = edge.v;
			return true;
		}
	}
	return false;
}

bool Graph::HasExactEdge(int u, int label, int v) const {
	for (const Edge& edge : list[u]) {
		if (edge.label == label and edge.v == v) return true;
	}
	return false;
}

void Graph::AllShortestPaths(std::vector<Edge>& prev, std::vector<int>& dist) const {
	prev = vector<Edge>(num_vertex);
	dist = vector<int>(num_vertex, -1);
	queue<int> q;
	q.push(0);
	dist[0] = 0;
	while (not q.empty()) {
		int u = q.front(); q.pop();
		for (const Edge& edge : list[u]) {
			if (dist[edge.v] == -1) { // Not seen
				dist[edge.v] = dist[u] + 1;
				prev[edge.v] = Edge(u, -edge.label);
				q.push(edge.v);
			}
		}
	}
}

void Graph::ComputeSpanningTree(Graph& st, vector<tuple<int, int, int>>& not_used) const {
	st = Graph(num_vertex);
	vector<int> root(num_vertex);
	for (int i = 0; i < num_vertex; ++i) root[i] = i;
	function<int(int)> Root = [&root, &Root](int u) -> int {
		if (root[u] == u) return u;
		return root[u] = Root(root[u]);
	};
	for (int i = 0; i < num_vertex; ++i) {
		for (const Edge& edge : list[i]) {
			int ri = Root(i), rj = Root(edge.v);
			if (ri != rj) {
				root[ri] = rj;
				st.AddEdge(i, edge.v, edge.label);
			} else if (i < edge.v or (i == edge.v and edge.label > 0))
				// We need the if to avoid repeating edges.
				not_used.push_back(make_tuple(i, edge.v, edge.label));
		}
	}
}

void Graph::ComputeQuotient(Graph& qt, const std::vector<int>& relation) const {
	assert(num_vertex == int(relation.size()));

	int nodes = 0;
	for (const int& subset : relation) nodes = max(nodes, subset + 1);

	qt.Resize(nodes);
	vector<vector<set<int>>> q_edges(nodes, vector<set<int>>(nodes));
	for (int i = 0; i < num_vertex; ++i) {
		int ni = relation[i];
		for (const Edge& edge : list[i]) {
			int nv = relation[edge.v];
			if (q_edges[ni][nv].count(edge.label) == 0) {
				q_edges[ni][nv].insert(edge.label);
				q_edges[nv][ni].insert(-edge.label);
				qt.AddEdge(ni, nv, edge.label);
			}
		}
	}
}

bool Graph::IsIsomorphic(const Graph& g) const {
	if (num_vertex != g.Size()) return false;
	if (max_label != g.MaxLabel()) return false;
	vector<int> v(num_vertex, -1);
	v[0] = 0; // The root is the same
	stack<int> st;
	st.push(0);
	while (not st.empty()) {
		int u = st.top();
		st.pop();
		int u2 = v[u];
		vector<int> next(2 * max_label + 1, -1);
		if (g.const_list(u2).size() != list[u].size()) return false;
		for (const Edge& edge : g.const_list(u2)) next[max_label + edge.label] = edge.v;
		for (const Edge& edge : list[u]) {
			int n2 = next[max_label + edge.label];
			if (n2 == -1) return false;
			if (v[edge.v] == -1) {
				v[edge.v] = n2;
				st.push(edge.v);
			}
			else if (v[edge.v] != n2) return false;
		}
	}
	return true;
}

void Graph::Show() const {
	assert(int(list.size()) == num_vertex);
	cout << list.size() << endl;
	for (int i = 0; i < num_vertex; ++i) {
		for (const Edge& edge : list[i]) {
			edge.Show();
			cout << " ";
		}
		cout << endl;
	}
}

void Graph::Swap(Graph& g1, Graph& g2) {
	swap(g1.list, g2.list);
	swap(g1.num_vertex, g2.num_vertex);
}

vector<vector<pair<int, int>>> Graph::ListEdgesByLabel() const {
	vector<vector<pair<int, int>>> res;
	for (int i = 0; i < num_vertex; ++i) {
	 for (const Edge& edge : list[i]) {
		 if (edge.label > 0) {
			 if (edge.label >= int(res.size())) res.resize(edge.label + 1);
			 res[edge.label].push_back(make_pair(i, edge.v));
		 }
	 }
	}
	return res;
}

Graph Graph::PullBack(const Graph& gH, const Graph& gK) {
	Graph pb;

	vector<vector<pair<int, int>>> lH = gH.ListEdgesByLabel();
	vector<vector<pair<int, int>>> lK = gK.ListEdgesByLabel();

	map<pair<int, int>, int> mindex;
	mindex[make_pair(0, 0)] = 0;
	auto Index = [&mindex](int u, int v) {
		if (mindex.count(make_pair(u, v))) return mindex[make_pair(u, v)];
		return mindex[make_pair(u, v)] = mindex.size();
	};

	int nlabel = min(lH.size(), lK.size());
	for (int l = 1; l < nlabel; ++l) {
		for (const pair<int, int>& eH : lH[l]) {
			for (const pair<int, int>& eK : lK[l]) {
				int idu = Index(eH.first, eK.first), idv = Index(eH.second, eK.second);
				if (max(idu, idv) >= pb.Size()) pb.Resize(max(idu, idv) + 1);
				pb.AddEdge(idu, idv, l);
			}
		}
	}

	return pb;
}

} // namespace stallings

ostream& operator<<(ostream& out, const stallings::Path& path) {
	out << 0;
	for (const stallings::Edge& edge : path)
		out << " --(" << (edge.label > 0 ? "" : "-") << char(abs(edge.label) + 'a' - 1) << ")--> " << edge.v;
	return out;
}
