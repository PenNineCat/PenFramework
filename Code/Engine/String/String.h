// File /Engine/String/String.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "StringStorage.hpp"
#include <format>

namespace PenFramework::PenEngine
{
	class String : public StringStorage<char>
	{
	public:
		using StringStorage::StringStorage;
		using StringStorage::operator=;

		String Substr(usize off = 0, usize len = NPos) const;
		String Right(usize len) const;
		String Left(usize len) const;
	};
}

template <>
struct std::formatter<PenFramework::PenEngine::String>
{
	static auto format(const PenFramework::PenEngine::String& value, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{}", value.Data());
	}
};