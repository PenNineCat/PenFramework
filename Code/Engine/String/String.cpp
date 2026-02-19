// File /Engine/String/String.cpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "String.h"

#include "../Math/MathFunction.hpp"

namespace PenFramework::PenEngine
{

	String::String() noexcept
	{
		InitSSOBuffer();
	}

	String::String(const String& str, usize len) : String(str.Data(), std::min(len, str.Size()))
	{

	}

	String::String(String&& str) noexcept
	{
		InitSSOBuffer();
		std::swap(m_buffer, str.m_buffer);
	}

	String::~String() noexcept
	{
		DeallocateBuffer();
	}

	String& String::operator=(const String& str)
	{
		DeallocateBuffer();

		usize len = str.Size();
		const cch* ptr = str.Data();

		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			memcpy(m_buffer.Stack.Buffer, ptr, len);
			m_buffer.Stack.Buffer[len] = cch();
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len);
			m_buffer.Heap.Size = len;
		}

		return *this;
	}

	String& String::operator=(String&& str) noexcept
	{
		DeallocateBuffer();

		std::swap(m_buffer, str.m_buffer);

		return *this;
	}

	String::String(usize capacity)
	{
		if (capacity <= LocalStorageCapacity)
			InitSSOBuffer();
		else
			InitHeapBuffer(capacity);
	}

	String::String(cch ch, usize count)
	{
		if (count <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			for (usize i = 0; i < count; ++i)
				m_buffer.Stack.Buffer[i] = ch;
			m_buffer.Stack.Buffer[count] = cch();
			m_buffer.Stack.Size = count;
		}
		else
		{
			auto* buffer = new cch[count + 1];

			m_buffer.Heap.Capacity = count;
			m_buffer.Heap.Buffer = buffer;
			m_buffer.Heap.IsHeapBuffer = true;
			m_buffer.Heap.Size = count;

			for (usize i = 0; i < count; ++i)
				m_buffer.Heap.Buffer[i] = ch;

			m_buffer.Heap.Buffer[count] = cch();
		}
	}

	usize String::Capacity() const noexcept
	{
		if (IsHeapBuffer())
			return m_buffer.Heap.Capacity;
		return LocalStorageCapacity;
	}

	usize String::Size() const noexcept
	{
		if (IsHeapBuffer())
			return m_buffer.Heap.Size;
		return m_buffer.Stack.Size;
	}

	bool String::Empty() const noexcept
	{
		return Size() == 0;
	}

	void String::Reserve(usize newCapacity)
	{
		newCapacity = CalculateReserveCapacity(newCapacity, Capacity());

		if (IsHeapBuffer())
		{
			if (newCapacity > m_buffer.Heap.Capacity)
				ReallocateHeapBuffer(newCapacity);
		}
		else
		{
			if (newCapacity > LocalStorageCapacity)
				SwitchToHeap(newCapacity);
		}
	}

	void String::ReserveExtra(usize extraCapacity)
	{
		Reserve(Size() + extraCapacity);
	}

	void String::Resize(usize size, cch ch)
	{
		if (usize currentSize = Size(); size > currentSize)
		{
			ReserveExtra(size);
			cch* buffer = Buffer();
			for (usize i = currentSize; i < currentSize + size; ++i)
				buffer[i] = ch;
		}
		ResetSizeAndEos(size);
	}

	void String::ShrinkToFit()
	{
		if (usize size = Size(); size <= LocalStorageCapacity)
			SwitchToStack();
		else
			ReallocateHeapBuffer(size);
	}

	const cch* String::Data() noexcept
	{
		return Buffer();
	}

	const cch* String::Data() const noexcept
	{
		return Buffer();
	}

	String String::Substr(usize off, usize len) const
	{
		usize size = Size();

		if (off >= size)
			return {};

		len = std::min(len, size);
		const cch* buffer = Buffer();

		String tmp(len);
		cch* tmpBuffer = tmp.Buffer();

		memcpy(tmpBuffer, buffer, len);
		return tmp;
	}

	cch String::Front() const noexcept
	{
		return Buffer()[0];
	}

	cch String::Back() const noexcept
	{
		return Buffer()[Size() - 1];
	}

	String String::Left(usize len) const
	{
		return Substr(0, len);
	}

	String String::Right(usize len) const
	{
		usize size = Size();
		return Substr(size - len, len);
	}

	cch& String::operator[](usize pos) noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscript out of range")
			return Buffer()[pos];
	}

	const cch& String::operator[](usize pos) const noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscript out of range")
			return Buffer()[pos];
	}

	void String::Clear() noexcept
	{
		ResetSizeAndEos(0);
	}

	void String::Append(const String& str)
	{
		usize newSize = str.Size();
		ReserveExtra(newSize);
		MemoryCopy(str.Buffer(), Buffer() + Size(), newSize);
	}

	String& String::operator+=(const String& str)
	{
		Append(str);
		return *this;
	}

	String String::operator+(const String& str) const
	{
		String tmp = *this;
		tmp.Append(str);
		return tmp;
	}

	bool String::operator==(const String& str) const noexcept
	{
		usize lSize = Size();

		if (lSize != str.Size())
			return false;

		return memcmp(Buffer(), str.Buffer(), lSize) == 0;
	}

	usize String::Find(const String& str, usize off) const noexcept
	{
		return std::_Traits_find<std::char_traits<cch>>(Data(),Size(),off,str.Data(),str.Size());
	}

	usize String::FindFirstOf(const String& str, usize off) const noexcept
	{
		return std::_Traits_find_first_of<std::char_traits<cch>>(Data(),Size(),off,str.Data(),str.Size());
	}

	usize String::FindLastOf(const String& str, usize off) const noexcept
	{
		return std::_Traits_find_last_of<std::char_traits<cch>>(Data(), Size(), off, str.Data(), str.Size());
	}

	usize String::FindFirstNotOf(const String& str, usize off) const noexcept
	{
		return std::_Traits_find_first_not_of<std::char_traits<cch>>(Data(), Size(), off, str.Data(), str.Size());
	}

	usize String::FindLastNotOf(const String& str, usize off) const noexcept
	{
		return std::_Traits_find_last_not_of<std::char_traits<cch>>(Data(), Size(), off, str.Data(), str.Size());
	}

	void String::ResetSizeAndEos(usize pos) noexcept
	{
		if (IsHeapBuffer())
		{
			m_buffer.Heap.Size = pos;
			m_buffer.Heap.Buffer[pos] = cch();
		}
		else
		{
			DEBUG_VERIFY_REPORT(pos <= LocalStorageCapacity, "string subscript out of range")
				m_buffer.Stack.Size = pos;
			m_buffer.Stack.Buffer[pos] = cch();
		}
	}

	void String::DeallocateBuffer() noexcept
	{
		if (IsHeapBuffer())
			delete[] m_buffer.Heap.Buffer;

		InitSSOBuffer();
	}

	void String::ReallocateHeapBuffer(usize capacity)
	{
		auto newBuffer = new cch[capacity + 1];
		if (m_buffer.Heap.Size > 0)
			memcpy(newBuffer, m_buffer.Heap.Buffer, m_buffer.Heap.Size);
		newBuffer[m_buffer.Heap.Size] = cch();
		delete[] m_buffer.Heap.Buffer;
		m_buffer.Heap.Buffer = newBuffer;
		m_buffer.Heap.Capacity = capacity;
	}

	bool String::IsHeapBuffer() const noexcept
	{
		return m_buffer.Stack.IsHeapBuffer;
	}

	cch* String::Buffer() noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap.Buffer : m_buffer.Stack.Buffer;
	}

	const cch* String::Buffer() const noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap.Buffer : m_buffer.Stack.Buffer;
	}

	void String::InitSSOBuffer() noexcept
	{
		m_buffer.Stack.IsHeapBuffer = false;
		m_buffer.Stack.Size = 0;
		m_buffer.Stack.Buffer[0] = cch();
	}

	void String::InitHeapBuffer(usize capacity)
	{
		capacity = CalculateReserveCapacity(capacity, LocalStorageCapacity);

		auto buffer = new cch[capacity + 1];

		m_buffer.Heap.Size = 0;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer = buffer;
		m_buffer.Heap.IsHeapBuffer = true;
		m_buffer.Heap.Buffer[0] = cch();
	}

	usize String::CalculateReserveCapacity(usize requestCapacity, usize oldCapacity) const noexcept
	{
		usize max = PTRDIFF_MAX;

		usize masked = requestCapacity | AllocateMask;
		if(masked > max)
			return max;

		if(oldCapacity > max - oldCapacity / 2)
			return max;

		return Max(masked,oldCapacity + oldCapacity / 2);
	}

	void String::SwitchToHeap(usize capacity)
	{
		usize size = m_buffer.Stack.Size;

		auto buffer = new cch[capacity + 1];

		buffer[size] = cch();
		m_buffer.Heap.IsHeapBuffer = true;
		m_buffer.Heap.Buffer = buffer;
		m_buffer.Heap.Size = size;
		m_buffer.Heap.Capacity = capacity;
	}

	void String::SwitchToStack() noexcept
	{
		cch* buffer = m_buffer.Heap.Buffer;
		usize size = std::min(m_buffer.Heap.Size, LocalStorageCapacity);
		if (size > 0)
			memcpy(m_buffer.Stack.Buffer, buffer, size);
		delete[] buffer;
		m_buffer.Stack.Buffer[size] = cch();
	}
}
