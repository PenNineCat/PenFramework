// File /Engine/Exception/InvalidArgument.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "Exception.hpp"

namespace PenFramework::PenEngine
{
	class InvalidArgument : public Exception
	{
	public:
		InvalidArgument(std::string_view source, std::string_view operation, std::string_view detail) : Exception("InvalidArgument", source, operation, detail) {}
	};
}