#ifndef SUBGROUP_HPP
#define SUBGROUP_HPP

#include <vector>
#include <map>
#include <iostream>

#include <graph.hpp>
#include <folding.hpp>

namespace stallings {

typedef std::vector<int> Element;

class Subgroup {
 public:
	Subgroup(); //Empty subgroup
	explicit Subgroup(const std::vector<Element>& base_);
	explicit Subgroup(const Graph& graph);

	// Print the Stallings Graph.
	void ShowGraph() const {
		stallings_graph.Show();
	}
	
	void ShowFoldings() const;
	void ShowStallingsGraph() const;
	
	// Show the subgroup base. It doesn't check if the elements in the
	// base are independent.
	void ShowBase() const;
	
	int GetBaseSize() const;
	Element GetBaseElement(int idx) const;

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

	Path GetPath(const Element& element) const;
	std::vector<int> GetCoordinates(const Element& element) const;

	static Element Inverse(const Element& element);
	static Element Product(const Element& a, const Element& b);
	static Subgroup Intersection(const Subgroup& H, const Subgroup& K);
	
 private:
	std::vector<Element> base;
	std::vector<Folding> foldings;
	Graph stallings_graph;
	std::map<Edge, int> coordinates;
	
	bool has_base;
	bool is_folded;
};

}  // namespace stallings

std::ostream& operator<<(std::ostream& out, const stallings::Element& element);
std::istream& operator>>(std::istream& in,  stallings::Element& element);

#endif
