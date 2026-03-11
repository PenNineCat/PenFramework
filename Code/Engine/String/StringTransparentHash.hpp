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

#include "String.hpp"

namespace PenFramework::PenEngine
{
	template <typename CharType>
	struct StringTransparentHash
	{
		using is_transparent = void;

		static usize operator()(BasicStringView<CharType> str) noexcept
		{
			return std::hash<BasicStringView<CharType>>::operator()(str);
		}

		static usize operator()(const BasicString<CharType>& str) noexcept
		{
			return std::hash<BasicString<CharType>>::operator()(str);
		}

		static usize operator()(const CharType* ptr) noexcept
		{
			return std::hash<BasicStringView<CharType>>::operator()(BasicStringView<CharType>(ptr));
		}
	};
}