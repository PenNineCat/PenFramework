// File /Engine/String/StringUnorderedMap
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "StringTransparentHash.hpp"
#include <unordered_map>

namespace PenFramework::PenEngine
{
	template <typename V>
	using StringUnorderedMap = std::unordered_map<std::string, V, StringTransparentHash, std::equal_to<>>;
	template <typename V>
	using StringUnorderedMultimap = std::unordered_multimap<std::string, V, StringTransparentHash, std::equal_to<>>;
}
