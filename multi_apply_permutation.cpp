// multi_apply_permutation.cpp
//
// A sample program (not overly generic) for applying
// a permutation to multiple random access ranges (e.g.,
// vectors). 
//
// The implementation is an in-place Fich-Munro-Poblete
// cycle leader algorithm that runs in \Theta(n) time,
// which is obviously optimal up to constant factors.

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#include <boost/algorithm/apply_permutation.hpp>

namespace detail 
{
	template <typename T, typename... Ts>
	std::size_t size_helper(const T& first, const Ts&...)
	{
		return first.size();
	}
}

template <typename... Ts>
void multi_apply_permutation(std::vector<std::size_t>& perm, Ts&... vecs)
{
	const std::size_t size = detail::size_helper(vecs...);

	for (std::size_t i = 0; i < size; ++i) 
	{
		std::size_t current = i;
		auto ind_begin = perm.begin();

		while (i != ind_begin[current]) 
		{
			const std::size_t next = ind_begin[current];

			// TODO: with C++17, we can simplify this to
			// ((void) std::swap(vecs[current], vecs[next]), ...);
			(void)std::initializer_list<int>{
				((void)std::swap(vecs[current], vecs[next]), 0)...};
			
			ind_begin[current] = current;
			current = next;
		}
		ind_begin[current] = current;
	}
}

int main()
{
	std::vector<int> a{ 1,2,3,4,5,6 };
	std::vector<int> b{ 1,2,3,4,5,6 };
	std::vector<int> c{ 6,5,4,3,2,1 };

	std::vector<std::size_t> perm{ 5,4,3,2,1,0 };

	multi_apply_permutation(perm, a, b, c);

	for (auto e : a) std::cout << e << " "; std::cout << "\n";	
	for (auto e : b) std::cout << e << " "; std::cout << "\n";
	for (auto e : c) std::cout << e << " "; std::cout << "\n";
}
