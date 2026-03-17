// File /Engine/String/StrSearchUtils.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Common/Type.hpp"
#include "../Utils/Concept.hpp"
#include <string>

namespace PenFramework::PenEngine
{
	static constexpr Usize NPos = static_cast<Usize>(-1);

	template <typename T>
	concept IsStdCharType = IsOneOf<T, Ch, Wch, Ch8, Ch16, Ch32>;

	template <typename T>
	concept CanConvertToU8CharType = IsStdCharType<T> && (sizeof(T) == sizeof(Ch8));

	template <typename T>
	concept CanConvertToU16CharType = IsStdCharType<T> && (sizeof(T) == sizeof(Ch16));

	template <typename T>
	concept CanConvertToU32CharType = IsStdCharType<T> && (sizeof(T) == sizeof(Ch32));

	template <typename CharType>
	Usize ChFind(CharType ch, Usize off, const CharType* source, Usize sourceLength) noexcept
	{
		if (sourceLength == 0 || off > sourceLength - 1)
			return NPos;

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;
		#ifdef _USE_STD_VECTOR_ALGORITHMS
		const CharType* res = std::_Find_vectorized(sourceStart, sourceEnd, ch);
		if (res == sourceEnd)
			return NPos;

		return static_cast<Usize>(res - source);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		const CharType* match = std::char_traits<CharType>::find(source, sourceLength, ch);
		if (match == nullptr)
			return NPos;

		return static_cast<Usize>(match - source);
	}

	template <typename CharType>
	Usize StrFind(const CharType* str, Usize off, Usize len, const CharType* source, Usize sourceLength) noexcept
	{
		if (len > sourceLength || off > sourceLength - len)
			return NPos;

		if (len == 0)
			return off;

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		const CharType* res = std::_Search_vectorized(sourceStart, sourceEnd, str, len);

		if (res != sourceEnd)
			return static_cast<Usize>(res - source);
		return NPos;

		#endif // _USE_STD_VECTOR_ALGORITHMS

		const CharType* matchesEnd = source + (sourceLength - len) + 1;

		for (const CharType* matchPosition = sourceStart;; ++matchPosition)
		{
			matchPosition = std::char_traits<CharType>::find(matchPosition, static_cast<size_t>(matchesEnd - matchPosition), *str);
			if (matchPosition == nullptr)
			{
				return NPos;
			}

			if (std::char_traits<CharType>::compare(matchPosition, str, len) == 0)
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}
	}

	template <typename CharType>
	Usize ChFindFirstOf(CharType ch, Usize off, const CharType* source, Usize sourceLength) noexcept
	{
		if (off >= sourceLength)
			return NPos;

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;
		#ifdef _USE_STD_VECTOR_ALGORITHMS
		const CharType* res = std::_Find_vectorized(sourceStart, sourceEnd, ch);
		if (res == sourceEnd)
			return NPos;

		return static_cast<Usize>(res - source);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		const CharType* match = std::char_traits<CharType>::find(source, sourceLength, ch);
		if (match == nullptr)
			return NPos;

		return static_cast<Usize>(match - source);
	}

	template <typename CharType>
	Usize StrFindFirstOf(const CharType* str, Usize off, Usize len, const CharType* source, Usize sourceLength) noexcept
	{
		if (off >= sourceLength)
			return NPos;

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (Usize vectorizedTargetLength = sourceLength - off; vectorizedTargetLength >= std::_Threshold_find_first_of)
		{
			Usize pos = std::_Find_first_of_pos_vectorized(sourceStart, vectorizedTargetLength, str, len);
			return pos != NPos ? pos + off : NPos;
		}
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<CharType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (Usize i = 0; i < len; ++i)
				bitmap[static_cast<U8>(str[i])] = true;

			for (Usize i = off; i < sourceLength; ++i)
				if (bitmap[static_cast<U8>(source[i])] == true)
					return i;

			return NPos;
		}

		for (const CharType* matchPosition = sourceStart; matchPosition < sourceEnd; ++matchPosition) {
			if (std::char_traits<CharType>::find(str, len, *matchPosition)) {
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename CharType>
	Usize ChFindLastOf(CharType ch, Usize off, const CharType* source, Usize sourceLength) noexcept
	{
		if (sourceLength == 0) { // no room for match
			return NPos;
		}

		off = std::min(off, sourceLength - 1);

		const CharType* sourceStart = source + off + 1;

		#if _USE_STD_VECTOR_ALGORITHMS
		const CharType* sourceEnd = source + off + 1;
		const CharType* res = std::_Find_last_vectorized(source, sourceEnd, ch);

		if (res != sourceEnd)
			return static_cast<size_t>(res - source);

		return NPos;
		#endif // _USE_STD_VECTOR_ALGORITHMS

		for (auto matchPosition = sourceStart;; --matchPosition) {
			if (std::char_traits<CharType>::eq(*matchPosition, ch)) {
				return static_cast<size_t>(matchPosition - source);
			}

			if (matchPosition == source) {
				return NPos;
			}
		}
	}

	template <typename CharType>
	Usize StrFindLastOf(const CharType* str, Usize off, Usize len, const CharType* source, Usize sourceLength) noexcept
	{
		if (len == 0 || sourceLength == 0)
			return NPos;

		off = std::min(sourceLength - 1, off);

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (Usize vectorizedTargetLength = off + 1; vectorizedTargetLength >= std::_Threshold_find_first_of)
			return std::_Find_last_of_pos_vectorized(source, off + 1, str, len);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<CharType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (Usize i = 0; i < len; ++i)
				bitmap[static_cast<U8>(str[i])] = true;

			for (Usize i = off; i > 0; --i)
				if (bitmap[static_cast<U8>(source[i - 1])] == true)
					return i - 1;

			return NPos;
		}

		for (const CharType* matchPosition = source + off; matchPosition > source; --matchPosition)
		{
			if (std::char_traits<CharType>::find(str, len, *matchPosition))
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename CharType>
	Usize ChFindFirstNotOf(CharType ch, Usize off, const CharType* source, Usize sourceLength) noexcept
	{
		if (off >= sourceLength)
			return NPos;

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;

		#if _USE_STD_VECTOR_ALGORITHMS
		const CharType* res = _STD _Find_not_ch_vectorized(sourceStart, sourceEnd, ch);
		if (res != sourceEnd)
			return static_cast<size_t>(res - source);
		return NPos;
		#endif // _USE_STD_VECTOR_ALGORITHMS

		for (auto matchPosition = sourceStart; matchPosition < sourceEnd; ++matchPosition)
		{
			if (!std::char_traits<CharType>::eq(*matchPosition, ch))
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename CharType>
	Usize StrFindFirstNotOf(const CharType* str, Usize off, Usize len, const CharType* source, Usize sourceLength) noexcept
	{
		if (off >= sourceLength)
			return NPos;

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (Usize vectorizedTargetLength = sourceLength - off; vectorizedTargetLength >= std::_Threshold_find_first_of)
		{
			Usize pos = std::_Find_first_not_of_pos_vectorized(sourceStart, sourceLength - off, str, len);
			return pos != NPos ? pos + off : NPos;
		}
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<CharType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (Usize i = 0; i < len; ++i)
				bitmap[static_cast<U8>(str[i])] = true;

			for (Usize i = off; i < sourceLength; ++i)
				if (bitmap[static_cast<U8>(source[i])] == false)
					return i;

			return NPos;
		}

		for (const CharType* matchPosition = sourceStart; matchPosition < sourceEnd; ++matchPosition) {
			if (!std::char_traits<CharType>::find(str, len, *matchPosition)) {
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename CharType>
	Usize ChFindLastNotOf(CharType ch, Usize off, const CharType* source, Usize sourceLength) noexcept
	{
		if (sourceLength == 0) { // no room for match
			return NPos;
		}

		off = std::min(off, sourceLength - 1);

		const CharType* sourceStart = source + off + 1;

		#if _USE_STD_VECTOR_ALGORITHMS
		const CharType* sourceEnd = source + off + 1;
		return std::_Find_last_not_ch_pos_vectorized(source, sourceEnd, ch);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		for (auto matchPosition = sourceStart;; --matchPosition) {
			if (!std::char_traits<CharType>::eq(*matchPosition, ch)) {
				return static_cast<size_t>(matchPosition - source);
			}

			if (matchPosition == source) {
				return NPos;
			}
		}
	}

	template <typename CharType>
	Usize StrFindLastNotOf(const CharType* str, Usize off, Usize len, const CharType* source, Usize sourceLength) noexcept
	{
		if (len == 0 || sourceLength == 0)
			return NPos;

		off = std::min(sourceLength - 1, off);

		const CharType* sourceStart = source + off;
		const CharType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (Usize vectorizedTargetLength = off + 1; vectorizedTargetLength >= std::_Threshold_find_first_of)
			return std::_Find_last_not_of_pos_vectorized(source, off + 1, str, len);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<CharType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (Usize i = 0; i < len; ++i)
				bitmap[static_cast<U8>(str[i])] = true;

			for (Usize i = off; i > 0; --i)
				if (bitmap[static_cast<U8>(source[i - 1])] == false)
					return i - 1;

			return NPos;
		}

		for (const CharType* matchPosition = source + off; matchPosition > source; --matchPosition)
		{
			if (!std::char_traits<CharType>::find(str, len, *matchPosition))
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

}
