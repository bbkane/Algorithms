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

// New simplified algorithm:
// Data is a array<struct<vector
// for word in text:
//   index = hs.insert(word)
//   data[prev_index].count++;
//   data[prev_index].next_words.append_once index and increase count
//   

#include <iostream>

#include <array>
#include <bitset>
#include <cstddef>
#include <functional>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

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
	// This is also the output of Hash, so I'm not sure how flexible it really is
	using SizeType = std::size_t;

	std::array<Type, MaxSize> data = { 0 };
	SizeType current_size{ 0 };
	std::bitset<MaxSize> initialized_data; // always initializes to all 0's
	Hash Hasher{};
	Equal Equaler{}; // TODO: right name?

	OneWayHashSet() = default;
	inline auto size() const { return current_size; }
	inline auto max_size() const { return MaxSize; }
	inline auto empty() const { return current_size == 0; }
	inline auto full() const { return current_size == max_size(); }
	// inserts and returns the index inserted at
	// TODO: Don't assume that the hash table is empty. Throw?
	// TODO: Move semantics?
	// this is a set, so it might not insert if the element already exists
	// TODO: keep an array of cached hashes in case Equal is too expensive? (probably better in the general case but worse in my case)
	SizeType insert(Type value)
	{
		if (full())
		{
			throw std::out_of_range("table full");
		}
		SizeType hash_value = Hasher(value);
		hash_value %= max_size();
		std::cout << "Hash( " << value << " ) = " << hash_value << std::endl;
		while (initialized_data[hash_value])
		{
			if (Equaler(data[hash_value], value))
			{
				return hash_value;
			}
			hash_value = (hash_value + 1) % size();
		}
		data[hash_value] = value;
		initialized_data[hash_value] = true;
		++current_size;
		return hash_value;
	}

	// How to exit on error here? I think I'm going to use an exception...
	// Though I could return max_size()
	// I don't even think I need to have this for the Markov Chain
	SizeType find(Type value)
	{
		SizeType hash_value = Hasher(value);
		hash_value %= max_size();
		// TODO: finish
		return 0;
	}

	// Note: https://stackoverflow.com/questions/4660123/overloading-friend-operator-for-template-class
	friend std::ostream& operator<<(std::ostream& o, const OneWayHashSet& hs)
	{
		o << "Max size: " << hs.max_size() << "\n";
		o << "Current size: " << hs.size() << "\n";
		o << "Index - Data - Initialized Data: " << "\n";
		// TODO: get some indexes here, some ellipses when there's long strings of zeros...
		for (SizeType i = 0; i < hs.max_size(); ++i)
		{
			o << "  " << i << "  " << hs.data[i] << "  " << hs.initialized_data[i] << "\n";
		}

		return o;
	}
};

// this is the data attached to the next index
struct NextData
{
	int count = 0;
	size_t index;
	NextData(size_t index, int count) : index(index), count(count) {}
};

// This is the data attached to the first index
struct PrevData
{
	int count = 0;
	std::vector<NextData> nexts; // I'm just going to linearly search this guy, though I guess I coudl make him a hashset as well
	PrevData() = default;
};

int main()
{
	std::ifstream fin(R"(C:\Users\Ben\Desktop\data.txt)");
	std::string word;
	constexpr size_t max_size = 1 << 5;
	OneWayHashSet<std::string, max_size> hs;
	std::array<PrevData, max_size> prev_data;
	for (size_t i = 0; i < prev_data.size(); ++i)
	{
		prev_data[i] = PrevData(); // It's initialized now
	}

	fin >> word;
	auto prev_index = hs.insert(word);
	while (fin >> word)
	{
		
	}
}