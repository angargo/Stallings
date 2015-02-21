#ifndef FOLDING_HPP
#define FOLDING_HPP

#include <graph.hpp>

namespace stallings {

// Stores a graph and the folding applied to that graph.
// v <--(label)-- u --(label)--> w
// If v < w. Nodes with index >= w are displaced, otherwise v = w.

class Folding {
 public:
	Folding() : graph(), u(0), v(0), w(0), label(0) {}

	void Show() const;

	Path RaisePath(const Path& path) const;

	Graph graph;

	// Edges u-v and u-w both have the same label.
	int u, v, w, label;
};

}  // namespace stallings

#endif // FOLDING_HPP
