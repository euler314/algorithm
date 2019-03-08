// choose.hpp
//
// Methods for choosing, perhaps uniformly at random, an element
// satisfying a given predicate. Provides an inplace method based
// on reservoir sampling, see e.g.
//
//   Vitter, Jeffrey S. "Random sampling with a reservoir." 
//   ACM Transactions on Mathematical Software (TOMS) 11.1 (1985): 37-57.
//
// Usage example:
// auto it = choose_if(v.cbegin(), v.cend(), [](int v) { return v % 2 == 0; }, std::mt19937{ std::random_device{}() });

#ifndef CHOOSE_HPP
#define CHOOSE_HPP

#include <iterator>
#include <random>

template <typename InputIterator, typename UnaryPredicate, typename URBG>
std::pair<InputIterator, std::size_t> 
choose_if(InputIterator begin, InputIterator end, UnaryPredicate p, URBG&& g)
{
	std::bernoulli_distribution sampler;
	using param_type = std::bernoulli_distribution::param_type;

	std::size_t count = 0;
	InputIterator choice = end;

	for (; begin != end; ++begin)
	{
		if (p(*begin))
		{
			++count;
			sampler.param(static_cast<param_type>(1.0 / count));

			if (sampler(g))
				choice = begin;
		}
	}

	return std::make_pair(choice, count);
}

#endif
