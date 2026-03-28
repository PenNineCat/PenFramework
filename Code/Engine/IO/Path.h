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
#include <filesystem>

namespace PenFramework::PenEngine
{
	class Path
	{
	public:
		using value_type = Ch;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = Usize;
		using difference_type = Isize;

		// @todo 长路径支持需要内部使用平台特化的separator，并且对于size >= MAX_PATH的路径自动加上\\?\ 目前不打算实现这一部分内容
		constexpr static Ch PreferredSeparator =
		{
			#ifdef LONG_PATH_SUPPORT 
			// 如果不启用长路径支持，windows api可以识别以'/'为分隔符的路径
			#ifdef _WIN32
			'\\'
			#else // _WIN32
			'/'
			#endif // _WIN32

			#else // LONG_PATH_SUPPORT
			'/'
			#endif // LONG_PATH_SUPPORT 
		};

		Path() noexcept = default;

		Path(const Path&) =default;
		Path(Path&&) noexcept = default;
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

		template <typename... T>
		static Path BuildRawPath(T... view);

		Path operator/(const Path& path) const;
		Path operator/(const String& path) const;
		Path operator/(StringView path) const;
		Path operator/(const Ch* path) const;
		Path operator/(const std::string& path) const;
		Path operator/(std::string_view path) const;

		Path& operator/=(const Path& path);
		Path& operator/=(const String& path);
		Path& operator/=(StringView path);
		Path& operator/=(const Ch* path);
		Path& operator/=(const std::string& path);
		Path& operator/=(std::string_view path);

		bool IsAbsolute() const noexcept;
		bool IsRelative() const noexcept;

		bool Empty() const noexcept;

		Path RootName() const;
		Path RootPath() const;
		Path RootDirectory() const;
		Path Filename() const;
		Path Extension() const;
		Path Stem() const;
		Path ParentPath() const;
		Path RelativePath() const;

		template <typename CharType>
		BasicString<CharType> ToString() const;
		template <typename CharType>
		std::basic_string<CharType> ToStdString() const;
		const Ch* Data() const noexcept;

		StringView ToView() const;

		bool HasRootName() const noexcept;
		bool HasRootPath() const noexcept;
		bool HasRootDirectory() const noexcept;
		bool HasRelativePath() const noexcept;
		bool HasParentPath() const noexcept;
		bool HasFilename() const noexcept;
		bool HasStem() const noexcept;
		bool HasExtension() const noexcept;
	private:
		StringView InternalGetRootName() const noexcept;
		StringView InternalGetRootDirectory() const noexcept;
		StringView InternalGetFilename() const noexcept;
		StringView InternalGetRelativePath() const noexcept;
		StringView InternalGetParentPath() const noexcept;

		static String Normalize(StringView path);

		String m_path;
	};

	template <typename ... T>
	Path Path::BuildRawPath(T... view)
	{
		Path res;
		(res.m_path += ... += view);
		return res;
	}

	template <typename CharType>
	BasicString<CharType> Path::ToString() const
	{
		return m_path.ConvertTo<CharType>();
	}

	template <typename CharType>
	std::basic_string<CharType> Path::ToStdString() const
	{
		return boost::locale::conv::utf_to_utf<CharType>(m_path.Data(), m_path.Data() + m_path.Size());
	}
}

template <>
struct std::hash<PenFramework::PenEngine::Path>
{
	static PenFramework::PenEngine::Usize operator()(const PenFramework::PenEngine::Path& path) noexcept
	{
		return std::hash<PenFramework::PenEngine::StringView>::operator()(path.ToView());
	}
};

template<>
struct std::formatter<PenFramework::PenEngine::Path> : std::formatter<std::string>
{
	static auto format(const PenFramework::PenEngine::Path& path, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{}", path.Data());
	}
};