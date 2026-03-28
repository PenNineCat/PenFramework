// File /Engine/Coroutine/AsyncTask.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <coroutine>

namespace PenFramework::PenEngine
{
	template <typename Ret>
	class AsyncTask
	{
	public:
		class Promise
		{
		public:
			AsyncTask get_return_object()
			{
				return AsyncTask(std::coroutine_handle<Promise>::from_promise(*this));
			}

			static std::suspend_always initial_suspend() noexcept { return {}; }
			static std::suspend_never final_suspend() noexcept { return {}; }

			void unhandled_exception();

			void return_value(const Ret& v)
			{
				m_v = v;
			}

			std::suspend_always yield_value(const Ret& v)
			{
				m_v = v;
				return {};
			}

			Ret& Get()
			{
				return m_v;
			}
		private:
			Ret m_v;
		};

		using promise_type = Promise;

		explicit AsyncTask(std::coroutine_handle<Promise> coroutine) :m_handle(coroutine) {}

		Ret Get()
		{
			m_handle.resume();
			return m_handle.promise().Get();
		}

		explicit operator bool() const noexcept
		{
			return !m_handle.done();
		}

		bool Done() const noexcept
		{
			return m_handle.done();
		}

		void Resume() const { m_handle.resume(); }
	private:
		std::coroutine_handle<Promise> m_handle;
	};
}