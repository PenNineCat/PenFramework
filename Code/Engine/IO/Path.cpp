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

#include "Internal/PathNormalizer.h"
#include "Internal/PathUtils.h"

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(const Path& path)
{
	if (&path != this)
		m_path = path.m_path;

	return *this;
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(Path&& path) noexcept
{
	if (&path != this)
		m_path = std::move(path.m_path);

	return *this;
}

PenFramework::PenEngine::Path::Path(const String& path) : Path(StringView(path))
{

}

PenFramework::PenEngine::Path::Path(StringView path) : m_path(Normalize(path))
{
}

PenFramework::PenEngine::Path::Path(const Ch* path) : Path(StringView(path))
{
}

PenFramework::PenEngine::Path::Path(const std::filesystem::path& path) : Path(StringView(path.string()))
{
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(const String& path)
{
	return *this = StringView(path);
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(StringView path)
{
	m_path = Normalize(path);
	return *this;
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(const Ch* path)
{
	return *this = StringView(path);
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(const std::string& path)
{
	return *this = StringView(path);
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator=(std::string_view path)
{
	return *this = StringView(path);
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(const Path& path)
{
	// 实现参照 https://cppreference.cn/w/cpp/filesystem/path/append
	// 
	// 若 p.is_absolute() || (p.has_root_name() && p.root_name() != root_name())，则如同通过 operator=(p)，以 p 替换当前路径然后完成。
	//  * 否则，若 p.has_root_directory()，则从* this 的通用格式路径名移除任何根目录和整个相对路径。
	//	* 否则，若 has_filename() || (!has_root_directory() && is_absolute())，则追加 path::preferred_separator 到 * this 的通用格式。
	//	* 无论哪种方式，都接着追加 p 的原生格式路径名（从其通用格式中省去任何根名）到 * this 的原生格式。

	StringView pRootName = path.InternalGetRootName();

	if (path.IsAbsolute() || (!pRootName.Empty()) && (pRootName != InternalGetRootName()))
		m_path.Clear();
	else if (path.HasRootDirectory())
		m_path = pRootName;
	else if (HasFilename() || (!HasRootDirectory() && IsAbsolute()))
		m_path += '/';

	m_path += path.m_path;
	return *this;
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(const String& path)
{
	return *this /= Path(StringView(path));
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(StringView path)
{
	return *this /= Path(path);
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(const Ch* path)
{
	return *this /= Path(StringView(path));
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(const std::string& path)
{
	return *this /= Path(StringView(path));
}

PenFramework::PenEngine::Path& PenFramework::PenEngine::Path::operator/=(std::string_view path)
{
	return *this /= Path(StringView(path));
}

bool PenFramework::PenEngine::Path::IsAbsolute() const noexcept
{
	#ifdef _WIN32
	StringView rootName = InternalGetRootName();
	StringView rootDirectory = InternalGetRootDirectory();

	// 1. 驱动器盘符路径 C:(/\)
	if (rootName.Size() == 2 && rootName[1] == ':')
		return !rootDirectory.Empty();

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

	#endif //_WIN32

	return HasRootDirectory();
}

bool PenFramework::PenEngine::Path::IsRelative() const noexcept
{
	return !IsAbsolute();
}

bool PenFramework::PenEngine::Path::Empty() const noexcept
{
	return m_path.Empty();
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::RootName() const
{
	return BuildRawPath(InternalGetRootName());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::RootPath() const
{
	return BuildRawPath(InternalGetRootName(), InternalGetRootDirectory());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::RootDirectory() const
{
	return BuildRawPath(InternalGetRootDirectory());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::Filename() const
{
	return BuildRawPath(InternalGetFilename());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::Extension() const
{
	// 实现参照 https://cppreference.cn/w/cpp/filesystem/path/extension

	//如果通用格式路径的 filename() 部分包含一个句点（.），并且不是特殊文件系统元素 dot 或 dot - dot 之一，那么“扩展名”是从最右边的句点（包括句点）开始，直到路径名末尾的子字符串。
	//如果文件名的第一个字符是句点，则该句点将被忽略（像“.profile”这样的文件名不被视为扩展名）。
	//如果路径名是.或 ..，或者 filename() 不包含 '.' 字符，则返回空路径。
	//对于将附加元素（如备用数据流或分区数据集名称）附加到扩展名的文件系统，实现可能会定义附加行为。

	StringView filename = InternalGetFilename();

	if (Usize pos = filename.FindLastOf('.'); filename == ".." || filename == '.' || pos == 0 || pos == StringView::NPos)
		return {};
	else
		return BuildRawPath(filename.Substr(pos));
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::Stem() const
{
	// 实现参照 https://cppreference.cn/w/cpp/filesystem/path/stem

	//返回从 filename() 的开头开始，直到但不包括最后一个句点(.) 字符的子字符串，但有以下例外：
	//如果文件名中的第一个字符是句点，则忽略该句点（像 ".profile" 这样的文件名不被视为扩展名）
	//如果文件名是特殊文件系统组件 点 或 点 - 点 之一，或者它没有句点，则该函数返回整个 filename()

	StringView filename = InternalGetFilename();

	if (Usize pos = filename.FindLastOf('.'); filename == ".." || filename == '.' || pos == 0 || pos == StringView::NPos)
		return BuildRawPath(filename);
	else
		return BuildRawPath(filename.Substr(0, pos));
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::ParentPath() const
{
	return BuildRawPath(InternalGetParentPath());
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::RelativePath() const
{
	return BuildRawPath(InternalGetRelativePath());
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Path::Data() const noexcept
{
	return m_path.Data();
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::ToView() const
{
	return m_path;
}

bool PenFramework::PenEngine::Path::HasRootName() const noexcept
{
	return !InternalGetRootName().Empty();
}

bool PenFramework::PenEngine::Path::HasRootPath() const noexcept
{
	return !(InternalGetRootName().Empty() && InternalGetRootDirectory().Empty());
}

bool PenFramework::PenEngine::Path::HasRootDirectory() const noexcept
{
	return !InternalGetRootDirectory().Empty();
}

bool PenFramework::PenEngine::Path::HasRelativePath() const noexcept
{
	return !InternalGetRelativePath().Empty();
}

bool PenFramework::PenEngine::Path::HasParentPath() const noexcept
{
	return !InternalGetParentPath().Empty();
}

bool PenFramework::PenEngine::Path::HasFilename() const noexcept
{
	return !InternalGetFilename().Empty();
}

bool PenFramework::PenEngine::Path::HasStem() const noexcept
{
	return !Stem().Empty();
}

bool PenFramework::PenEngine::Path::HasExtension() const noexcept
{
	return !Extension().Empty();
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::operator/(const Path& path) const
{
	Path res(*this);
	res /= path;
	return res;
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::operator/(const String& path) const
{
	Path res(*this);
	res /= path;
	return res;
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::operator/(StringView path) const
{
	Path res(*this);
	res /= path;
	return res;
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::operator/(const Ch* path) const
{
	Path res(*this);
	res /= path;
	return res;
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::operator/(const std::string& path) const
{
	Path res(*this);
	res /= path;
	return res;
}

PenFramework::PenEngine::Path PenFramework::PenEngine::Path::operator/(std::string_view path) const
{
	Path res(*this);
	res /= path;
	return res;
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetRootName() const noexcept
{
	#ifdef _WIN32
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	Internal::PathAnalyzer::Iterator it = analyzer.Forward();

	if (it.State == Internal::PathAnalyzer::State::AtRootName)
		return it.Path;

	return {};
	#endif

	return {};
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetRootDirectory() const noexcept
{
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	Internal::PathAnalyzer::Iterator it = analyzer.Forward();

	#ifdef _WIN32
	if (it.State == Internal::PathAnalyzer::State::AtRootName)
		++it;
	#endif // _WIN32

	if (it.State == Internal::PathAnalyzer::State::AtRootDir)
		return it.Path;

	return {};
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetFilename() const noexcept
{
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	Internal::PathAnalyzer::Iterator it = analyzer.Forward();
	while (it.State <= Internal::PathAnalyzer::State::AtRootDir)
		++it;

	if (it.State == Internal::PathAnalyzer::State::AtEnd)
		return {};

	return Internal::PathAnalyzer(m_path,Internal::PathAnalyzer::State::AtEnd).Backward().Path;
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetRelativePath() const noexcept
{
	Internal::PathAnalyzer analyzer(m_path, Internal::PathAnalyzer::State::AtStart);
	auto it = analyzer.Forward();
	while (it.State <= Internal::PathAnalyzer::State::AtRootDir)
		++it;

	if (it.State == Internal::PathAnalyzer::State::AtEnd)
		return {};

	return StringView(it.Path.Data(),m_path.EndData() - 1);
}

PenFramework::PenEngine::StringView PenFramework::PenEngine::Path::InternalGetParentPath() const noexcept
{
	if(Empty())
		return {};

	Internal::PathAnalyzer forwardAnalyzer(m_path,Internal::PathAnalyzer::State::AtStart);
	Internal::PathAnalyzer::Iterator forwardIt = forwardAnalyzer.Forward();
	while(forwardIt.State <= Internal::PathAnalyzer::State::AtRootDir)
		++forwardIt;

	if(forwardIt.State == Internal::PathAnalyzer::State::AtEnd)
		return m_path;

	Internal::PathAnalyzer backwardAnalyzer(m_path, Internal::PathAnalyzer::State::AtEnd);
	Internal::PathAnalyzer::Iterator backwardIt = backwardAnalyzer.Backward();
	
	if(backwardIt.Path.Data() == m_path.Data())
		return {};

	--backwardIt;
	return StringView(m_path.Data(), backwardIt.Path.EndData());
}

PenFramework::PenEngine::String PenFramework::PenEngine::Path::Normalize(StringView path)
{
	// 实现参照 https://cppreference.cn/w/cpp/filesystem/path

	// 规范路径名具有以下语法

	//	1.root - name(可选)：在具有多个根的文件系统（例如 "C:" 或 "//myserver"）上标识根。在有歧义的情况下，形成有效 root - name 的最长字符序列被视为 root - name。标准库除了操作系统 API 所理解的根名之外，还可以定义额外的根名。
	//	2.root - directory(可选)：一个目录分隔符，如果存在，则将此路径标记为绝对路径。如果它不存在（且除根名之外的第一个元素是文件名），则该路径是相对路径，需要另一个路径作为起始位置才能解析为文件名。
	//	3.零个或多个以下内容
	//	file - name：非目录分隔符或首选目录分隔符的字符序列（操作系统或文件系统可能会施加额外的限制）。此名称可以标识文件、硬链接、符号链接或目录。识别两个特殊的 file - name
	//		dot：由一个点字符.组成的文件名是表示当前目录的目录名。
	//		dot - dot：由两个点字符 ..组成的文件名是表示父目录的目录名。
	//	directory - separator：正斜杠字符 / 或作为 path::preferred_separator 提供的替代字符。如果此字符重复，则将其视为单个目录分隔符： / usr///////lib 与 /usr/lib 相同。
	//	路径可以通过遵循以下算法进行规范化

	//	1.如果路径为空，则停止（空路径的规范形式是空路径）。
	//	2.将每个 directory - separator（可能由多个斜杠组成）替换为单个 path::preferred_separator。
	//	3.将 root - name 中的每个斜杠字符替换为 path::preferred_separator。
	//	4.删除每个 dot 及紧随其后的任何 directory - separator。
	//	5.删除每个非 dot - dot 文件名，其后紧跟一个 directory - separator 和一个 dot - dot，以及紧随其后的任何 directory - separator。
	//	6.如果存在 root - directory，则删除所有 dot - dot 和紧随其后的任何 directory - separator。
	//	7.如果最后一个文件名是 dot - dot，则删除任何尾随的 directory - separator。
	//	8.如果路径为空，则添加一个 dot（. / 的规范形式是.）。

	if (path.Empty()) // 1.如果路径为空，则停止（空路径的规范形式是空路径）。
		return {};

	String res(path.Size());

	std::vector<StringView> stack;

	Internal::PathNormalizer normalizer(path, Internal::PathNormalizer::State::AtStart);

	Internal::PathNormalizer::Iterator it = normalizer.Search();

	bool isAbsolute = false;

	// 第一次Search只会出现三种状态转移
	// 1.AtRootName 由 AtStart -> AtRootName
	// 2.AtRootDir 由 AtStart -> AtRootName [[fallthrough]] -> AtRootDir
	// 3.AtBlock 由 AtStart -> AtRootName [[fallthrough]] -> AtBlock
	if (it.CurrentState == Internal::PathNormalizer::State::AtRootName)
	{
		// Windows 绝对路径
		// "C:/(xxx)"

		// 为了避免根路径被错误弹出，直接将根插入到结果中
		res += it.Block;

		// 3.将 root - name 中的每个斜杠字符替换为 path::preferred_separator
		for (Ch& ch : res)
			if (Internal::IsValidSeparator(ch))
				ch = PreferredSeparator;

		isAbsolute = true;

		// 定向到第一个Block
		++it; // AtRootName -> AtRootDir
		++it; // AtRootDir -> AtBlock
	}
	else if (it.CurrentState == Internal::PathNormalizer::State::AtRootDir)
	{
		// Unix Like 绝对路径
		// "/(xxxx)"
		isAbsolute = true;

		// 定向到第一个Block
		++it; // AtRootDir -> AtBlock
	}
	// it.CurrentState == AtBlock 相对路径，直接分析路径
	// "xx/xxx"
	// "C:xx/xxx"

	while (it.CurrentState < Internal::PathNormalizer::State::AtEnd)
	{
		if (it.Block == "..")
		{
			if (isAbsolute)
			{
				// 6.如果存在 root - directory，则删除所有 dot - dot 和紧随其后的任何 directory - separator
				// 即如果是绝对路径，且栈为空，则不进行操作
				if (!stack.empty())
					stack.pop_back();
			}
			else
			{
				// 对于../../../../xxx 这种相对路径，需要保留".."
				// 不存在xxx/xxx/xx/../../../xx这种中间出现多个".."的情形，因为栈内不存在xx/..这种情形
				// 即如果back() == ".." 则前面的所有部分都是".."
				if (stack.empty() || stack.back() == "..")
					stack.push_back(it.Block);
				else
					// 5.删除每个非 dot - dot 文件名，其后紧跟一个 directory - separator 和一个 dot - dot，以及紧随其后的任何 directory - separator
					stack.pop_back();
			}
		}
		else if (it.Block != '.')
		{
			stack.push_back(it.Block);
		}

		// 4.删除每个 dot 及紧随其后的任何 directory - separator
		// 对于it.Block == '.' 直接continue

		++it;
	}

	for (auto block = stack.begin(), end = stack.end(); block != end; ++block)
	{
		// 2.将每个 directory - separator（可能由多个斜杠组成）替换为单个 path::preferred_separator
		// 由于block就是按dir - sep切分的，故可直接按block + PreferredSeparator 拼接
		// 7.如果最后一个文件名是 dot - dot，则删除任何尾随的 directory - separator
		// 不需要特判，因为无论如何，最后的directory - separator都被舍去了
		// 如果isAbsolute == true，则在Windows环境下会补全"C:/"的PreferredSeparator，如果是Unix Like环境，则会补全第一个PreferredSeparator
		if (isAbsolute || !res.Empty())
			res += PreferredSeparator;
		res += *block;
	}

	if (res.Empty())
	{
		if (isAbsolute)
			res += '/'; // 说明原路径就是"/......."unix like绝对路径，但是后续部分全部被舍去了 
		else
			res += '.'; // 8.如果路径为空，则添加一个 dot（. / 的规范形式是.）
	}

	res.ShrinkToFit();

	return res;
}
