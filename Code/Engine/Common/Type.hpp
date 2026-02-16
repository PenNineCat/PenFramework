// File /Engine/Common/Type.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <__msvc_int128.hpp>
#include <cstdint>

namespace PenFramework::PenEngine
{
	/// @brief std::byte
	using b8e = std::byte;
	using b8 = uint8_t;
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	/// @brief struct _Unsigned128
	using u128c = std::_Unsigned128;
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	/// @brief struct _Signed128
	using i128c = std::_Signed128;
	using usize = size_t;
	using cch = char;
	using wch = wchar_t;
	using ch8 = char8_t;
	using ch16 = char16_t;
	using ch32 = char32_t;
	using f32 = float;
	using f64 = double;
}