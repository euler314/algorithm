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

namespace detail
{
    template <typename T, typename... Ts>
    std::size_t size_helper(const T& first, const Ts&...)
    {
        return first.size();
    }
}

template <typename... Ts>
void multi_apply_permutation(std::vector<std::size_t>& perm, Ts&... ranges)
{
    const std::size_t size = detail::size_helper(ranges...);

    for (std::size_t i = 0; i < size; ++i)
    {
        std::size_t current = i;
        auto permIt = perm.begin();

        while (i != permIt[current])
        {
            const std::size_t next = permIt[current];

            ((void)std::swap(ranges[current], ranges[next]), ...);

            permIt[current] = current;
            current = next;
        }
        permIt[current] = current;
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
