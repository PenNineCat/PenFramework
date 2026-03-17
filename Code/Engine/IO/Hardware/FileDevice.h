// File /Engine/IO/Hardware/FileDevice.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../IIODevice.h"

namespace PenFramework::PenEngine
{
	class FileDevice : public IIODevice
	{
	public:
		

		virtual bool Read(IInputStream& stream);
		virtual bool Write(IOutputStream& stream);
	private:
	};
}