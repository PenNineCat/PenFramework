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

namespace PenFramework::PenEngine
{
	class MemoryAllocator
	{
	public:
		template <typename T>
		static constexpr T* Allocate(usize bytes);

		template <typename T>
		static constexpr void Free(T* block,usize bytes) noexcept;
	private:
	};

	template <typename T>
	constexpr T* MemoryAllocator::Allocate(usize bytes)
	{
		return static_cast<T*>(std::allocator<T>::allocate);
	}

	template <typename T>
	constexpr void MemoryAllocator::Free(T* block, usize bytes) noexcept
	{
		std::_Deallocate<std::max(alignof(T),16ull)>(block, bytes);
	}
}
