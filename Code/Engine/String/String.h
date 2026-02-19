// File /Engine/String/String.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <string>

#include "../Common/Type.hpp"
#include "../DebugTools/Verify.hpp"
#include "../Exception/Exception.hpp"
#include "../Memory/Memory.hpp"
#include "../Memory/Utils.hpp"
#include "../Utils/Concept.hpp"
#include "Utils.hpp"

#ifdef _USE_STD_VECTOR_FLOATING_ALGORITHMS
#define ENABLE_MSVC_CONTAINER_PARALLEL_ALGORITHMS
#endif // _USE_STD_VECTOR_FLOATING_ALGORITHMS

namespace PenFramework::PenEngine
{
	class String
	{
	public:
		static constexpr usize LocalStorageCapacity = 22;
		static constexpr usize Npos = static_cast<usize>(-1);
		static constexpr usize ReallocateMultiple = 2;
		static constexpr usize AllocateMask = 15;

		String() noexcept;
		String(const String& str, usize len = Npos);
		String(String&& str) noexcept;
		~String() noexcept;

		String& operator=(const String& str);
		String& operator=(String&& str) noexcept;

		explicit String(usize capacity);
		String(cch ch, usize count);
		template <CanConvertToU8CharType T>
		String(const T* ptr, usize len);
		template <CanConvertToU8CharType T>
		explicit String(const T* ptr);
		template <CanConvertToU8CharType T>
		explicit String(const std::basic_string<T>& str, usize off = 0, usize len = Npos);
		template <CanConvertToU8CharType T>
		explicit String(std::basic_string_view<T> str, usize off = 0, usize len = Npos);

		template <CanConvertToU8CharType T>
		String& operator=(T ch);
		template <CanConvertToU8CharType T>
		String& operator=(const std::basic_string<T>& str);
		template <CanConvertToU8CharType T>
		String& operator=(const T* ptr);

		usize Capacity() const noexcept;
		usize Size() const noexcept;
		bool Empty() const noexcept;

		void Reserve(usize newCapacity);
		void ReserveExtra(usize extraCapacity);
		void Resize(usize size, cch ch = cch());
		void ShrinkToFit();

		const cch* Data() noexcept;
		const cch* Data() const noexcept;

		String Substr(usize off = 0, usize len = Npos) const;

		cch Front() const noexcept;
		cch Back() const noexcept;

		String Left(usize len) const;
		String Right(usize len) const;

		cch& operator[](usize pos) noexcept;
		const cch& operator[](usize pos) const noexcept;

		void Clear() noexcept;

		void Append(const String& str);
		template <CanConvertToU8CharType T>
		void Append(T ch, usize count = 1);
		template <CanConvertToU8CharType T>
		void Append(const T* ptr);
		template <CanConvertToU8CharType T>
		void Append(const T* ptr, usize len);
		template <CanConvertToU8CharType T>
		void Append(const std::basic_string<T>& str, usize off = 0, usize len = Npos);
		template <CanConvertToU8CharType T>
		void Append(std::basic_string_view<T> str, usize off = 0, usize len = Npos);
		template <typename T> requires std::is_arithmetic_v<T>
		void Append(T v, usize off = 0);

		String& operator+=(const String& str);
		template <CanConvertToU8CharType T>
		String& operator+=(T ch);
		template <CanConvertToU8CharType T>
		String& operator+=(const T* ptr);
		template <CanConvertToU8CharType T>
		String& operator+=(const std::basic_string<T>& str);

		String operator+(const String& str)const;
		template <CanConvertToU8CharType T>
		String operator+(T ch)const;
		template <CanConvertToU8CharType T>
		String operator+(const T* ptr)const;
		template <CanConvertToU8CharType T>
		String operator+(const std::basic_string<T>& str)const;

		bool operator==(const String& str) const noexcept;
		template <CanConvertToU8CharType T>
		bool operator==(const T* ptr) const noexcept;
		template <CanConvertToU8CharType T>
		bool operator==(const std::basic_string<T>& str) const noexcept;
		template <CanConvertToU8CharType T>
		bool operator==(std::basic_string_view<T> str) const noexcept;

		template <CanConvertToU8CharType T>
		bool Contain(T ch, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		bool Contain(const T* ptr, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		bool Contain(const std::basic_string<T>& str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		bool Contain(std::basic_string_view<T> str, usize off = 0) const noexcept;

		usize Find(const String& str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize Find(T ch, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize Find(const T* ptr, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize Find(const std::basic_string<T>& str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize Find(std::basic_string_view<T> str, usize off = 0) const noexcept;

		template <CanConvertToU8CharType T>
		usize FindFirstOf(T ch, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastOf(T ch, usize off = Npos) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindFirstNotOf(T ch, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastNotOf(T ch, usize off = Npos) const noexcept;

		usize FindFirstOf(const String& str, usize off = 0) const noexcept;
		usize FindLastOf(const String& str, usize off = Npos) const noexcept;
		usize FindFirstNotOf(const String& str, usize off = 0) const noexcept;
		usize FindLastNotOf(const String& str, usize off = Npos) const noexcept;

		template <CanConvertToU8CharType T>
		usize FindFirstOf(const T* ptr, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastOf(const T* ptr, usize off = Npos) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindFirstNotOf(const T* ptr, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastNotOf(const T* ptr, usize off = Npos) const noexcept;

		template <CanConvertToU8CharType T>
		usize FindFirstOf(const std::basic_string<T>& str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastOf(const std::basic_string<T>& str, usize off = Npos) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindFirstNotOf(const std::basic_string<T>& str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastNotOf(const std::basic_string<T>& str, usize off = Npos) const noexcept;

		template <CanConvertToU8CharType T>
		usize FindFirstOf(std::basic_string_view<T> str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastOf(std::basic_string_view<T> str, usize off = Npos) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindFirstNotOf(std::basic_string_view<T> str, usize off = 0) const noexcept;
		template <CanConvertToU8CharType T>
		usize FindLastNotOf(std::basic_string_view<T> str, usize off = Npos) const noexcept;
	private:
		void ResetSizeAndEos(usize pos) noexcept;
		void DeallocateBuffer() noexcept;
		void ReallocateHeapBuffer(usize capacity);
		bool IsHeapBuffer() const noexcept;

		cch* Buffer() noexcept;
		const cch* Buffer() const noexcept;

		void InitSSOBuffer() noexcept;
		void InitHeapBuffer(usize capacity);

		usize CalculateReserveCapacity(usize requestCapacity, usize oldCapacity) const noexcept;

		void SwitchToHeap(usize capacity);
		void SwitchToStack() noexcept;
		union Buffer
		{
			struct Stack
			{
				u8 IsHeapBuffer : 1;
				u8 Size : sizeof(u8) * 8 - 1;
				cch Buffer[LocalStorageCapacity + 1];
			} Stack;
			struct Heap
			{
				usize IsHeapBuffer : 1;
				usize Capacity : sizeof(usize) * 8 - 1;
				usize Size;	
				cch* Buffer;
			} Heap;
		} m_buffer;
	};


	template <CanConvertToU8CharType T>
	String::String(const T* ptr, usize len)
	{
		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			memcpy(m_buffer.Stack.Buffer, ptr, len);
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len);
		}
		ResetSizeAndEos(len);
	}

	template <CanConvertToU8CharType T>
	String::String(const T* ptr) : String(ptr, std::char_traits<T>::length(ptr))
	{

	}

	template <CanConvertToU8CharType T>
	String::String(const std::basic_string<T>& str, usize off, usize len)
	{
		std::basic_string_view<T> view = std::basic_string_view<T>(str).substr(off, len);
		len = str.size();
		cch* ptr = str.data();

		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			memcpy(m_buffer.Stack.Buffer, ptr, len);
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len);
		}
		ResetSizeAndEos(len);
	}

	template <CanConvertToU8CharType T>
	String::String(std::basic_string_view<T> str, usize off, usize len)
	{
		std::basic_string_view<T> view = str.substr(off, len);
		len = str.size();
		cch* ptr = str.data();

		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			memcpy(m_buffer.Stack.Buffer, ptr, len);
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len);
		}
		ResetSizeAndEos(len);
	}

	template <CanConvertToU8CharType T>
	String& String::operator=(T ch)
	{
		DeallocateBuffer();
		m_buffer.Stack.Buffer[0] = static_cast<cch>(ch);
		ResetSizeAndEos(1);
		return *this;
	}

	template <CanConvertToU8CharType T>
	String& String::operator=(const std::basic_string<T>& str)
	{
		DeallocateBuffer();
		usize len = str.size();
		cch* ptr = str.data();
		if (len <= LocalStorageCapacity)
		{
			memcpy(m_buffer.Stack.Buffer, ptr, len);
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len);
		}
		ResetSizeAndEos(len);
		return *this;
	}


	template <CanConvertToU8CharType T>
	String& String::operator=(const T* ptr)
	{
		DeallocateBuffer();
		usize len = std::char_traits<T>::length(ptr);
		if (len <= LocalStorageCapacity)
		{
			memcpy(m_buffer.Stack.Buffer, ptr, len);
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len);
		}
		ResetSizeAndEos(len);
		return *this;
	}

	template <CanConvertToU8CharType T>
	void String::Append(T ch, usize count)
	{
		if (count == 0)
			return;

		ReserveExtra(count);

		cch* buffer = Buffer();
		usize size = Size();
		for (usize i = size; i < size + count; ++i)
			buffer[i] = static_cast<cch>(ch);
	}

	template <CanConvertToU8CharType T>
	void String::Append(const T* ptr)
	{
		Append(ptr, std::char_traits<T>::length(ptr));
	}

	template <CanConvertToU8CharType T>
	void String::Append(const T* ptr, usize len)
	{
		if (len == 0)
			return;

		ReserveExtra(len);
		usize size = Size();
		cch* buffer = Buffer();
		memcpy(buffer + size, ptr, len);
	}

	template <CanConvertToU8CharType T>
	void String::Append(const std::basic_string<T>& str, usize off, usize len)
	{
		std::basic_string_view<T> view = std::basic_string_view<T>(str).substr(off, len);
		Append(view.data(), view.size());
	}

	template <CanConvertToU8CharType T>
	void String::Append(std::basic_string_view<T> str, usize off, usize len)
	{
		std::basic_string_view<T> view = str.substr(off, len);
		Append(view.data(), view.size());
	}

	template <typename T> requires std::is_arithmetic_v<T>
	void String::Append(T v, usize off)
	{
		constexpr usize bufferLength = std::numeric_limits<T>::digits10 + 2;
		char buffer[bufferLength] = {};
		auto [ec, ptr] = std::to_chars(buffer, bufferLength, v);

		DEBUG_VERIFY_REPORT(ec == std::errc(), "buffer length must be large enough to accomodate the value")
			Append(buffer, static_cast<usize>(ptr - buffer));
	}

	template <CanConvertToU8CharType T>
	String& String::operator+=(T ch)
	{
		Append(ch);
		return *this;
	}

	template <CanConvertToU8CharType T>
	String& String::operator+=(const T* ptr)
	{
		Append(ptr);
		return *this;
	}

	template <CanConvertToU8CharType T>
	String& String::operator+=(const std::basic_string<T>& str)
	{
		Append(str);
		return *this;
	}

	template <CanConvertToU8CharType T>
	String String::operator+(T ch) const
	{
		String tmp = *this;
		tmp.Append(ch);
		return tmp;
	}

	template <CanConvertToU8CharType T>
	String String::operator+(const T* ptr) const
	{
		String tmp = *this;
		tmp.Append(ptr);
		return tmp;
	}

	template <CanConvertToU8CharType T>
	String String::operator+(const std::basic_string<T>& str) const
	{
		String tmp = *this;
		tmp.Append(str);
		return tmp;
	}

	template <CanConvertToU8CharType T>
	bool String::operator==(const T* ptr) const noexcept
	{
		return memcmp(Buffer(), ptr, Size()) == 0;
	}

	template <CanConvertToU8CharType T>
	bool String::operator==(const std::basic_string<T>& str) const noexcept
	{
		return operator==(str.data());
	}

	template <CanConvertToU8CharType T>
	bool String::operator==(std::basic_string_view<T> str) const noexcept
	{
		return operator==(str.data());
	}

	template <CanConvertToU8CharType T>
	bool String::Contain(T ch, usize off) const noexcept
	{
		return Find(ch, off) != Npos;
	}

	template <CanConvertToU8CharType T>
	bool String::Contain(const T* ptr, usize off) const noexcept
	{
		return Find(ptr, off) != Npos;
	}

	template <CanConvertToU8CharType T>
	bool String::Contain(const std::basic_string<T>& str, usize off) const noexcept
	{
		return Find(str, off) != Npos;
	}

	template <CanConvertToU8CharType T>
	bool String::Contain(std::basic_string_view<T> str, usize off) const noexcept
	{
		return Find(str, off) != Npos;
	}

	template <CanConvertToU8CharType T>
	usize String::Find(T ch, usize off) const noexcept
	{
		return std::_Traits_find_ch<std::char_traits<cch>>(Buffer(), Size(), off, static_cast<cch>(ch));
	}

	template <CanConvertToU8CharType T>
	usize String::Find(const T* ptr, usize off) const noexcept
	{
		return std::_Traits_find<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(ptr), std::char_traits<cch>::length(reinterpret_cast<const cch*>(ptr)));
	}

	template <CanConvertToU8CharType T>
	usize String::Find(const std::basic_string<T>& str, usize off) const noexcept
	{
		return std::_Traits_find<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::Find(std::basic_string_view<T> str, usize off) const noexcept
	{
		return std::_Traits_find<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstOf(T ch, usize off) const noexcept
	{
		return std::_Traits_find_ch<std::char_traits<cch>>(Buffer(), Size(), off, static_cast<cch>(ch));
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastOf(T ch, usize off) const noexcept
	{
		return std::_Traits_rfind_ch<std::char_traits<cch>>(Buffer(), Size(), off, static_cast<cch>(ch));
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstNotOf(T ch, usize off) const noexcept
	{
		return std::_Traits_find_not_ch<std::char_traits<cch>>(Buffer(), Size(), off, static_cast<cch>(ch));
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastNotOf(T ch, usize off) const noexcept
	{
		return std::_Traits_rfind_not_ch<std::char_traits<cch>>(Buffer(), Size(), off, static_cast<cch>(ch));
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstOf(const T* ptr, usize off) const noexcept
	{
		return std::_Traits_find_first_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(ptr), std::char_traits<cch>::length(reinterpret_cast<const cch*>(ptr)));
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastOf(const T* ptr, usize off) const noexcept
	{
		return std::_Traits_find_last_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(ptr), std::char_traits<cch>::length(reinterpret_cast<const cch*>(ptr)));
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstNotOf(const T* ptr, usize off) const noexcept
	{
		return std::_Traits_find_first_not_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(ptr), std::char_traits<cch>::length(reinterpret_cast<const cch*>(ptr)));
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastNotOf(const T* ptr, usize off) const noexcept
	{
		return std::_Traits_find_last_not_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(ptr), std::char_traits<cch>::length(reinterpret_cast<const cch*>(ptr)));
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstOf(const std::basic_string<T>& str, usize off) const noexcept
	{
		return std::_Traits_find_first_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastOf(const std::basic_string<T>& str, usize off) const noexcept
	{
		return std::_Traits_find_last_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstNotOf(const std::basic_string<T>& str, usize off) const noexcept
	{
		return std::_Traits_find_first_not_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastNotOf(const std::basic_string<T>& str, usize off) const noexcept
	{
		return std::_Traits_find_last_not_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstOf(std::basic_string_view<T> str, usize off) const noexcept
	{
		return std::_Traits_find_first_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastOf(std::basic_string_view<T> str, usize off) const noexcept
	{
		return std::_Traits_find_last_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindFirstNotOf(std::basic_string_view<T> str, usize off) const noexcept
	{
		return std::_Traits_find_first_not_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}

	template <CanConvertToU8CharType T>
	usize String::FindLastNotOf(std::basic_string_view<T> str, usize off) const noexcept
	{
		return std::_Traits_find_last_not_of<std::char_traits<cch>>(Buffer(), Size(), off, reinterpret_cast<const cch*>(str.data()), str.size());
	}
}
