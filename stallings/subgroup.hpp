#ifndef SUBGROUP_HPP
#define SUBGROUP_HPP

#include "graph.hpp"
#include <vector>
#include <iostream>

namespace stallings {

typedef std::vector<short> Element;

class Folding {
 public:
	Folding() : graph(), u(0), v(0), w(0), label(0) {}
	void Show() const;
	
	Graph graph;
	
	// Edges u-v and u-w both have the same label.
	int u, v, w, label;
};

class Subgroup {
 public:
	explicit Subgroup(const std::vector<Element>& base_);
	// Print the Stallings Graph.
	void ShowGraph() const {
		stallings_graph.Show();
	}
	
	void ShowFoldings() const;
	void ShowStallingsGraph() const;
	
	// Show the subgroup base. It doesn't check if the elements in the
	// base are independent.
	void ShowBase() const;
	
	// Add element as a 'petal' to graph.
	static void AddElement(const Element& element, Graph& graph);
	
	
	bool IsFolded() const {
		return is_folded;
	}
	
	// Make foldings until the graph is folded.
	void Fold();
	
	// Find a duplicate edge, return true if found.
	bool FindFolding(Folding& fold) const;
	
	// Perform a folding.
	void DoFolding(Folding& fold);
	
	// Return true if element is a member of the subgroup.
	bool Contains(const Element& element) const;
	
 private:
	std::vector<Element> base;
	std::vector<Folding> foldings;
	Graph stallings_graph;
	
	bool has_base;
	bool is_folded;
};

}  // namespace stallings

std::ostream& operator<<(std::ostream& out, const stallings::Element& element);
std::istream& operator>>(std::istream& in,  stallings::Element& element);

#endif