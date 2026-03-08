// File /Engine/Utils/TransparentHash
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once
#include "Concept.hpp"

namespace PenFramework::PenEngine
{
	template <typename... From>
		struct TransparentHash
	{
		using is_transparent = void;

		template <typename T> requires IsOneOf<T, From...>
		static usize operator()(T&& element) const noexcept(noexcept(std::declval<std::hash<T>>()(std::declval<T>())))
		{
			return std::hash<T>()(std::forward<T>(element));
		}
	};
}
