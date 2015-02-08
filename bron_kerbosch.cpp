// bron_kerbosch.cpp
//
// A fast implementation of the basic version of the Bron-Kerbosch 
// algorithm for listing all maximal cliques (no pivoting, no ordering). 
// Includes a compact and cache efficient representation for graph 
// on at most 64 vertices. 

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdint>
#include <bitset>
#include <intrin.h>

typedef std::uint64_t index_t;

class graph
{
public:
	graph(index_t n) : adj_(n) { }

	graph& operator=(const graph&) = delete;

	~graph() { }

	void add_edge(index_t u, index_t v)
	{
		assert(u >= 0 &&
			v >= 0 &&
			u != v &&
			u < adj_.size() &&
			v < adj_.size());

		adj_[u] |= (1 << v);
		adj_[v] |= (1 << u);
	}

	index_t get_degree(index_t u) const
	{
		return __popcnt64(adj_[u]);
	}

	index_t num_vertices() const
	{
		return adj_.size();
	}

	std::vector<index_t> adj_;
};

void complement(graph& g)
{
	for (index_t i = 0; i < g.adj_.size(); ++i)
	{
		g.adj_[i] = ~g.adj_[i];
		g.adj_[i] ^= (1 << i);
	}
}

bool is_adjacent(const graph& g, int u, int v)
{
	assert(u >= 0 && v >= 0 && u < g.adj_.size() && v < g.adj_.size());
	return g.adj_[u] & (1 << v);
}

template <typename OutputIterator>
void bron_kerbosch_recursive(const graph& g, index_t r, index_t p, index_t x, OutputIterator out)
{
	if (p == 0 && x == 0)
	{
		*out = r;
	}

	for (unsigned long v; p != 0; p &= ~(1 << v))
	{
		_BitScanForward64(&v, p);

		bron_kerbosch_recursive(g, r | (1 << v), p & g.adj_[v], x & g.adj_[v], out);
		p &= ~(1 << v);
		x |= (1 << v);
	}
}

template <typename OutputIterator>
void bron_kerbosch(const graph& g, OutputIterator out)
{
	const index_t p = (std::numeric_limits<index_t>::max() >>
		(std::numeric_limits<index_t>::digits - g.num_vertices()));

	bron_kerbosch_recursive(g, 0, p, 0, out);
}

int main()
{
	static_assert(std::is_unsigned<index_t>::value, "index_t must be unsigned");

	graph g(6);
	g.add_edge(0, 1);
	g.add_edge(1, 2);
	g.add_edge(2, 3);
	g.add_edge(3, 4);
	g.add_edge(4, 5);
	g.add_edge(0, 5);

	for (int i = 0; i < g.num_vertices(); ++i)
	{
		std::cout << i << ", = " << std::bitset<8>(g.adj_[i]).to_string() << "\n";
	}

	std::vector<index_t> cliques;
	bron_kerbosch(g, std::back_inserter(cliques));

	for (auto e : cliques)
		std::cout << std::bitset<8>(e).to_string() << "\n";
}
