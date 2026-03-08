// File /Engine/Utils/Singleton.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <type_traits>

namespace PenFramework::PenEngine
{
	template <typename T>
	class Singleton
	{
		friend T;
	public:
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;

		virtual ~Singleton() noexcept = default;

		static T& GetInstance() noexcept(std::is_nothrow_default_constructible_v<T>)
		{
			static T instance;
			return instance;
		}
	private:
		Singleton() noexcept = default;
	};
}