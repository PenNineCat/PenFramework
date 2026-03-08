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
	// 由于如果Exception使用String来存储内部信息，会出现双重依赖问题，这里就先使用std::string，后续可能会换成const char*等基础内容
	class Exception : public std::exception
	{
	public:
		Exception() noexcept = default;
		Exception(std::string_view exceptionType,std::string_view source, std::string_view operation, std::string_view detail) :m_type(exceptionType), m_source(source), m_operation(operation), m_detail(detail) {}

		virtual ~Exception() noexcept override = default;

		virtual const char* what() const noexcept override { return m_detail.data(); }

		std::string_view Source() const noexcept { return m_source; }
		std::string_view Operation() const noexcept { return m_operation; }
		std::string_view Detail() const noexcept { return m_detail; }
		const std::stacktrace& Stacktrace() const noexcept { return m_stacktrace; }
	private:

		std::string m_type;
		std::string m_source;
		std::string m_operation;
		std::string m_detail;
		std::stacktrace m_stacktrace = std::stacktrace::current();
	};
}
