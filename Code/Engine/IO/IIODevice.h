// File /Engine/IO/IIODevice.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "IInputStream.h"
#include "IOutputStream.h"

namespace PenFramework::PenEngine
{
	enum class IODeviceMode : U8
	{
		WriteOnly,
		ReadOnly,

		WriteAndRead,

		Binary,
		Text,
	};
	class IIODevice
	{
	public:
		virtual ~IIODevice() noexcept = default;

		virtual bool Read(IInputStream& stream) = 0;
		virtual bool Write(IOutputStream& stream) = 0;
	};
}