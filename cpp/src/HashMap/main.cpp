//#include "OneWayHashSet.hpp"


// The current strategy for a fast text Markov chain implementation
// for word, next_word in text
	//if word not in bloom filter: // this might be slower with my level of text, but fuck it, I want a bloom filter
	//	insert_no_exists into hash_table // take the hash, find the nearest index, and insert
	//	update_word_next next_word index // use that index to update the next_word_list and count
	//else:
	//	if word in cache: // this is a simple array of indexes of the most common words
	//		update_word_next next_word index
	//		update_count word index
	//	else:
	//		insert_maybe_exists hash_table word
	//		update_coutn word index
	//		update_word_next next_word index 
// NOTE: This will probably be better if I update the word_next part after I get the index of the second word. 
// for word in word:
//   insert_and_get_index word
//   update_previous_next_word word

#include <iostream>

#include <array>
#include <bitset>
#include <cstddef>
#include <functional>
#include <memory>

// TODO: https://english.stackexchange.com/questions/395078/an-adjective-describing-a-collection-that-can-be-added-to-but-not-removed-from

// TODO: I really think I want to call this a HashSet and use a separate array for the values

// This class offers a linear probing hash map that can only be added to (I'm implementing a Markov Chain and I'm not going to need deletion)
// It's fixed size and cannot have keys deleted from it, which means that You can get an index from an operation and that index is stable.
template<
	class Type,
	size_t MaxSize,
	class Hash = std::hash<Type>,
	class Equal = std::equal_to<Type>
	// TODO: class Allocator
> struct OneWayHashSet
{
	// Just in case I want to make this a parameter
	using SizeType = std::size_t;

	std::array<Type, MaxSize> data;
	SizeType current_size;
	std::bitset<MaxSize> initialized_data; // always initializes to all 0's
	Hash Hasher{};

	OneWayHashSet() : current_size(0)
	{

	}
	inline auto max_size() const { return MaxSize; }
	inline auto empty() const { return current_size == 0; }
	inline auto full() const { return current_size == size(); }
	SizeType insert(Type value)
	{
		return 0;
	}
};

int main()
{
	OneWayHashSet<int, 5> hm;
	std::cout << hm.max_size() << std::endl;
	auto Hasher = std::hash<int>();
	std::cout << Hasher(5) << std::endl;
	std::cout << Hasher(5) << std::endl;
	std::cout << std::hash<int>()(5) << std::endl;
}