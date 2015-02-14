#include "subgroup.hpp"

#include <cassert>
#include <cmath>
#include <cassert>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

namespace stallings {

/******************************   Folding   *******************************/

void Folding::Show() const {
	graph.Show();
	cout << "Folding edges " << u << "-" << v << " and ";
	cout << u << "-" << w << ", with label " << label << "." << endl;
}

/******************************   Subgroup   ******************************/

Subgroup::Subgroup(const std::vector<Element>& base_) : base(base_),
		has_base(true), is_folded(false){
	// Create graph with a 'petal' for each element in the base.
	stallings_graph = Graph(1);  // Base vertex.
	for (const Element& element : base) {
		AddElement(element, stallings_graph);
	}
	
	has_base = true;
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
						stallings_graph[i].push_back(edge);
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
		if (fold.v > fold.w) std::swap(fold.v, fold.w);
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
						stallings_graph[ni].push_back(Edge(nv, edge.label));
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
	foldings.push_back(std::move(fold));
}

bool Subgroup::Contains(const Element& element) const {
	int node = 0;
	for (const short& factor : element) {
		int v;
		if (stallings_graph.HasEdge(node, factor, v)) {
			node = v;
		}
		else return false;
	}
	return node == 0;
}

}  // namespace stallings

std::ostream& operator<<(std::ostream& out, const stallings::Element& element) {
	for (int i = 0; i < int(element.size()); ++i) {
		if (i) out << " ";
		if (element[i] < 0) out << '-';
		out << char(std::abs(element[i]) + 'a' - 1);
	}
	return out;
}

std::istream& operator>>(std::istream& in, stallings::Element& element) {
	element.clear();
	string line;
	do {
		getline(in, line);
	} while (line.empty());
	std::stringstream ss(line);
	string factor;
	while (ss >> factor) {
		short f = 1;
		int p = 0;
		if (factor[p] == '-') {
			f = -1;
			++p;
		}
		int num = 0;
		while ('0' <= factor[p] and factor[p] <= '9') num = 10 * num + int(factor[p++] - '0');
		if ((f == 1 and p == 0) or (f == -1 and p == 1)) num = 1;
		assert(p + 1 == int(factor.size()));
		while (num--) element.push_back(f * int(factor[p] - 'a' + 1));
	}
	return in;
}
