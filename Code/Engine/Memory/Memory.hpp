// File /Engine/Memory/MemoryAllocator.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once

#include "../Common/Type.hpp"

namespace PenFramework::PenEngine::Memory
{
	template <typename T>
	static T* Allocate(usize count)
	{
		return operator new(count * sizeof(T), alignof(T));
	}

	template <typename T>
	static void Deallocate(T* buffer, usize count) noexcept
	{
		operator delete(buffer, count, alignof(T));
	}

	template <typename T, typename... Args>
	static void Construct(T* position, Args&&... args)
	{
		new (position) T(std::forward<Args>(args)...);
	}

	template <typename T>
	static void DestroyForRange(T* begin, T* end)
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			T* it = begin;

			while (it != end)
			{
				it->~T();
				++it;
			}
		}
	}
}
