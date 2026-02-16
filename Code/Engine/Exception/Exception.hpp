// File /Engine/Exception/Exception.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once
#include <exception>
#include <stacktrace>

namespace PenFramework::PenEngine
{
	class Exception : std::exception
	{
	public:
		virtual ~Exception() noexcept override = default;

		virtual const char* what() const noexcept override { return m_detail.data(); }

		std::string_view Source() const noexcept { return m_source; }
		std::string_view Operation() const noexcept { return m_operation; }
		std::string_view Detail() const noexcept { return m_detail; }
		const std::stacktrace& Stacktrace() const noexcept { return m_stacktrace; }
	private:
		std::string m_source;
		std::string m_operation;
		std::string m_detail;
		std::stacktrace m_stacktrace = std::stacktrace::current();
	};
}
