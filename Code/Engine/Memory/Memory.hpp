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

#include <atomic>

#include "../Common/Type.hpp"

namespace PenFramework::PenEngine::Memory
{
	static std::atomic<usize> g_currentMemoryAllocatedBytes;

	template <typename T>
	static T* Allocate(usize count)
	{
		#ifdef _DEBUG
		g_currentMemoryAllocatedBytes.fetch_add(count * sizeof(T), std::memory_order::relaxed);
		#endif // _DEBUG

		return static_cast<T*>(operator new(count * sizeof(T), std::align_val_t{alignof(T)}));
	}

	template <typename T>
	static void Deallocate(T* buffer, usize count) noexcept
	{
		#ifdef _DEBUG
		g_currentMemoryAllocatedBytes.fetch_sub(count * sizeof(T), std::memory_order::relaxed);
		#endif // _DEBUG

		operator delete(buffer, count * sizeof(T), std::align_val_t{alignof(T)});
	}

	template <typename T, typename... Args>
	static void Construct(T* position, Args&&... args)
	{
		new (position) T(std::forward<Args>(args)...);
	}

	template <typename T>
	static void Destroy(T* object)
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
			object->~T();
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
