// File /Engine/IO/Internal/PathAnalyzer.h
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

#include "../../String/String.hpp"

namespace PenFramework::PenEngine::Internal
{
	class PathAnalyzer
	{
	public:
		enum class State : U8
		{
			AtStart,
			AtRootName,
			AtRootDir,
			AtBlock,
			AtEnd,
		};
		struct Iterator
		{
			PathAnalyzer* Analyzer;
			State State;
			StringView Path;

			Iterator& operator++() noexcept
			{
				return *this = Analyzer->Forward();
			}

			Iterator& operator--() noexcept
			{
				return *this = Analyzer->Backward();
			}
		};

		PathAnalyzer(StringView path, State startState) noexcept : m_rawPath(path), m_state(startState) {}

		// 正向搜索
		Iterator Forward() noexcept;
		Iterator Backward() noexcept;

		const Ch* GetForwardStartPointer() const noexcept;
		const Ch* GetBackwardStartPointer() const noexcept;

		bool AtStart() const noexcept { return m_state == State::AtStart; }
		bool AtRootName() const noexcept { return m_state == State::AtRootName; }
		bool AtRootDir() const noexcept { return m_state == State::AtRootDir; }
		bool AtFilename() const noexcept { return m_state == State::AtBlock; }
		bool AtEnd() const noexcept { return m_state == State::AtEnd; }
	private:
		StringView m_rawPath;
		StringView m_processedPath;
		State m_state;
	};
}