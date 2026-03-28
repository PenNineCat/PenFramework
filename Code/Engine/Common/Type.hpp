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

#include <cstddef>
#include <cstdint>

namespace PenFramework::PenEngine
{
	using B8 = uint8_t;
	using U8 = uint8_t;
	using U16 = uint16_t;
	using U32 = uint32_t;
	using u64 = uint64_t;
	using I8 = int8_t;
	using I16 = int16_t;
	using I32 = int32_t;
	using I64 = int64_t;
	using Usize = size_t;
	using Isize = ptrdiff_t;
	using Ch = char;
	using Wch = wchar_t;
	using Ch8 = char8_t;
	using Ch16 = char16_t;
	using Ch32 = char32_t;
	using F32 = float;
	using F64 = double;

	static constexpr Usize BitsPerBytes = 8;
}