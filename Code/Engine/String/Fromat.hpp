// File /Engine/String/Format
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Common/Iterator.hpp"
#include "String.h"
#include <format>

namespace PenFramework::PenEngine
{

	template <int = 0>
	String VFormat(std::string_view fmt, std::format_args args)
	{
		String str;
		str.Reserve(fmt.size() + args._Estimate_required_capacity());
		std::vformat_to(BackInserter(str), fmt, args);
		return str;
	}

	template <typename... Args>
	String Format(std::format_string<Args...> fmt, Args&&... args)
	{
		return VFormat(fmt.get(), std::make_format_args(args...));
	}

	// 目前对于wstring，是采取直接构建wstring，再转为String的两次构造，
	// 还是尝试边编码边构造并不明确，目前先注释掉
	//template <typename... Args>
	//String Fromat(std::wformat_string<Args...> fmt, Args&&... args);

	// STL format目前不支持utf32，是采取直接构造std::string，再转为String的两次构造，
	// 还是尝试边编码边构造并不明确，目前先注释掉
	//template <typename... Args>
	//String/U32String Format(std::u32format_string<Args...> fmt,Args&&... args);
}
