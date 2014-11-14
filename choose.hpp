// choose.hpp
//
// Methods for choosing, perhaps uniformly at random, an element
// satisfying a given predicate. Provides an inplace method based
// on reservoir sampling, see e.g.
//
//   Vitter, Jeffrey S. "Random sampling with a reservoir." 
//   ACM Transactions on Mathematical Software (TOMS) 11.1 (1985): 37-57.

#ifndef CHOOSE_HPP
#define CHOOSE_HPP

#include <iterator>
#include <random>

template <typename Iterator, typename RandomNumberGenerator>
Iterator choose(Iterator begin, Iterator end, RandomNumberGenerator& rng)
{
	Iterator result = begin;
	std::advance(result, rng());
	return result;
}

template <typename Iterator, typename UnaryPredicate>
Iterator choose_if(Iterator begin, Iterator end, UnaryPredicate p)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution sampler;

	std::size_t count = 0;
	Iterator choice = end;

	for (Iterator it = begin; it != end; ++it)
	{
		if (p(*it))
		{
			++count;
			sampler.param(1.0 / count);

			if (sampler(gen))
				choice = it;
		}
	}

	return choice;
}

#endif
