// File /Engine/DebugTools/Verify.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once

#ifdef _DEBUG
#define DEBUG_VERIFY_REPORT(cond,message) \
	if(!(cond)) \
	{ \
	 	(void) ((1 != _CrtDbgReport(_CRT_ASSERT,__FILE__,__LINE__,nullptr,"%s",message)) || (_CrtDbgBreak(), 0)); \
		__fastfail(5); \
		std::unreachable(); \
	} 

#else
#define DEBUG_VERIFY_REPORT(cond,message)
#endif // _DEBUG

namespace PenFramework::PenEngine
{

}
