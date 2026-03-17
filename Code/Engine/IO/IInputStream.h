// File /Engine/IO/IStream.h
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

namespace PenFramework::PenEngine
{
	class IInputStream
	{
	public:
		virtual ~IInputStream() noexcept = default;
		virtual void PrepareBuffer(Usize requiredLen) = 0;
		virtual void WriteBuffer(B8* data, Usize actualLen) = 0;
	};
}