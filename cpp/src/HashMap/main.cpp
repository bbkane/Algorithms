//#include "OneWayHashSet.hpp"

// So I think I got this basically working and then Windows limits the stack size.
// To overcome this, I'm going to convert std::arrays to std::vectors, call reserve, and carry on.
// See https://software.intel.com/en-us/articles/memory-limits-applications-windows
// I also need to use a x64 build. See https://stackoverflow.com/a/28370892/2958070

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

	//std::array<Type, MaxSize> data;
	std::vector<Type> data;
	SizeType current_size{ 0 };
	std::bitset<MaxSize> initialized_data; // always initializes to all 0's
	Hash Hasher{};
	Equal Equaler{}; // TODO: right name?

	OneWayHashSet()
	{
		data.resize(max_size()); // This resizes the internal array and fills it with objects in their default state.
	}

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
		//std::cout << "Hash( " << value << " ) = " << hash_value << std::endl;
		while (initialized_data[hash_value])
		{
			if (Equaler(data[hash_value], value))
			{
				return hash_value;
			}
			hash_value = (hash_value + 1) % max_size();
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


// This is the data attached to the first index
struct Data
{
	// this is the data attached to the next index
	struct NextData
	{
		int count = 0;
		size_t index;
		NextData(size_t index, int count) : index(index), count(count) {}
	};

	int count = 0;
	std::vector<NextData> nexts; // I'm just going to linearly search this guy, though I guess I coudl make him a hashset as well
	Data() = default;

	friend std::ostream& operator<<(std::ostream& o, const Data d)
	{
		o << "Count: " << d.count << "\n";
		o << "Nexts: \n";
		for (auto& n : d.nexts)
		{
			o << "  " << "Count: " << n.count << "  Index: " << n.index << "\n";
		}
		return o;
	}
};

template <std::size_t MaxSize>
struct MarkovChain
{
	inline auto max_size() const { return MaxSize; }
	OneWayHashSet<std::string, MaxSize> hs;
	std::vector<Data> datas;

	MarkovChain()
	{
		datas.resize(MaxSize);
	}

	void load_from_text_file(const char* text_file_path)
	{
		std::ifstream fin(text_file_path);
		std::string word;
		try
		{
			fin >> word;
			auto prev_index = hs.insert(word);
			while (fin >> word)
			{
				auto index = hs.insert(word);
				Data& prev_data = datas[prev_index];
				prev_data.count++;

				// TODO: replace this whole search with a set? A HashSet would work but it needs to be growable
				// I should probably test all of this :)
				bool element_in_nexts = false;
				for (Data::NextData& e : prev_data.nexts)
				{
					if (e.index == index)
					{
						e.count++;
						element_in_nexts = true;
						break;
					}
				}
				if (!element_in_nexts)
				{
					prev_data.nexts.push_back(Data::NextData(index, 1));
				}

				// Get the next element ready to be processed
				prev_index = index;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			std::terminate();
		}
	}

	void print_stats()
	{
		std::size_t total_unique_words = 0;
		std::string most_used_word;
		std::size_t most_used_word_count = 0;
		for (std::size_t i = 0; i < hs.max_size(); ++i)
		{

			if (hs.initialized_data[i])
			{
				// std::cout << hs.data[i] << " " << datas[i].count << "\n";
				total_unique_words++;

				if (datas[i].count > most_used_word_count)
				{
					most_used_word = hs.data[i];
					most_used_word_count = datas[i].count;
				}
			}
		}
		std::cout << "Total unique words: " << total_unique_words << "\n";
		std::cout << "Most used word: " << most_used_word << " at " << most_used_word_count << "\n";
		std::cout << "size " << hs.size() << std::endl;
	}

	std::size_t get_next_index(std::size_t current_index)
	{

	}
	// I'd have to serialize nested data- Ewww
	// Maybe get a JSON parser here or SQLite?
	// If I use SQLite the analysis will be slower
	// But I can use SQL... and whatever can talk to SQLite...
	// I'm going to punt it for now


};

int main1()
{

	constexpr size_t max_size = 100'000;
	auto markov_chain = MarkovChain<max_size>();
	markov_chain.load_from_text_file(R"(C:\Users\Ben\Desktop\war_and_peace.txt)");
	markov_chain.print_stats();

	return 0;
}


int main()
{
	main1();
}