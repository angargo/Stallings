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

#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <iostream>

namespace stallings {

class Edge {
 public:
	Edge() : v(0), label(0) {}

	Edge(int v_, int label_) : v(v_), label(label_) {}

	bool operator<(const Edge& b) const {
		if (v == b.v) return label < b.label;
		return v < b.v;
	}

	int v, label;
	void Show() const;
};

typedef std::vector<Edge> Path;

typedef std::vector<Edge> Adj;
typedef std::vector<Adj> AdjList;

class Graph {
 public:
	// Create an empty graph.
	Graph() : num_vertex(0), max_label(0), list(0) {}
	
	// Create an empty graph with 'n' nodes.
	explicit Graph(int n) : num_vertex(n), max_label(0), list(n) {}
	
	// Return the number of nodes.
	int Size() const {
		return num_vertex;
	}
	void Resize(int size);

	int MaxLabel() const {
		return max_label;
	}
	
	// Add the specified edge to the graph.
	void AddEdge(int u, int v, int label);        // Bidirectional
	void AddSingleEdge(int u, int v, int label);  // Only in one direction
	
	// Add a new vertex to the graph.
	void AddVertex();
	
	// Find a repeated edge in the graph.
	bool FindRepeatedEdge(int& u, int& v, int& w, int& label) const;
	
	// Return true if there is an edge in u with specified label, and the
	// neighbour through this edge.
	bool HasEdge(int u, int label, int& v) const;
	bool HasExactEdge(int u, int label, int v) const;

	// Shortest path from every node to the root
	void AllShortestPaths(std::vector<Edge>& prev, std::vector<int>& dist) const;

	void ComputeSpanningTree(Graph& st, std::vector<std::tuple<int, int, int>>& not_used) const;

	void ComputeQuotient(Graph& qt, const std::vector<int>& relation) const;

	bool IsIsomorphic(const Graph& g) const;

	// For each label, a list of the edges with that label (label > 0).
	std::vector<std::vector<std::pair<int, int>>> ListEdgesByLabel() const;
	
	// Prints the graph.
	void Show() const;

	Adj& operator[](int idx) {
		return list[idx];
	}

	const Adj& const_list(int idx) const {
		return list[idx];
	}
	
	// Swap two graphs.
	static void Swap(Graph& g1, Graph& g2);

	static Graph PullBack(const Graph& gH, const Graph& gK);

 private:
	int num_vertex;
	int max_label;
	AdjList list;
};

}  // namespace stallings

std::ostream& operator<<(std::ostream& out, const stallings::Path& path);

#endif
