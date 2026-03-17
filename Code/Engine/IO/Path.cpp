// File /Engine/IO/Path.cpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "Path.h"

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(const Path& path)
{
	m_path = path.m_path;
	return *this;
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(Path&& path) noexcept
{
	m_path = std::move(path.m_path);
	return *this;
}

PenFramework::PenEngine::Path::Path(const String& path)
{

}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(const Path& path)
{
	StringView rootName = path.InternalGetRootName();
	Usize rootNameSize = rootName.Size();

	if (path.IsAbsolute() || (!rootName.Empty() && rootName != InternalGetRootName()))
	{
		m_path = path.m_path;
		return *this;
	}
}

bool PenFramework::PenEngine::Path::IsAbsolute() const noexcept
{
	StringView rootName = InternalGetRootName();
	StringView rootPath = InternalGetRootDir();

	// 1. 驱动器盘符路径 C:(/\)
	if (rootName.Size() == 2 && rootName[1] == ':')
		return !rootPath.Empty();

	// 2. 无根路径 \foo \foo\bar
	if (rootName.Empty())
		return false;

	// 3. 服务器/Unix Like路径 \\server\share //server/share
	if (rootName.Size() < 3)
		return false;
	if (rootName[0] != '/' && rootName[0] != '\\')
		return false;
	if (rootName[1] != '/' && rootName[1] != '\\')
		return false;

	return true;
}

bool PenFramework::PenEngine::Path::Empty() const noexcept
{
	return m_path.Empty();
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::GetRootName() const
{
	return UncheckedBuildPath(InternalGetRootName());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::GetRootPath() const
{
	return UncheckedBuildPath(InternalGetRootDir());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::GetFilename() const
{
	return UncheckedBuildPath(InternalGetFilename());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::GetExtension() const
{
	StringView filename = InternalGetRootName();

	if (Usize pos = filename.FindLastOf('.'); pos != StringView::NPos)
		return UncheckedBuildPath(filename.Right(pos));

	return {};
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Path::Data() const noexcept
{
	return m_path.Data();
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::UncheckedBuildPath(StringView view)
{
	Path res;
	res.m_path = view;
	return res;
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetRootName() const noexcept
{
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	auto it = analyzer.Search();

	if (it.State == Internal::PathAnalyzer::State::AtRootName)
		return it.View;

	return {};
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetRootDir() const noexcept
{
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	auto it = analyzer.Search();

	if (it.State == Internal::PathAnalyzer::State::AtRootDir)
		return it.View;

	return {};
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetFilename() const noexcept
{
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	auto it = analyzer.Search();
	while (it.State <= Internal::PathAnalyzer::State::AtFilename)
		++it;

	if (it.State == Internal::PathAnalyzer::State::AtEnd)
		return {};

	return it.View;
}