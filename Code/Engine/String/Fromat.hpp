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
#include "String.hpp"
#include <format>

namespace PenFramework::PenEngine
{
	template <int = 0>
	String VFormat(std::string_view fmt, std::format_args args)
	{
		String str;

		#ifdef _MSC_VER
		// MSVC STL可以通过这个api计算出按照这个参数列表需要分配的估计大小，但是libcxx我并没有找到类似的api
		str.Reserve(fmt.size() + args._Estimate_required_capacity()); 
		#endif // _MSC_VER

		std::vformat_to(BackInserter(str), fmt, args);
		return str;
	}

	template <typename... Args>
	String Format(std::format_string<Args...> fmt, Args&&... args)
	{
		return VFormat(fmt.get(), std::make_format_args(args...));
	}
}
