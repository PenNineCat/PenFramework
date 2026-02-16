// File /Engine/Memory/Utils.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Common/Type.hpp"
#include <type_traits>

namespace PenFramework::PenEngine
{
	template <typename T> requires std::is_trivially_copyable_v<T>
	constexpr T* MemoryCopy(const T* from, T* to, usize bytes) noexcept
	{
		if consteval
		{
			for (usize i = 0; i < bytes; ++i)
			{
				to[i] = from[i];
			}
			return to;
		}

		return static_cast<T*>(memcpy(to, from, bytes));
	}
}
