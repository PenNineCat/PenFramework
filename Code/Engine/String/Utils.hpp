// File /Engine/String/Utils.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Utils/Concept.hpp"
#include "../Common/Type.hpp"
#include <boost/locale/encoding.hpp>
#include <string>

namespace PenFramework::PenEngine
{
	template <typename T>
	concept IsStdCharType = IsOneOf<T, cch, wch, ch8, ch16, ch32>;

	template <typename T>
	concept CanConvertToU8CharType = IsStdCharType<T> && (sizeof(T) == sizeof(ch8));

	template <typename T>
	concept CanConvertToU16CharType = IsStdCharType<T> && (sizeof(T) == sizeof(ch16));

	template <typename T>
	concept CanConvertToU32CharType = IsStdCharType<T> && (sizeof(T) == sizeof(ch32));

	template <IsStdCharType In, IsStdCharType Out>
	std::basic_string<Out> StringConvert(std::basic_string_view<In> view)
	{
		return boost::locale::conv::utf_to_utf<Out>(view.data(), view.data() + view.size());
	}

	template <IsStdCharType T>
	void UtfToUtf8(const T* from, cch* to, usize charLength)
	{
		if constexpr (CanConvertToU8CharType<T>)
		{
			MemoryCopy(ReinterpretPointerCast<cch>(from), to, charLength);
		}
		else if constexpr(CanConvertToU16CharType<T>)
		{

		}
		else
		{

		}
	}

	template <IsStdCharType T>
	void UtfToUtf8(T from, cch* to, usize charCount)
	{
		if constexpr (CanConvertToU8CharType<T>)
		{
			std::fill_n(to,charCount,static_cast<cch>(from));
		}
		else if constexpr (CanConvertToU16CharType<T>)
		{

		}
		else
		{

		}
	}
}
