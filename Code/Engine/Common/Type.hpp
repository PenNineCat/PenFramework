#pragma once

#include <__msvc_int128.hpp>
#include <cstdint>

namespace PenFramework::PenEngine
{
	/// @brief std::byte
	using b8e = std::byte;
	using b8 = uint8_t;
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	/// @brief struct _Unsigned128
	using u128c = std::_Unsigned128;
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	/// @brief struct _Signed128
	using i128c = std::_Signed128;
	using usize = size_t;
}