// File /Engine/Common/Iterator.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

// STL自己的各种迭代器以及工具函数很多都是硬编码的调用容器的函数名
// 这个设计思路对于框架自定义容器中是很麻烦的行为，故实现这些迭代器及其相关函数

#include <iterator>

namespace PenFramework::PenEngine
{
	template <typename Container>
	concept IsStandardBIContainer = requires (Container c,const typename Container::value_type& lv,typename Container::value_type&& rv)
	{
		c.push_back(lv);
		c.push_back(std::move(rv));
	};

	template <typename Container>
	concept IsFrameworkBIContainer = requires (Container c, const typename Container::value_type & lv, typename Container::value_type && rv)
	{
		c.PushBack(lv);
		c.PushBack(std::move(rv));
	};

	template <typename Container>
	concept IsStandardIContainer = requires (Container c, const typename Container::value_type & lv, typename Container::value_type && rv)
	{
		c.push_front(lv);
		c.push_front(std::move(rv));
	};

	template <typename Container>
	concept IsFrameworkFIContainer = requires (Container c, const typename Container::value_type & lv, typename Container::value_type && rv)
	{
		c.PushFront(lv);
		c.PushFront(std::move(rv));
	};

	template <typename Container>
	class BackInsertIterator
	{
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = void;
		using pointer = void;
		using reference = void;

		using container_type = Container;

		using difference_type = ptrdiff_t;

		constexpr explicit BackInsertIterator(Container& container) noexcept : m_container(std::addressof(container)) {}

		constexpr BackInsertIterator& operator=(const typename container_type::value_type& val)
		{
			if constexpr (IsStandardBIContainer<Container>)
				m_container->push_back(val);
			else if constexpr(IsFrameworkBIContainer<Container>)
				m_container->PushBack(val);
			else
				static_assert(false,"不受支持的容器类型");
			
			return *this;
		}

		constexpr BackInsertIterator& operator=(typename container_type::value_type&& val)
		{
			if constexpr (IsStandardBIContainer<Container>)
				m_container->push_back(std::move(val));
			else if constexpr (IsFrameworkBIContainer<Container>)
				m_container->PushBack(std::move(val));
			else
				static_assert(false, "不受支持的容器类型");

			return *this;
		}

		[[nodiscard]] constexpr BackInsertIterator& operator*() noexcept
		{
			return *this;
		}

		constexpr BackInsertIterator& operator++() noexcept
		{
			return *this;
		}

		constexpr BackInsertIterator& operator++(int) noexcept
		{
			return *this;
		}
	private:
		Container* m_container = nullptr;
	};

	template <typename Container>
	BackInsertIterator<Container> BackInserter(Container& container)
	{
		return BackInsertIterator(container);
	}

	template <typename Container>
	class FrontInsertIterator
	{
		using iterator_category = std::output_iterator_tag;
		using value_type = void;
		using pointer = void;
		using reference = void;

		using container_type = Container;

		using difference_type = ptrdiff_t;

		constexpr explicit FrontInsertIterator(Container& container) noexcept : m_container(std::addressof(container)) {}

		constexpr FrontInsertIterator& operator=(const typename container_type::value_type& val)
		{
			if constexpr (IsStandardIContainer<Container>)
				m_container->push_front(val);
			else if constexpr (IsFrameworkFIContainer<Container>)
				m_container->PushFront(val);
			else
				static_assert(false, "不受支持的容器类型");

			return *this;
		}

		constexpr FrontInsertIterator& operator=(typename container_type::value_type&& val)
		{
			if constexpr (IsStandardIContainer<Container>)
				m_container->push_front(std::move(val));
			else if constexpr (IsFrameworkFIContainer<Container>)
				m_container->PushFront(std::move(val));
			else
				static_assert(false, "不受支持的容器类型");

			return *this;
		}

		[[nodiscard]] constexpr FrontInsertIterator& operator*() noexcept
		{
			return *this;
		}

		constexpr FrontInsertIterator& operator++() noexcept
		{
			return *this;
		}

		constexpr FrontInsertIterator& operator++(int) noexcept
		{
			return *this;
		}
	private:
		Container* m_container = nullptr;
	};

	template <typename Container>
	FrontInsertIterator<Container> FrontInserter(Container& container)
	{
		return FrontInsertIterator(container);
	}
}
