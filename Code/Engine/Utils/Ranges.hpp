#pragma once

#include "Concept.hpp"
#include <ranges>

namespace PenFramework::PenEngine::Ranges
{
	namespace Detail
	{
		template <typename Source>
		struct SourceChoice
		{
			Source Source;
			bool Nothrow;
		};
	}

	namespace BeginDetail
	{
		template <typename T>
		concept HasMemberBeginIterator = requires(const T & t)
		{
			t.Begin();
		};

		struct BeginCallObject
		{
			enum class BeginCallSource
			{
				Func = 1,
				Ranges = 2
			};

			template <typename T>
			consteval static Detail::SourceChoice<BeginCallSource> CheckSource() noexcept
			{
				if constexpr (HasMemberBeginIterator<T>)
					return { BeginCallSource::Func,noexcept(std::declval<T>().Begin()) };
				else
					return { BeginCallSource::Ranges,noexcept(std::ranges::begin(std::declval<T>())) };
			}

			template <typename T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T>().Nothrow)
			{
				constexpr auto source = CheckSource<T>();
				if constexpr (source.Source == BeginCallSource::Func)
					return t.Begin();
				else
					return std::ranges::begin(t);
			}
		};
	}

	inline constexpr BeginDetail::BeginCallObject Begin = {};

	namespace EndDetail
	{
		template <typename T>
		concept HasMemberEndIterator = requires(T && t)
		{
			t.End();
		};

		struct EndCallObject
		{
			enum class EndCallSource
			{
				Func = 1,
				Ranges = 2
			};

			template <typename T>
			consteval static Detail::SourceChoice<EndCallSource> CheckSource() noexcept
			{
				if constexpr (HasMemberEndIterator<T>)
					return { EndCallSource::Func,noexcept(std::declval<T>().End()) };
				else
					return { EndCallSource::Ranges,noexcept(std::ranges::end(std::declval<T>())) };
			}

			template <typename T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T>().Nothrow)
			{
				constexpr auto source = CheckSource<T>();
				if constexpr (source.Source == EndCallSource::Func)
					return t.End();
				else
					return std::ranges::end(t);
			}
		};
	}

	inline constexpr EndDetail::EndCallObject End = {};

	namespace DataDetail
	{
		template <typename T>
		concept HasMemberDataFunc = requires (T && t)
		{
			{ t.Data() } -> IsPointerToObject;
		};

		template <typename T>
		concept HasContiguousIterator = requires (T && t)
		{
			{ t.Begin() } -> std::contiguous_iterator;
		};

		struct DataCallObject
		{
			enum class DataCallSource
			{
				Func = 1,
				Iterator = 2,
				Ranges = 3
			};

			template <typename T>
			consteval static Detail::SourceChoice<DataCallSource> CheckSource() noexcept
			{
				if constexpr (HasMemberDataFunc<T>)
					return { DataCallSource::Func,noexcept(std::declval<T>().Data()) };
				else if constexpr (HasContiguousIterator<T>)
					return { DataCallSource::Iterator,noexcept(std::to_address(std::declval<T>().Begin())) };
				else
					return { DataCallSource::Ranges,noexcept(std::ranges::data(std::declval<T>())) };
			}

			template <typename T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T>().Nothrow)
			{
				constexpr auto source = CheckSource<T>();
				if constexpr (source.Source == DataCallSource::Func)
					return t.Data();
				else if constexpr (source.Source == DataCallSource::Iterator)
					return std::to_address(t.Begin());
				else
					return std::ranges::data(t);
			}
		};
	}

	inline constexpr DataDetail::DataCallObject Data = {};

	namespace SizeDetail
	{
		template <typename T>
		concept HasMemberSizeFunc = requires (T && t)
		{
			{ t.Size() } -> IsPointerToObject;
		};

		template <typename T>
		concept HasDifferenceIterator = requires (T && t)
		{
			{ t.Begin() } -> std::forward_iterator;
			{ t.End() } -> std::sized_sentinel_for<decltype(t.Begin())>;
		};

		struct SizeCallObject
		{
			enum class SizeCallSouce
			{
				Func = 1,
				Iterator = 2,
				Ranges = 3
			};

			template <typename T>
			consteval static Detail::SourceChoice<SizeCallSouce> CheckSource() noexcept
			{
				if constexpr (HasMemberSizeFunc<T>)
					return { SizeCallSouce::Func,noexcept(std::declval<T>().Size()) };
				else if constexpr (HasDifferenceIterator<T>)
					return { SizeCallSouce::Iterator,noexcept(std::declval<T>().Begin() - std::declval<T>().End()) };
				else
					return { SizeCallSouce::Ranges,noexcept(std::ranges::size(std::declval<T>())) };
			}
			template <typename T>
			constexpr static auto operator()(T&& t) noexcept(CheckSource<T>().Nothrow)
			{
				constexpr auto source = CheckSource<T>();
				if constexpr (source.Source == SizeCallSouce::Func)
					return t.Size();
				else if constexpr (source.Source == SizeCallSouce::Iterator)
					return t.Begin() - t.End();
				else
					return std::ranges::size(t);
			}
		};
	}

	inline constexpr SizeDetail::SizeCallObject Size = {};

	template <typename T>
	concept Range = requires (T & t)
	{
		Begin(t);
		End(t);
	};

	template <typename T>
	using IteratorType = decltype(Ranges::Begin(std::declval<T&>()));

	template <Range T>
	using RangeValueType = std::iter_value_t<IteratorType<T>>;

	template <Range T>
	using RangeReferenceType = std::iter_reference_t<IteratorType<T>>;

	template <typename T>
	concept SizedRange = Range<T> && requires (const T & t)
	{
		{ Size(t) };
	};
	template <typename T>
	concept ContiguousRange = Range<T>
		&& std::contiguous_iterator<IteratorType<T>>
		&& requires (T && t)
	{
		{ Data(t) } ->std::same_as<std::add_pointer_t<RangeReferenceType<T>>>;
	};


}