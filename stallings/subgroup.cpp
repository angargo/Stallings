#include <subgroup.hpp>

#include <cassert>
#include <cmath>
#include <cassert>
#include <sstream>
#include <stack>

using namespace std;

namespace stallings {

Subgroup::Subgroup() : has_base(false), is_folded(false) {
	stallings_graph = Graph(1);  // Base vertex.
}

Subgroup::Subgroup(const vector<Element>& base_) : base(base_),
		has_base(true), is_folded(false){
	// Create graph with a 'petal' for each element in the base.
	stallings_graph = Graph(1);  // Base vertex.
	for (const Element& element : base) {
		AddElement(element, stallings_graph);
	}

	Fold();
}

Subgroup::Subgroup(const Graph& graph) : has_base(false), is_folded(true) {
	stallings_graph = graph;
	// TODO get a base trololol
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
	foldings.push_back(move(fold));
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

Path Subgroup::GetPath(const Element& element) const {
	Path path;
	int node = 0;
	for (const short& factor : element) {
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

Element Subgroup::Inverse(const Element& element) {
	Element ele;
	for (int i = int(element.size()) - 1; i >= 0; --i) ele.push_back(-element[i]);
	return ele;
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

	// TODO Connected components.
	stack<int> st;
	st.push(0);
	deg[0] = -1; // -1 means
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

ostream& operator<<(ostream& out, const stallings::Element& element) {
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
