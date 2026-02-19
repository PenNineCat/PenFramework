// File /Engine/Utils/Flag.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once
#include <type_traits>

#define DECL_ENUM_FLAG_OPERATORS(EnumType) \
    constexpr PenFramework::PenEngine::Flag<EnumType> operator|(EnumType lhs, EnumType rhs) noexcept { \
        return PenFramework::PenEngine::Flag<EnumType>(lhs) | rhs; \
    } \
    constexpr PenFramework::PenEngine::Flag<EnumType> operator&(EnumType lhs, EnumType rhs) noexcept { \
        return PenFramework::PenEngine::Flag<EnumType>(lhs) & rhs; \
    } \
    constexpr PenFramework::PenEngine::Flag<EnumType> operator^(EnumType lhs, EnumType rhs) noexcept { \
        return PenFramework::PenEngine::Flag<EnumType>(lhs) ^ rhs; \
    } \
    constexpr PenFramework::PenEngine::Flag<EnumType> operator|(EnumType lhs, PenFramework::PenEngine::Flag<EnumType> rhs) noexcept { \
        return rhs | lhs; \
    } \
    constexpr PenFramework::PenEngine::Flag<EnumType> operator&(EnumType lhs, PenFramework::PenEngine::Flag<EnumType> rhs) noexcept { \
        return rhs & lhs; \
    } \
    constexpr PenFramework::PenEngine::Flag<EnumType> operator^(EnumType lhs, PenFramework::PenEngine::Flag<EnumType> rhs) noexcept { \
        return rhs ^ lhs; \
    } \
	constexpr EnumType operator~(EnumType lhs) noexcept { \
		return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(lhs)); \
	} \
    constexpr bool operator!(EnumType value) noexcept { \
        return static_cast<std::underlying_type_t<EnumType>>(value) == 0; \
    }
#define DECL_ENUM_FLAG_TYPE(EnumType) \
		using EnumType##Flag = PenFramework::PenEngine::Flag<EnumType>;  \
		DECL_ENUM_FLAG_OPERATORS(EnumType)

#define DECL_ENUM_FLAG_TYPE_WITH_NAME(EnumType,FlagType) \
		using FlagType = PenFramework::PenEngine::Flag<EnumType>; \
		DECL_ENUM_FLAG_OPERATORS(EnumType)

#define DECL_ENUM_FLAG_FRIEND_OPERATORS(EnumType) \
    friend constexpr PenFramework::PenEngine::Flag<EnumType> operator|(EnumType lhs, EnumType rhs) noexcept { \
        return PenEngine::Flag<EnumType>(lhs) | rhs; \
    } \
    friend constexpr PenFramework::PenEngine::Flag<EnumType> operator&(EnumType lhs, EnumType rhs) noexcept { \
        return PenEngine::Flag<EnumType>(lhs) & rhs; \
    } \
    friend constexpr PenFramework::PenEngine::Flag<EnumType> operator^(EnumType lhs, EnumType rhs) noexcept { \
        return PenEngine::Flag<EnumType>(lhs) ^ rhs; \
    } \
    friend constexpr PenFramework::PenEngine::Flag<EnumType> operator|(EnumType lhs, PenFramework::PenEngine::Flag<EnumType> rhs) noexcept { \
        return rhs | lhs; \
    } \
    friend constexpr PenFramework::PenEngine::Flag<EnumType> operator&(EnumType lhs, PenFramework::PenEngine::Flag<EnumType> rhs) noexcept { \
        return rhs & lhs; \
    } \
    friend constexpr PenFramework::PenEngine::Flag<EnumType> operator^(EnumType lhs, PenFramework::PenEngine::Flag<EnumType> rhs) noexcept { \
        return rhs ^ lhs; \
    } \
    friend constexpr bool operator!(EnumType value) noexcept { \
        return static_cast<std::underlying_type_t<EnumType>>(value) == 0; \
    }
#define DECL_ENUM_FLAG_FRIEND_TYPE(EnumType) \
		using EnumType##Flag = Flag<EnumType>;  \
		DECL_ENUM_FLAG_FRIEND_OPERATORS(EnumType)

#define DECL_ENUM_FLAG_FRIEND_TYPE_WITH_NAME(EnumType,FlagType) \
		using FlagType = Flag<EnumType>; \
		DECL_ENUM_FLAG_FRIEND_OPERATORS(EnumType)

namespace PenFramework::PenEngine
{
	template <typename EnumType> requires std::is_enum_v<EnumType>
	class Flag
	{
	public:
		using EnumUnderlyingType = std::underlying_type_t<EnumType>;

		constexpr Flag() noexcept = default;
		constexpr Flag(const Flag&) noexcept = default;
		constexpr Flag(Flag&&) noexcept = default;
		constexpr Flag& operator=(const Flag&) noexcept = default;
		constexpr Flag& operator=(Flag&&) noexcept = default;

		explicit constexpr Flag(EnumType flag) noexcept :
			m_value(flag) {}

		constexpr Flag& operator=(EnumType flag) noexcept
		{
			m_value = static_cast<EnumUnderlyingType>(flag);
			return *this;
		}

		constexpr Flag operator|(EnumType flag) const noexcept
		{
			return Flag(m_value | static_cast<EnumUnderlyingType>(flag));
		}

		constexpr Flag operator|(Flag other) const noexcept
		{
			return Flag(m_value | other.m_value);
		}

		constexpr Flag& operator|=(EnumType flag) noexcept
		{
			m_value |= static_cast<EnumType>(flag);
			return *this;
		}

		constexpr Flag& operator|=(Flag other) noexcept
		{
			m_value |= other.m_value;
			return *this;
		}

		constexpr Flag operator&(EnumType flag) const noexcept
		{
			return Flag(m_value & static_cast<EnumUnderlyingType>(flag));
		}

		constexpr Flag operator&(Flag other) const noexcept
		{
			return Flag(m_value & other.m_value);
		}

		constexpr Flag& operator&=(EnumType flag) noexcept
		{
			m_value &= static_cast<EnumType>(flag);
			return *this;
		}

		constexpr Flag& operator&=(Flag other) noexcept
		{
			m_value &= other.m_value;
			return *this;
		}

		constexpr Flag operator^(EnumType flag) const noexcept
		{
			return Flag(m_value ^ static_cast<EnumUnderlyingType>(flag));
		}

		constexpr Flag operator^(Flag other) const noexcept
		{
			return Flag(m_value ^ other.m_value);
		}

		constexpr Flag& operator^=(EnumType flag) noexcept
		{
			m_value ^= static_cast<EnumType>(flag);
			return *this;
		}

		constexpr Flag& operator^=(Flag other) noexcept
		{
			m_value ^= other.m_value;
			return *this;
		}

		constexpr Flag& operator~() const noexcept
		{
			return Flag(~m_value);
		}

		constexpr bool Test(EnumType flag) const noexcept
		{
			return (m_value & static_cast<EnumUnderlyingType>(flag)) != 0;
		}

		constexpr bool TestAll(Flag flag) const noexcept
		{
			return (m_value & flag.m_value) == flag.m_value;
		}

		constexpr bool Any() const noexcept
		{
			return m_value != 0;
		}

		constexpr bool operator==(const Flag&) const noexcept = default;
		constexpr bool operator==(EnumType flag) const noexcept
		{
			return m_value == static_cast<EnumUnderlyingType>(flag);
		}
	private:
		explicit constexpr Flag(EnumUnderlyingType value) noexcept :
			m_value(value) {}

		EnumUnderlyingType m_value = 0;
	};
}
