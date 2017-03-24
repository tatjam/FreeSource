#pragma once
#include <vector>
#include <map>

//TODO

template<typename T, typename K>

// DONT USE
class simple_multimap 
{
public:

	std::multimap<T, K> mmap;


	void add(T key, K obj)
	{
		mmap.insert(std::pair<T, K>(key, obj));
	}

	std::vector<K> get(T key)
	{
		std::vector<K> out = std::vector<K>();
		std::multimap<T, K>::iterator it;

		std::pair <std::multimap<T, K>::iterator, std::multimap<T, K>::iterator> ret;
		ret = mmap.equal_range(key);
		for (std::multimap<T, K>::iterator it = ret.first; it != ret.second; ++it)
			out.push_back(it->second);

		return out;
	}

	void removeAll(T key)
	{
		mmap.erase(key);
	}

	// Gets you a vector with all the keys (unique)
	std::vector<T> getKeys()
	{
		std::vector<T> ret = std::vector<T>();
		auto end = mmap.end();
		auto it = mmap.begin();
		for (it = mmap.begin(), end = mmap.end(); it != end; it = mmap.upper_bound(it->first))
		{
			ret.push_back(it->first);
		}

		return ret;
	}

	std::vector<K> getValues()
	{
		std::vector<K> ret = std::vector<T>();
		auto end = mmap.end();
		auto it = mmap.begin();
		for (it = mmap.begin(), end = mmap.end(); it != end; it = mmap.upper_bound(it->first))
		{
			ret.push_back(it->second);
		}

		return ret;
	}


	// Gets total unique values in the multimap (maybe it only retuns total keys?
	int keySize()
	{
		int c = 0;
		auto end = mmap.end();
		auto it = mmap.begin();
		for (it = mmap.begin(), end = mmap.end(); it != end; it = mmap.upper_bound(it->first))
		{
			c++;
		}

		return c;
	}

	std::vector<K> operator [](T key)
	{
		return get(key);

	}
};