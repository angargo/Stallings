#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>

namespace stallings {

class Edge {
 public:
	Edge() : v(0), label(0) {}
	Edge(int v_, int label_) : v(v_), label(label_) {}
	int v, label;
	void Show() const;
};

typedef std::vector<Edge> Adj;
typedef std::vector<Adj> AdjList;

class Graph {
 public:
	// Create an empty graph.
	Graph() : num_vertex(0), list(0) {}
	
	// Create an empty graph with 'n' nodes.
	explicit Graph(int n) : num_vertex(n), list(n) {}
	
	// Return the number of nodes.
	int Size() const {
		return num_vertex;
	}
	
	// Add the specified edge to the graph.
	void AddEdge(int u, int v, int label);
	
	// Add a new vertex to the graph.
	void AddVertex();
	
	// Find a repeated edge in the graph.
	bool FindRepeatedEdge(int& u, int& v, int& w, int& label) const;
	
	// Return true if there is an edge in u with specified label, and the
	// neighbour through this edge.
	bool HasEdge(int u, int label, int& v) const;
	
	// Prints the graph.
	void Show() const;
	
	// Swap two graphs.
	static void Swap(Graph& g1, Graph& g2);
	
	Adj& operator[](int idx) {
		return list[idx];
	}

 private:
	int num_vertex;
	AdjList list;
};

}  // namespace stallings

#endif