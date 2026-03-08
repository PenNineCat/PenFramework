// File /Engine/String/StringTransparentHash
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Utils/TransparentHash.hpp"
#include "String.hpp"

namespace PenFramework::PenEngine
{
	template <typename CharType>
	using StringClassTransparentHash = TransparentHash<BasicString<CharType>, BasicStringView<CharType>, std::basic_string<CharType>, std::basic_string_view<CharType>>;

	template <typename CharType>
	struct StringTransparentHash : StringClassTransparentHash<CharType>
	{
		using StringClassTransparentHash::operator();

		static usize operator()(const CharType* ptr) noexcept
		{
			return std::hash<BasicStringView<CharType>>(BasicStringView<CharType>(ptr));
		}
	};
}