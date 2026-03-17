// File /Engine/String/StringView.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Utils/Iterator.hpp"
#include "../Common/Type.hpp"
#include "../DebugTools/Verify.hpp"
#include "../Exception/InvalidArgument.hpp"
#include "../Utils/Ranges.hpp"
#include "StrSearchUtils.hpp"
#include <format>
#include <string>

namespace PenFramework::PenEngine
{
	template <typename T>
	concept CurrentStringSupportCharType = IsOneOf<T, Ch, Ch32>;

	template <typename CharType>
	class StringConstIterator
	{
	public:
		using iterator_category = std::contiguous_iterator_tag;
		using iterator_concept = std::contiguous_iterator_tag;

		using value_type = CharType;
		using difference_type = Isize;

		using const_pointer = const value_type*;
		using pointer = const_pointer;

		using const_reference = const value_type&;
		using reference = const_reference;

		StringConstIterator() noexcept = default;
		explicit StringConstIterator(pointer ptr) noexcept : m_ptr(ptr) {}

		reference operator*() const noexcept
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				return *m_ptr;
		}
		pointer operator->() const noexcept
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				return m_ptr;
		}

		StringConstIterator& operator++() noexcept
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				++m_ptr;
			return *this;
		}

		StringConstIterator operator++(int) noexcept
		{
			StringConstIterator tmp = *this;
			++(*this);
			return tmp;
		}

		StringConstIterator& operator--() noexcept
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				--m_ptr;
			return *this;
		}

		StringConstIterator operator--(int) noexcept
		{
			StringConstIterator tmp = *this;
			--(*this);
			return tmp;
		}

		StringConstIterator& operator+=(difference_type off)
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				m_ptr += off;
			return *this;
		}

		StringConstIterator& operator-=(difference_type off)
		{
			return (*this) += -off;
		}

		friend StringConstIterator operator+(StringConstIterator it, int n) {
			it += n;
			return it;
		}

		friend StringConstIterator operator+(int n, StringConstIterator it) {
			it += n;
			return it;
		}

		StringConstIterator operator-(difference_type off) const noexcept
		{
			StringConstIterator tmp = *this;
			tmp -= off;
			return tmp;
		}

		difference_type operator-(const StringConstIterator& it) const noexcept
		{
			DEBUG_VERIFY_REPORT(it.m_ptr && m_ptr, "cannot dereference value-initialized string iterator")
				return static_cast<difference_type>(m_ptr - it.m_ptr);
		}

		reference operator[](difference_type n) const noexcept
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				return m_ptr[n];
		}

		bool operator==(const StringConstIterator& other) const noexcept = default;
		auto operator<=>(const StringConstIterator& other) const noexcept = default;

		pointer Data() const noexcept
		{
			DEBUG_VERIFY_REPORT(m_ptr, "cannot dereference value-initialized string iterator")
				return m_ptr;
		}
	protected:
		pointer m_ptr;
	};

	template <typename CharType>
	class BasicStringView
	{
	public:
		static_assert(CurrentStringSupportCharType<CharType>, "unsupported char type in current version");
		constexpr static Usize NPos = static_cast<Usize>(-1);

		using value_type = CharType;
		using pointer = value_type*;
		using const_pointer = const pointer;
		using reference = const value_type&;

		using size_type = Usize;
		using difference_type = Isize;

		using CharTraits = std::char_traits<CharType>;

		using ReverseIterator = std::reverse_iterator<StringConstIterator<CharType>>;
		using ConstReverseIterator = std::reverse_iterator<StringConstIterator<CharType>>;

		using iterator = StringConstIterator<CharType>;
		using const_iterator = StringConstIterator<CharType>;
		using reverse_iterator = ReverseIterator;
		using const_reverse_iterator = ConstReverseIterator;

		using ConstIterator = const_iterator;

		BasicStringView() noexcept = default;
		/*implicit*/ BasicStringView(const CharType* str) noexcept : BasicStringView(str, CharTraits::length(str)) {}
		BasicStringView(const CharType* str, Usize len) noexcept : m_str(str), m_len(len) {}
		BasicStringView(const CharType* begin, const CharType* end) noexcept : m_str(begin), m_len(end - begin) {}

		BasicStringView(ConstIterator begin, ConstIterator end) noexcept : m_str(begin.m_ptr), m_len(end.m_ptr - begin.m_ptr) {}

		template <typename Range> requires(!std::same_as<std::remove_cvref_t<Range>, BasicStringView>
		&& ContiguousRange<Range>
			&& SizedRange<Range>
			&& std::same_as<RangeValueType<Range>, CharType>
			&& !std::is_convertible_v<Range, const CharType*>
			&& !requires(std::remove_cvref_t<Range>& Rng)
		{
			Rng.operator BasicStringView<CharType>();
		})
			constexpr explicit BasicStringView(Range&& rng) noexcept(noexcept(PenEngine::Data(rng)) && noexcept(PenEngine::Size(rng)))
			: m_str(PenEngine::Data(rng)), m_len(static_cast<Usize>(PenEngine::Size(rng))) {
		}

		template <typename Range> requires(!std::same_as<std::remove_cvref_t<Range>, BasicStringView>
		&& ContiguousRange<Range>
			&& SizedRange<Range>
			&& std::same_as<RangeValueType<Range>, CharType>
			&& !std::is_convertible_v<Range, const CharType*>
			&& !requires(std::remove_cvref_t<Range>& Rng)
		{
			Rng.operator BasicStringView<CharType>();
		})
			constexpr BasicStringView& operator=(Range&& rng) noexcept(noexcept(PenEngine::Data(rng)) && noexcept(PenEngine::Size(rng)))
		{
			m_str = PenEngine::Data(rng);
			m_len = static_cast<Usize>(PenEngine::Size(rng));
			return *this;
		}

		bool operator==(const BasicStringView&) const noexcept = default;
		bool operator==(const std::basic_string<CharType>& str) const noexcept
		{
			return m_len == str.size() && CharTraits::compare(m_str,str.data(),m_len) == 0;
		}
		bool operator==(std::basic_string_view<CharType> str) const noexcept
		{
			return m_len == str.size() && CharTraits::compare(m_str, str.data(), m_len) == 0;
		}
		bool operator==(const CharType* str) const noexcept
		{
			return CharTraits::compare(m_str, str.data(), m_len) == 0;
		}

		Usize Capacity() const noexcept { return m_len; }
		Usize Size() const noexcept { return m_len; }

		const CharType* Data() noexcept;
		const CharType* Data() const noexcept;

		BasicStringView Substr(Usize off = 0, Usize len = NPos) const;
		BasicStringView Right(Usize len) const;
		BasicStringView Left(Usize len) const;

		bool Empty() const noexcept;

		bool Contain(CharType ch, Usize off = 0) const noexcept;
		bool Contain(BasicStringView str, Usize off = 0) const noexcept;
		bool Contain(const CharType* str, Usize off = 0) const noexcept;
		bool Contain(const CharType* str, Usize off, Usize len) const noexcept;
		bool Contain(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		bool Contain(std::basic_string_view<CharType>& str, Usize off = 0) const noexcept;

		ConstIterator begin() const noexcept;
		ConstIterator end() const noexcept;
		ConstIterator cbegin() const noexcept;
		ConstIterator cend() const noexcept;

		ConstReverseIterator rbegin() const noexcept;
		ConstReverseIterator rend() const noexcept;
		ConstReverseIterator crbegin() const noexcept;
		ConstReverseIterator crend() const noexcept;

		ConstIterator Begin() const noexcept;
		ConstIterator End() const noexcept;
		ConstIterator CBegin() const noexcept;
		ConstIterator CEnd() const noexcept;

		ConstReverseIterator RBegin() const noexcept;
		ConstReverseIterator REnd() const noexcept;
		ConstReverseIterator CRBegin() const noexcept;
		ConstReverseIterator CREnd() const noexcept;

		CharType Front() const noexcept;
		CharType Back() const noexcept;

		const CharType& operator[](Usize size) const noexcept;
		const CharType& At(Usize size) const;

		Usize Find(CharType ch, Usize off = 0) const noexcept;
		Usize Find(BasicStringView str, Usize off = 0) const noexcept;
		Usize Find(const CharType* str, Usize off = 0) const noexcept;
		Usize Find(const CharType* str, Usize off, Usize len) const noexcept;
		Usize Find(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		Usize Find(std::basic_string_view<CharType>& str, Usize off = 0) const noexcept;

		Usize FindFirstOf(CharType ch, Usize off = 0) const noexcept;
		Usize FindFirstOf(BasicStringView str, Usize off = 0) const noexcept;
		Usize FindFirstOf(const CharType* str, Usize off = 0) const noexcept;
		Usize FindFirstOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindFirstOf(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		Usize FindFirstOf(std::basic_string_view<CharType> str, Usize off = 0) const noexcept;

		Usize FindLastOf(CharType ch, Usize off = NPos) const noexcept;
		Usize FindLastOf(BasicStringView str, Usize off = 0) const noexcept;
		Usize FindLastOf(const CharType* str, Usize off = NPos) const noexcept;
		Usize FindLastOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindLastOf(const std::basic_string<CharType>& str, Usize off = NPos) const noexcept;
		Usize FindLastOf(std::basic_string_view<CharType> str, Usize off = NPos) const noexcept;

		Usize FindFirstNotOf(CharType ch, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(BasicStringView str, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(const CharType* str, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindFirstNotOf(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(std::basic_string_view<CharType> str, Usize off = 0) const noexcept;

		Usize FindLastNotOf(CharType ch, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(BasicStringView str, Usize off = 0) const noexcept;
		Usize FindLastNotOf(const CharType* str, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindLastNotOf(const std::basic_string<CharType>& str, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(std::basic_string_view<CharType> str, Usize off = NPos) const noexcept;
	private:
		const CharType* m_str = nullptr;
		Usize m_len = 0;
	};

	template <typename CharType>
	const CharType* BasicStringView<CharType>::Data() noexcept
	{
		return m_str;
	}

	template <typename CharType>
	const CharType* BasicStringView<CharType>::Data() const noexcept
	{
		return m_str;
	}

	template <typename CharType>
	BasicStringView<CharType> BasicStringView<CharType>::Substr(Usize off, Usize len) const
	{
		off = std::min(off, m_len);
		return BasicStringView(m_str + off, m_len - off);
	}

	template <typename CharType>
	BasicStringView<CharType> BasicStringView<CharType>::Right(Usize len) const
	{
		return Substr(m_len - len, len);
	}

	template <typename CharType>
	BasicStringView<CharType> BasicStringView<CharType>::Left(Usize len) const
	{
		return Substr(0, len);
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Empty() const noexcept
	{
		return m_str == nullptr || m_len == 0;
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Contain(CharType ch, Usize off) const noexcept
	{
		return Find(ch, off) != NPos;
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Contain(BasicStringView str, Usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Contain(const CharType* str, Usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Contain(const CharType* str, Usize off, Usize len) const noexcept
	{
		return Find(str, off, len) != NPos;
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Contain(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicStringView<CharType>::Contain(std::basic_string_view<CharType>& str, Usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::begin() const noexcept
	{
		return ConstIterator(Data());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::end() const noexcept
	{
		return ConstIterator(Data() + Size());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::cbegin() const noexcept
	{
		return ConstIterator(Data());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::cend() const noexcept
	{
		return ConstIterator(Data() + Size());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::rbegin() const noexcept
	{
		return ConstReverseIterator(end());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::rend() const noexcept
	{
		return ConstReverseIterator(begin());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::crbegin() const noexcept
	{
		return ConstReverseIterator(cend());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::crend() const noexcept
	{
		return ConstReverseIterator(cbegin());
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::Begin() const noexcept
	{
		return begin();
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::End() const noexcept
	{
		return end();
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::CBegin() const noexcept
	{
		return cbegin();
	}

	template <typename CharType>
	BasicStringView<CharType>::ConstIterator BasicStringView<CharType>::CEnd() const noexcept
	{
		return cend();
	}

	template <typename CharType>
	typename BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::RBegin() const noexcept
	{
		return rbegin();
	}

	template <typename CharType>
	typename BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::REnd() const noexcept
	{
		return rend();
	}

	template <typename CharType>
	typename BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::CRBegin() const noexcept
	{
		return crbegin();
	}

	template <typename CharType>
	typename BasicStringView<CharType>::ConstReverseIterator BasicStringView<CharType>::CREnd() const noexcept
	{
		return crend();
	}

	template <typename CharType>
	CharType BasicStringView<CharType>::Front() const noexcept
	{
		return Data()[0];
	}

	template <typename CharType>
	CharType BasicStringView<CharType>::Back() const noexcept
	{
		return Data()[m_len];
	}

	template <typename CharType>
	const CharType& BasicStringView<CharType>::operator[](Usize size) const noexcept
	{
		DEBUG_VERIFY_REPORT(size < m_len, "string subscription out of range")
			return m_str[size];
	}

	template <typename CharType>
	const CharType& BasicStringView<CharType>::At(Usize size) const
	{
		if (size >= m_len)
			throw InvalidArgument("BasicStringView", "Function At", "string subscription out of range");

		return m_str[size];
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::Find(CharType ch, Usize off) const noexcept
	{
		return ChFind(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::Find(BasicStringView str, Usize off) const noexcept
	{
		return Find(str.Data(), off, str.Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::Find(const CharType* str, Usize off) const noexcept
	{
		return Find(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::Find(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFind(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::Find(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::Find(std::basic_string_view<CharType>& str, Usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstOf(CharType ch, Usize off) const noexcept
	{
		return ChFindFirstOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstOf(BasicStringView str, Usize off) const noexcept
	{
		return FindFirstOf(str.Data(), off, str.Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstOf(const CharType* str, Usize off) const noexcept
	{
		return FindFirstOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindFirstOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastOf(CharType ch, Usize off) const noexcept
	{
		return ChFindLastOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastOf(BasicStringView str, Usize off) const noexcept
	{
		return FindLastOf(str.Data(), off, str.Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastOf(const CharType* str, Usize off) const noexcept
	{
		return FindLastOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindLastOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstNotOf(CharType ch, Usize off) const noexcept
	{
		return ChFindFirstNotOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstNotOf(BasicStringView str, Usize off) const noexcept
	{
		return FindFirstNotOf(str.Data(), off, str.Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstNotOf(const CharType* str, Usize off) const noexcept
	{
		return FindFirstNotOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstNotOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindFirstNotOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstNotOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindFirstNotOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastNotOf(CharType ch, Usize off) const noexcept
	{
		return ChFindLastNotOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastNotOf(BasicStringView str, Usize off) const noexcept
	{
		return FindLastNotOf(str.Data(), off, str.Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastNotOf(const CharType* str, Usize off) const noexcept
	{
		return FindLastNotOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastNotOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindLastNotOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastNotOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicStringView<CharType>::FindLastNotOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	using StringView = BasicStringView<Ch>;
	using U32View = BasicStringView<Ch32>;
}

template <>
struct std::formatter<PenFramework::PenEngine::StringView> : std::formatter<std::string_view>
{
	static auto format(PenFramework::PenEngine::StringView str, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{}", str.Data());
	}
};

template <typename CharType>
struct std::hash<PenFramework::PenEngine::BasicStringView<CharType>>
{
	static PenFramework::PenEngine::Usize operator()(PenFramework::PenEngine::BasicStringView<CharType> str) noexcept
	{
		return std::hash<std::string_view>::operator()(std::string_view(str.Data(), str.Size()));
	}
};