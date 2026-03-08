// File /Engine/Utils/Concept.hpp
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
	template <typename T, typename... Ts>
	concept IsOneOf = (std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Ts>> || ...);

	template <typename From, typename To>
	concept IsStaticCastable = requires(From && t) { static_cast<To>(std::forward<From>(t)); };

	template <typename T>
	concept IsPointer = std::is_pointer_v<T>;

	template <typename T>
	concept IsPointerToObject = IsPointer<T> || std::is_object_v<std::remove_pointer_t<T>>;

	template <typename T>
	concept IsReferenceable = !std::is_void_v<std::add_lvalue_reference_t<T>>;

	template <typename T>
	concept IsDereferenceable = requires(T & t)
	{
		{ *t } -> IsReferenceable;
	};
}
