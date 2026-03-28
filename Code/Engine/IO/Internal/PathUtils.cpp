// File /Engine/IO/Internal/PathUtils.cpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

// This is an internally dependent file.

#include "PathUtils.h"


const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::ForwardTrySkipRootName(const Ch* start, const Ch* end) noexcept
{
	#ifdef _WIN32
	// Windows环境

	// 先检查是不是本地驱动器标识路径
	if (auto ret = ForwardTrySkipDriveRoot(start, end); ret != nullptr)
		return ret;
	// 再检查是不是网络驱动器标识路径
	if (auto ret = ForwardTrySkipNetworkRoot(start, end); ret != nullptr)
		return ret;

	#endif // _WIN32

	// Unix Like环境下，RootName是不存在的
	return nullptr;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::ForwardTrySkipDriveRoot(const Ch* start, const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	// 示例情况
	// start -> "1:\.."
	// start[0] != 'a'~'z' || 'A'~'Z' 
	// start[1] == ':'
	// ret nullptr

	// start -> "C\..."
	// start[0] == 'a'~'z' || 'A'~'Z' 
	// start[1] != ':'
	// ret nullptr

	// start -> "C:"
	// end == ":"
	// start + 1 == end
	// ret nullptr

	if (start + 1 == end || !IsValidDriveLetter(start[0]) || start[1] != ':')
		return nullptr;

	return start + 2;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::ForwardTrySkipNetworkRoot(const Ch* start, const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	return ForwardTrySkipName(ForwardTrySkipCountedSeparator(start, end, 2), end);
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::ForwardTrySkipName(const Ch* start,
																						 const Ch* end) noexcept
{
	if (start == nullptr || start == end || IsValidSeparator(*start))
		return nullptr;

	start += 1;

	while (start != end && !IsValidSeparator(*start))
		start += 1;

	return start;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::ForwardTrySkipAllSeparator(const Ch* start, const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	if (!IsValidSeparator(*start))
		return nullptr;

	start += 1;

	while (start != end && IsValidSeparator(*start))
		start += 1;

	return start;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::ForwardTrySkipCountedSeparator(const Ch* start,
																									 const Ch* end, Usize count) noexcept
{
	const Ch* ret = ForwardTrySkipAllSeparator(start, end);

	if (ret == nullptr)
		return nullptr;

	if (ret == start + count)
		return ret;

	return nullptr;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::BackwardTrySkipRootName(const Ch* start,
																							  const Ch* end) noexcept
{
	#ifdef _WIN32
	if(const Ch* ret = BackwardTrySkipDriveRoot(start,end))
		return ret;
	if(const Ch* ret = BackwardTrySkipNetworkRoot(start,end))
		return ret;
	#endif // _WIN32

	return nullptr;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::BackwardTrySkipDriveRoot(const Ch* start,
																							   const Ch* end) noexcept
{
	if (start == end)
		return nullptr;

	if (start - 1 == end || !IsValidDriveLetter(*(start - 1)) || start[1] != ':')
		return nullptr;

	return start - 2;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::BackwardTrySkipNetworkRoot(const Ch* start,
																								 const Ch* end) noexcept
{
	if (start == end)
		return nullptr;

	return BackwardTrySkipCountedSeparator(BackwardTrySkipName(start, end), end, 2);
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::BackwardTrySkipName(const Ch* start,
	const Ch* end) noexcept
{
	if (start == nullptr || start == end || IsValidSeparator(*start))
		return nullptr;

	start -= 1;

	while (start != end && !IsValidSeparator(*start))
		start -= 1;

	if (start == end)
	{
		const Ch* ret = BackwardTrySkipRootName(start,end + 1);
		if(ret != nullptr)
			return ret - 1;
	}

	return start;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::BackwardTrySkipAllSeparator(const Ch* start,
                                                                                                  const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	if (!IsValidSeparator(*start))
		return nullptr;

	start -= 1;

	while (start != end && IsValidSeparator(*start))
		start -= 1;

	return start;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::BackwardTrySkipCountedSeparator(const Ch* start,
																									  const Ch* end, Usize count) noexcept
{
	const Ch* ret = BackwardTrySkipAllSeparator(start, end);

	if (ret == nullptr)
		return nullptr;

	if (ret == start - count)
		return ret;

	return nullptr;
}

bool PenFramework::PenEngine::Internal::IsValidSeparator(Ch ch) noexcept
{
	if (ch == '/')
		return true;
	#if _WIN32
	if (ch == '\\')
		return true;
	#endif // _WIN32
	return false;
}

bool PenFramework::PenEngine::Internal::IsValidDriveLetter(Ch ch) noexcept
{
	return (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A');
}
