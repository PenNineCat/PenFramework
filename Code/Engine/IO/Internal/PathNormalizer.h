// File /Engine/IO/Internal/PathNormalizer.h
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
#include "../../String/StringView.hpp"

namespace PenFramework::PenEngine::Internal
{
	class PathNormalizer
	{
	public:
		enum class State : U8
		{
			AtStart,
			AtRootDir,
			AtRootName,
			AtBlock,
			AtEnd
		};

		struct Iterator
		{
			PathNormalizer* Normalizer;
			State CurrentState;
			StringView Block;


			Iterator& operator=(const Iterator&) noexcept = default;

			Iterator& operator++() noexcept
			{
				return *this = Normalizer->Search();
			}
		};

		PathNormalizer(StringView path, State state) noexcept : m_rawPath(path), m_state(state) {}

		Iterator Search() noexcept;
	private:
		const Ch* GetStartPointer() const noexcept;

		StringView m_rawPath;
		StringView m_processedPath;
		State m_state;
	};
}