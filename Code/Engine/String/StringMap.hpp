// File /Engine/UtilsContainer/StringMap.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <map>
#include <string>

namespace PenFramework::PenEngine
{
	template <typename V>
	using StringMap = std::map<std::string, V, std::less<>>;
	template <typename V>
	using StringMultimap = std::multimap<std::string,V,std::less<>>;
}
