#pragma once

#include "../Utils/TransparentHash.hpp"
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace PenFramework::PenEngine
{
	using StringTransparentHash = TransparentHash<std::string_view, std::hash<std::string_view>, std::string, char*, std::string_view>;

	template <typename V>
	using StringUnorderedMap = std::unordered_map<std::string, V, StringTransparentHash,std::equal_to<>>;
	template <typename V>
	using StringUnorderedMultimap = std::unordered_multimap<std::string, V, StringTransparentHash, std::equal_to<>>;
	using StringUnorderedSet = std::unordered_set<std::string, StringTransparentHash, std::equal_to<>, std::equal_to<>>;
	using StringUnorderedMultiset = std::unordered_multiset<std::string, StringTransparentHash, std::equal_to<>, std::equal_to<>>;
	template <typename V>
	using StringMap = std::map<std::string, V, std::less<>>;
	using StringSet = std::set<std::string, std::less<>>;
	template <typename V>
	using StringMultimap = std::multimap<std::string,V,std::less<>>;
}