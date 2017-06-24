#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <memory>

// TODO: https://english.stackexchange.com/questions/395078/an-adjective-describing-a-collection-that-can-be-added-to-but-not-removed-from

// This class offers a linear probing hash map that can only be added to (I'm implementing a Markov Chain and I'm not going to need deletion)
template<
	class Type,
	std::size_t MaxSize,
	class Hash=std::hash<Type>,
	class Equal=std::equal_to<Type>
	// TODO: class Allocator
> struct OneWayHashSet
{
	//std::size_t MaxSize;
	std::array<Type, MaxSize> Data;
	OneWayHashSet() = default;
	inline auto size() const { return MaxSize; }
};