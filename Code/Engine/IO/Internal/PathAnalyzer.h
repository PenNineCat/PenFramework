// File /Engine/IO/Internal/PathAnalyzer.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

// This is an internal dependent.

#pragma once

#include "../../String/String.hpp"

namespace PenFramework::PenEngine::Internal
{
	struct PathAnalyzer
	{
	public:
		enum class State : U8
		{
			AtStart,
			AtRootName,
			AtRootDir,
			AtFilename,
			AtRemainingSeparator,
			AtEnd,
		};
		struct Iterator
		{
			PathAnalyzer* Analyzer;
			State State;
			StringView View;

			explicit Iterator(PathAnalyzer* analyzer, PathAnalyzer::State state, StringView view) noexcept : Analyzer(analyzer), State(state), View(view) {}
			Iterator& operator=(const Iterator&) = default;

			Iterator& operator++() noexcept
			{
				return *this = Analyzer->Search();
			}
		};

		PathAnalyzer(StringView path, State startState) noexcept : m_rawPath(path), m_state(startState) {}

		// 正向搜索
		Iterator Search();

		const Ch* GetStartPointer() const noexcept;

		bool AtStart() const noexcept { return m_state == State::AtStart; }
		bool AtRootName() const noexcept { return m_state == State::AtRootName; }
		bool AtRootDir() const noexcept { return m_state == State::AtRootDir; }
		bool AtFilename() const noexcept { return m_state == State::AtFilename; }
		bool AtRemainingSeparator() const noexcept { return m_state == State::AtRemainingSeparator; }
		bool AtEnd() const noexcept { return m_state == State::AtEnd; }
	private:
		// 正向搜索辅助函数
		static const Ch* TrySkipRootName(const Ch* start, const Ch* end) noexcept;
		static const Ch* TrySkipDriveRoot(const Ch* start, const Ch* end) noexcept;
		static const Ch* TrySkipNetworkRoot(const Ch* start, const Ch* end) noexcept;
		static const Ch* TrySkipName(const Ch* start, const Ch* end) noexcept;
		static const Ch* TrySkipAllSeparator(const Ch* start, const Ch* end) noexcept;
		static const Ch* TrySkipCountedSeparator(const Ch* start, const Ch* end, Usize count) noexcept;

		static bool IsValidSeparator(Ch ch) noexcept;
		static bool IsValidDriveLetter(Ch ch) noexcept;

		StringView m_rawPath;
		StringView m_searchedPath;
		State m_state;
	};
}