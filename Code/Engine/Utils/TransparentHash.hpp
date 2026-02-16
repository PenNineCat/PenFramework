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

	template <typename Bridge, typename Hash = std::hash<Bridge>, typename... From> requires (IsStaticCastable<From, Bridge> && ...)
		struct TransparentHash
	{
		using is_transparent = void;

		template <typename T> requires IsOneOf<T, From...>
		size_t operator()(T&& element) const noexcept(noexcept(std::declval<Hash>()(std::declval<Bridge>())))
		{
			return Hash()(static_cast<Bridge>(std::forward<T>(element)));
		}
	};
}
