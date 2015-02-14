#include "graph.hpp"

#include <iostream>
#include <map>
#include <cstdlib>
#include <cassert>

namespace stallings {

void Edge::Show() const {
	std::cout << "(" << v << "," << (label > 0 ? "" : "-") << char(abs(label) + 'a' - 1) << ")";
}

void Graph::AddEdge(int u, int v, int label) {
	list[u].push_back(Edge(v, label));
	list[v].push_back(Edge(u, -label));
}

void Graph::AddVertex() {
	++num_vertex;
	list.resize(num_vertex);
}

bool Graph::FindRepeatedEdge(int& u, int& v, int& w, int& label) const {
	for (u = 0; u < num_vertex; ++u) {
		std::map<int, int> m;
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

void Graph::Show() const {
	using std::cout;
	assert(int(list.size()) == num_vertex);
	cout << list.size() << std::endl;
	for (int i = 0; i < num_vertex; ++i) {
		for (const Edge& edge : list[i]) {
			edge.Show();
			cout << " ";
		}
		cout << std::endl;
	}
}

void Graph::Swap(Graph& g1, Graph& g2) {
	std::swap(g1.list, g2.list);
	std::swap(g1.num_vertex, g2.num_vertex);
}

} // namespace stallings