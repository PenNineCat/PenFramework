// File /Engine/String/String.cpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "String.h"

namespace PenFramework::PenEngine
{
	String String::Substr(usize off, usize len) const
	{
		usize size = Size();

		if (off >= size)
			return {};

		len = std::min(len, size - off);

		return String(Buffer() + off, len);
	}

	String String::Right(usize len) const
	{
		return Substr(Size() - len, len);
	}

	String String::Left(usize len) const
	{
		return Substr(0, len);
	}
}