// File /Engine/IO/Path.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../String/String.hpp"
#include "Internal/PathAnalyzer.h"
#include <filesystem>

namespace PenFramework::PenEngine
{
	class Path
	{
	public:
		constexpr static Ch NativeSeparator() noexcept;

		Path() noexcept = default;

		Path(const Path& path) : m_path(path.m_path) {}
		Path(Path&& path) noexcept : m_path(std::move(path.m_path)) {}

		Path& operator=(const Path& path);
		Path& operator=(Path&& path) noexcept;

		explicit Path(const String& path);
		explicit Path(StringView path);
		explicit Path(const Ch* path);
		explicit Path(const std::filesystem::path& path);

		Path& operator=(const String& path);
		Path& operator=(StringView path);
		Path& operator=(const Ch* path);
		Path& operator=(const std::string& path);
		Path& operator=(std::string_view path);

		Path operator/(Path& path);
		Path operator/(const String& path);
		Path operator/(StringView path);
		Path operator/(const Ch* path);
		Path operator/(const std::string& path);
		Path operator/(std::string_view path);

		Path& operator/=(const Path& path);
		Path& operator/=(const String& path);
		Path& operator/=(StringView path);
		Path& operator/=(const Ch* path);
		Path& operator/=(const std::string& path);
		Path& operator/=(std::string_view path);

		bool IsAbsolute() const noexcept;
		bool IsRelative() const noexcept;

		bool Empty() const noexcept;

		Path GetRootName() const;
		Path GetRootPath() const;
		Path GetFilename() const;
		Path GetExtension() const;

		template <typename CharType>
		BasicString<CharType> ToString() const;
		template <typename CharType>
		std::basic_string<CharType> ToStdString() const;
		const Ch* Data() const noexcept;

		bool HasRootPath() const noexcept;
		bool HasRootDirectory() const noexcept;
		bool HasRelativePath() const noexcept;
		bool HasParentPath() const noexcept;
		bool HasFilename() const noexcept;
		bool HasStem() const noexcept;
		bool HasExtention() const noexcept;
	private:
		static Path UncheckedBuildPath(StringView view);

		StringView InternalGetRootName() const noexcept;
		StringView InternalGetRootDir() const noexcept;
		StringView InternalGetFilename() const noexcept;


		String m_path;
	};

	template <typename CharType>
	BasicString<CharType> Path::ToString() const
	{
		return m_path.ConvertTo<CharType>();
	}

	template <typename CharType>
	std::basic_string<CharType> Path::ToStdString() const
	{
		return boost::locale::conv::utf_to_utf<CharType>(m_path.Data(),m_path.Size());
	}

	constexpr Ch Path::NativeSeparator() noexcept
	{
		#ifdef _WIN32
		return '\\';
		#else
		return '/';
		#endif // _WIN32
	}
}

template<>
struct std::formatter<PenFramework::PenEngine::Path> : std::formatter<std::string>
{
	static auto format(const PenFramework::PenEngine::Path& path, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{}", path.Data());
	}
};