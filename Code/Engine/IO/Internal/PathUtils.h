// File /Engine/IO/Internal/PathUtils.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

// This is an internally dependent file.

#pragma once

#include "../../Common/Type.hpp"

namespace PenFramework::PenEngine::Internal
{
	const Ch* ForwardTrySkipRootName(const Ch* start, const Ch* end) noexcept;
	const Ch* ForwardTrySkipDriveRoot(const Ch* start, const Ch* end) noexcept;
	const Ch* ForwardTrySkipNetworkRoot(const Ch* start, const Ch* end) noexcept;
		
	// @brief 尝试跳过start到end区间的名称
	// @retval nullptr start为空，或者==end，或者以'/'开始
	// @retval ptr 返回区间内的下一个'/'所在位置指针
	const Ch* ForwardTrySkipName(const Ch* start, const Ch* end) noexcept;
	const Ch* ForwardTrySkipAllSeparator(const Ch* start, const Ch* end) noexcept;
	const Ch* ForwardTrySkipCountedSeparator(const Ch* start, const Ch* end, Usize count) noexcept;

	const Ch* BackwardTrySkipRootName(const Ch* start, const Ch* end) noexcept;
	const Ch* BackwardTrySkipDriveRoot(const Ch* start, const Ch* end) noexcept;
	const Ch* BackwardTrySkipNetworkRoot(const Ch* start, const Ch* end) noexcept;

	// @brief 尝试跳过start到end区间的名称
	// @retval nullptr start为空，或者==end，或者以'/'开始
	// @retval ptr 返回区间内的下一个'/'所在位置指针
	const Ch* BackwardTrySkipName(const Ch* start, const Ch* end) noexcept;
	const Ch* BackwardTrySkipAllSeparator(const Ch* start, const Ch* end) noexcept;
	const Ch* BackwardTrySkipCountedSeparator(const Ch* start, const Ch* end, Usize count) noexcept;

	bool IsValidSeparator(Ch ch) noexcept;
	bool IsValidDriveLetter(Ch ch) noexcept;
}