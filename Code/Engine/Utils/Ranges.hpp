// File /Engine/Utils/Ranges.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
//
//-------------------------------------------------------------------------------
// 3rd Party Code: MSVC STL type_traits (core)
// Source: Microsoft Corporation (MSVC STL)
// License: Apache-2.0 WITH LLVM-exception
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//-------------------------------------------------------------------------------
//
// Note: Included temporarily to workaround issues with std::_Fake_copy_init 
//       in current build environment. 
// TODO: Investigate llvm-libcxx integration or standard-compliant alternative 
//       to remove this dependency.

#pragma once

#include "Concept.hpp"

namespace PenFramework::PenEngine
{
	namespace Detail
	{
		template <typename T>
		struct SourceChoice
		{
			T Choice;
			bool IsNothrow;
		};

		template <typename T>
		concept HasCompleteElementArray = requires (T & t) { sizeof(t[0]); };

		template <typename T>
		concept IsResourceSafeRangeTarget = std::is_lvalue_reference_v<T> || std::ranges::enable_borrowed_range<T>;
	}


	namespace BeginDetail
	{
		using std::begin;

		template <typename T>
		concept HasFrameworkContainerIterator = requires (T t)
		{
			{ std::_Fake_copy_init(t.Begin()) }-> std::input_or_output_iterator;
		};

		template <typename T>
		concept HasSTDLikeContainerIterator = requires (T t)
		{
			{ std::_Fake_copy_init(t.begin()) }-> std::input_or_output_iterator;
		};

		template <typename T>
		concept HasSTDLikeADL = requires(T t)
		{
			{ std::_Fake_copy_init(begin(t)) }-> std::input_or_output_iterator;
		};

		struct BeginStruct
		{
			enum class BeginChoice : U8
			{
				Array,
				FrameworkContainerIterator,
				STDLikeContainerIterator,
				STDLikeADLSearch,
				None
			};

			template <typename T>
			constexpr static Detail::SourceChoice<BeginChoice> CheckSource() noexcept
			{
				if constexpr (std::is_array_v<T>)
					return { BeginChoice::Array,true };
				else if constexpr (HasFrameworkContainerIterator<T>)
					return { BeginChoice::FrameworkContainerIterator,noexcept(std::_Fake_copy_init(std::declval<T>().Begin())) };
				else if constexpr (HasSTDLikeContainerIterator<T>)
					return { BeginChoice::STDLikeContainerIterator,noexcept(std::_Fake_copy_init(std::declval<T>().begin())) };
				else if constexpr (HasSTDLikeADL<T>)
					return { BeginChoice::STDLikeADLSearch,noexcept(std::_Fake_copy_init(begin(std::declval<T>()))) };
				else
					return { BeginChoice::None,true };
			}

			template <Detail::IsResourceSafeRangeTarget T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T&>().IsNothrow)
			{
				constexpr Detail::SourceChoice<BeginChoice> choice = CheckSource<T&>();

				if constexpr (choice.Choice == BeginChoice::Array)
				{
					static_assert(Detail::HasCompleteElementArray<T>,
								  "Do not accept arrays with incomplete element types.");
					return t;
				}
				else if constexpr (choice.Choice == BeginChoice::FrameworkContainerIterator)
					return t.Begin();
				else if constexpr (choice.Choice == BeginChoice::STDLikeContainerIterator)
					return t.begin();
				else if constexpr (choice.Choice == BeginChoice::STDLikeADLSearch)
					return begin(t);
				else
					static_assert(false, "Unsupported target");
			}
		};
	}

	constexpr inline BeginDetail::BeginStruct Begin = {};

	namespace EndDetail
	{
		using std::end;

		template <typename T>
		concept HasFrameworkContainerIterator = requires (T t)
		{
			{ std::_Fake_copy_init(t.End()) }-> std::input_or_output_iterator;
		};

		template <typename T>
		concept HasSTDLikeContainerIterator = requires (T t)
		{
			{ std::_Fake_copy_init(t.end()) }-> std::input_or_output_iterator;
		};

		template <typename T>
		concept HasSTDLikeADL = requires(T t)
		{
			{ std::_Fake_copy_init(end(t)) }-> std::input_or_output_iterator;
		};

		struct EndStruct
		{
			enum class EndChoice : U8
			{
				Array,
				FrameworkContainerIterator,
				STDLikeContainerIterator,
				STDLikeADLSearch,
				None
			};

			template <typename T>
			constexpr static Detail::SourceChoice<EndChoice> CheckSource() noexcept
			{
				if constexpr (std::is_array_v<T>)
					return { EndChoice::Array,true };
				else if constexpr (HasFrameworkContainerIterator<T>)
					return { EndChoice::FrameworkContainerIterator,noexcept(std::_Fake_copy_init(std::declval<T>().End())) };
				else if constexpr (HasSTDLikeContainerIterator<T>)
					return { EndChoice::STDLikeContainerIterator,noexcept(std::_Fake_copy_init(std::declval<T>().end())) };
				else if constexpr (HasSTDLikeADL<T>)
					return { EndChoice::STDLikeADLSearch,noexcept(std::_Fake_copy_init(end(std::declval<T>()))) };
				else
					return { EndChoice::None,true };
			}

			template <Detail::IsResourceSafeRangeTarget T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T&>().IsNothrow)
			{
				constexpr Detail::SourceChoice<EndChoice> choice = CheckSource<T&>();

				if constexpr (choice.Choice == EndChoice::Array)
				{
					static_assert(Detail::HasCompleteElementArray<T>,
								  "Do not accept arrays with incomplete element types.");
					return t;
				}
				else if constexpr (choice.Choice == EndChoice::FrameworkContainerIterator)
					return t.End();
				else if constexpr (choice.Choice == EndChoice::STDLikeContainerIterator)
					return t.end();
				else if constexpr (choice.Choice == EndChoice::STDLikeADLSearch)
					return end(t);
				else
					static_assert(false, "Unsupported target");
			}
		};
	}

	constexpr inline EndDetail::EndStruct End = {};

	namespace SizeDetail
	{
		using std::size;

		template <typename T>
		concept IntegralLikeType = std::is_integral_v<T> && !std::same_as<std::remove_cvref_t<T>, bool>;

		template <typename T>
		concept HasFrameworkContainerFunc = requires (T t) { { std::_Fake_copy_init(t.Size()) }-> IntegralLikeType; };

		template <typename T>
		concept HasSTDLikeContainerFunc = requires (T t) { { std::_Fake_copy_init(t.size()) }-> IntegralLikeType; };

		template <typename T>
		concept HasSTDLikeADL = requires (T t) { { std::_Fake_copy_init(size(t)) }-> IntegralLikeType; };

		template <typename T>
		concept CanDifferece = requires (T t)
		{
			{ Begin(t) }-> std::forward_iterator;
			{ End(t) }->std::sized_sentinel_for<decltype(Begin(t))>;
		};

		struct SizeStruct
		{
			enum class SizeChoice : U8
			{
				Array,
				FrameworkContainerFunc,
				STDLikeContainerFunc,
				STDLikeADLSearch,
				Difference,
				None
			};

			template <typename T>
			constexpr static Detail::SourceChoice<SizeChoice> CheckSource() noexcept
			{
				if constexpr (std::is_array_v<T>)
					return { SizeChoice::Array,true };
				else if constexpr (HasFrameworkContainerFunc<T>)
					return { SizeChoice::FrameworkContainerFunc,noexcept(std::_Fake_copy_init(std::declval<T>().Size())) };
				else if constexpr (HasSTDLikeContainerFunc<T>)
					return { SizeChoice::STDLikeContainerFunc,noexcept(std::_Fake_copy_init(std::declval<T>().size())) };
				else if constexpr (HasSTDLikeADL<T>)
					return { SizeChoice::STDLikeADLSearch,noexcept(std::_Fake_copy_init(size(std::declval<T>()))) };
				else if constexpr (CanDifferece<T>)
					return { SizeChoice::Difference,noexcept(PenEngine::End(std::declval<T>()) - PenEngine::Begin(std::declval<T>())) };
				else
					return { SizeChoice::None,true };
			}

			template <Detail::IsResourceSafeRangeTarget T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T&>().IsNothrow)
			{
				constexpr Detail::SourceChoice<SizeChoice> choice = CheckSource<T&>();

				if constexpr (choice.Choice == SizeChoice::Array)
				{
					static_assert(std::extent_v<T> != 0, "The size of target array is zero");

					return std::extent_v<std::remove_cvref_t<T&>>;
				}
				else if constexpr (choice.Choice == SizeChoice::FrameworkContainerFunc)
					return t.Size();
				else if constexpr (choice.Choice == SizeChoice::STDLikeContainerFunc)
					return t.size();
				else if constexpr (choice.Choice == SizeChoice::STDLikeADLSearch)
					return size(t);
				else if constexpr (choice.Choice == SizeChoice::Difference)
					return static_cast<Usize>(PenEngine::Begin(t) - PenEngine::End(t));
				else
					static_assert(false, "Unsupported target");
			}
		};
	}

	constexpr inline SizeDetail::SizeStruct Size = {};

	namespace DataDetail
	{
		template <typename T>
		concept HasFrameworkContainerFunc = requires (T t) { { std::_Fake_copy_init(t.Data()) }-> IsPointerToObject; };

		template <typename T>
		concept HasSTDLikeContainerFunc = requires (T t) { { std::_Fake_copy_init(t.data()) }-> IsPointerToObject; };

		template <typename T>
		concept HasContiguousIterator = requires(T t) { { std::to_address(PenEngine::Begin(t)) }-> std::contiguous_iterator; };

		struct DataStruct
		{
			enum class DataChoice : U8
			{
				FrameworkContainerFunc,
				STDLikeContainerFunc,
				Address,
				None
			};

			template <typename T>
			constexpr static Detail::SourceChoice<DataChoice> CheckSource() noexcept
			{
				if constexpr (HasFrameworkContainerFunc<T>)
					return { DataChoice::FrameworkContainerFunc,noexcept(std::_Fake_copy_init(std::declval<T>().Data())) };
				else if constexpr (HasSTDLikeContainerFunc<T>)
					return { DataChoice::STDLikeContainerFunc,noexcept(std::_Fake_copy_init(std::declval<T>().data())) };
				else if constexpr (HasContiguousIterator<T>)
					return { DataChoice::Address,noexcept(std::_Fake_copy_init(std::to_address(PenEngine::Begin(std::declval<T>())))) };
				else
					return { DataChoice::None,true };
			}

			template <Detail::IsResourceSafeRangeTarget T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T&>().IsNothrow)
			{
				constexpr Detail::SourceChoice<DataChoice> choice = CheckSource<T&>();

				if constexpr (choice.Choice == DataChoice::FrameworkContainerFunc)
					return t.Data();
				else if constexpr (choice.Choice == DataChoice::STDLikeContainerFunc)
					return t.data();
				else if constexpr (choice.Choice == DataChoice::Address)
					return std::to_address(PenEngine::Begin(t));
				else
					static_assert(false, "Unsupported target");
			}
		};
	}

	constexpr inline DataDetail::DataStruct Data = {};

	template <typename T>
	using IteratorType = decltype(PenEngine::Begin(std::declval<T&>()));

	template <typename T>
	using IteratorReferenceType = std::iter_reference_t<T>;

	template <typename T>
	using IteratorValueType = std::iter_value_t<T>;

	template <typename T>
	using RangeReferenceType = IteratorReferenceType<IteratorType<T>>;

	template <typename T>
	using RangeValueType = IteratorValueType<IteratorType<T>>;

	template <typename T>
	concept IsSupportRange = requires (T t) {
		PenEngine::Begin(t);
		PenEngine::End(t);
	};

	template <typename T>
	concept ContiguousRange = IsSupportRange<T> && std::contiguous_iterator<IteratorType<T>> && requires(T t)
	{
		{ PenEngine::Data(t) }-> std::same_as<std::add_pointer_t<RangeReferenceType<T>>>;
	};

	template <typename T>
	concept SizedRange = IsSupportRange<T> && requires (T t)
	{
		PenEngine::Size(t);
	};
}