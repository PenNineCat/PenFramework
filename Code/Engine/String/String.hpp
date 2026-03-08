// File /Engine/String/BasicString.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Common/Iterator.hpp"
#include "../Common/Type.hpp"
#include "../DebugTools/Verify.hpp"
#include "../Exception/Exception.hpp"
#include "../Math/MathFunction.hpp"
#include "../Memory/Memory.hpp"
#include "../Utils/Concept.hpp"
#include "StringView.hpp"
#include <boost/locale/encoding_utf.hpp>
#include <charconv>

namespace PenFramework::PenEngine
{
	class UtfConversionException : public Exception
	{
	public:
		explicit UtfConversionException(std::string_view source) : Exception("ConversionException", source, "尝试转换utf字符串", "转换失败") {}
	};

	template <typename CharType>
	class BasicString
	{
	public:
		static constexpr u8 LocalStorageCapacity = 22ull / sizeof(CharType);
		static constexpr u8 AllocateMask = sizeof(CharType) <= 1 ? 15
			: sizeof(CharType) <= 2 ? 7
			: sizeof(CharType) <= 4 ? 3
			: sizeof(CharType) <= 8 ? 1
			: 0;
		static constexpr usize MaxStorageCapacity = Pow(2ull, sizeof(usize) * BitsPerBytes - 1) - 1;
		static constexpr usize NPos = static_cast<usize>(-1);

		using CharTraits = std::char_traits<CharType>;

		using value_type = CharType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = usize;
		using difference_type = isize;

		using is_contiguous = std::true_type;

		class Iterator : public StringConstIterator<CharType>
		{
		public:
			using iterator_category = std::contiguous_iterator_tag;
			using value_type = value_type;
			using difference_type = isize;
			using pointer = value_type*;
			using reference = value_type&;

			constexpr Iterator() noexcept : StringConstIterator<CharType>() {}
			explicit constexpr Iterator(pointer ptr) noexcept : StringConstIterator<CharType>(ptr) {}

			reference operator*() const noexcept { return *const_cast<pointer>(this->m_ptr); }
			pointer operator->() const noexcept { return const_cast<pointer>(this->m_ptr); }

			Iterator& operator++() noexcept { ++this->m_ptr; return *this; }
			Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }
			Iterator& operator--() noexcept { --this->m_ptr; return *this; }
			Iterator operator--(int) noexcept { Iterator tmp = *this; --(*this); return tmp; }

			Iterator& operator+=(difference_type n) noexcept { this->m_ptr += n; return *this; }
			Iterator& operator-=(difference_type n) noexcept { this->m_ptr -= n; return *this; }

			friend Iterator operator+(Iterator it, difference_type n) noexcept { it += n; return it; }
			friend Iterator operator+(difference_type n, Iterator it) noexcept { it += n; return it; }
			friend Iterator operator-(Iterator it, difference_type n) noexcept { it -= n; return it; }

			difference_type operator-(const Iterator& it) const noexcept {
				return static_cast<difference_type>(this->m_ptr - it.m_ptr);
			}

			reference operator[](difference_type n) const noexcept { return const_cast<reference>(this->m_ptr[n]); }

			bool operator==(const Iterator&) const noexcept = default;
			auto operator<=>(const Iterator&) const noexcept = default;
		};

		using ConstIterator = StringConstIterator<CharType>;
		using ReverseIterator = std::reverse_iterator<Iterator>;
		using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

		using iterator = Iterator;
		using const_iterator = ConstIterator;
		using reverse_iterator = ReverseIterator;
		using const_reverse_iterator = ConstReverseIterator;


		BasicString() noexcept;

		explicit BasicString(usize capacity);
		BasicString(CharType ch, usize count);
		explicit BasicString(const CharType* str);
		BasicString(const CharType* str, usize length);

		template <typename SourceCharType>
		BasicString(const SourceCharType* str, usize length, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		BasicString(const BasicString& other);
		BasicString(BasicString&& other) noexcept;

		BasicString(const BasicString& other, usize len);

		BasicString(BasicStringView<CharType> str) : BasicString(str.Data(), str.Size())
		{
		}

		BasicString& operator=(const BasicString& other);
		BasicString& operator=(BasicString&& other) noexcept;
		BasicString& operator=(const CharType* str);
		BasicString& operator=(const std::basic_string<CharType>& str);
		BasicString& operator=(std::basic_string_view<CharType> str);
		BasicString& operator=(CharType ch);

		~BasicString() noexcept;

		BasicString& operator+=(const BasicString& other);
		BasicString& operator+=(const CharType* str);
		BasicString& operator+=(const std::basic_string<CharType>& str);
		BasicString& operator+=(std::basic_string_view<CharType> str);
		BasicString& operator+=(CharType ch);

		BasicString operator+(const BasicString& other);
		BasicString operator+(const CharType* str);
		BasicString operator+(const std::basic_string<CharType>& str);
		BasicString operator+(std::basic_string_view<CharType> str);
		BasicString operator+(CharType ch);

		bool operator==(const BasicString& other) const noexcept;
		bool operator==(const CharType* str) const noexcept;
		bool operator==(const std::basic_string<CharType>& str) const noexcept;
		bool operator==(std::basic_string_view<CharType> str) const noexcept;
		bool operator==(CharType ch) const noexcept;

		usize Size() const noexcept;
		usize Capacity() const noexcept;
		bool Empty() const noexcept;

		void Reserve(usize newCapacity);
		void ReserveExtra(usize extraCapacity);
		void Resize(usize size, CharType ch = CharType());
		void ShrinkToFit();

		CharType* Data() noexcept;
		const CharType* Data() const noexcept;

		BasicString Substr(usize off = 0, usize len = NPos) const;
		BasicString Right(usize len) const;
		BasicString Left(usize len) const;

		void Clear() noexcept;

		void Append(const BasicString& other);
		void Append(const CharType* str);
		void Append(const CharType* str, usize len);
		void Append(CharType ch, usize count = 1);
		void Append(const std::basic_string<CharType>& str);
		void Append(std::basic_string_view<CharType> str);

		template <typename SourceCharType>
		void ConvertAndAppend(const BasicString<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(const SourceCharType* str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(const SourceCharType* str, usize len, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		bool CanConvertToUnicode() const noexcept;

		void PushBack(const BasicString& other);
		void PushBack(const CharType* str);
		void PushBack(const CharType* str, usize len);
		void PushBack(CharType ch, usize count = 1);
		void PushBack(const std::basic_string<CharType>& str);
		void PushBack(std::basic_string_view<CharType> str);

		template <typename SourceCharType>
		void ConvertAndPushBack(const SourceCharType& other);
		template <typename SourceCharType>
		void ConvertAndPushBack(const SourceCharType* str);
		template <typename SourceCharType>
		void ConvertAndPushBack(const SourceCharType* str, usize len);
		template <typename SourceCharType>
		void ConvertAndPushBack(const std::basic_string<SourceCharType>& str);
		template <typename SourceCharType>
		void ConvertAndPushBack(std::basic_string_view<SourceCharType> str);

		void PushFront(const BasicString& other);
		void PushFront(const CharType* str);
		void PushFront(const CharType* str, usize len);
		void PushFront(CharType ch, usize count = 1);
		void PushFront(const std::basic_string<CharType>& str);
		void PushFront(std::basic_string_view<CharType> str);

		template <typename SourceCharType>
		void ConvertAndPushFront(const BasicString<SourceCharType>& other, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(const SourceCharType* str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(const SourceCharType* str, usize len, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		template <typename T> requires (std::is_arithmetic_v<T> && !std::is_same_v<T, CharType>)
			void Append(T v);

		bool Contain(CharType ch, usize off = 0) const noexcept;
		bool Contain(const CharType* str, usize off = 0) const noexcept;
		bool Contain(const CharType* str, usize off, usize len) const noexcept;
		bool Contain(const BasicString& other, usize off = 0) const noexcept;
		bool Contain(const std::basic_string<CharType>& str, usize off = 0) const noexcept;
		bool Contain(std::basic_string_view<CharType> str, usize off = 0) const noexcept;

		ConstIterator begin() const noexcept;
		ConstIterator end() const noexcept;
		ConstIterator cbegin() const noexcept;
		ConstIterator cend() const noexcept;
		Iterator begin() noexcept;
		Iterator end() noexcept;

		ConstReverseIterator rbegin() const noexcept;
		ConstReverseIterator rend() const noexcept;
		ConstReverseIterator crbegin() const noexcept;
		ConstReverseIterator crend() const noexcept;
		ReverseIterator rbegin() noexcept;
		ReverseIterator rend() noexcept;

		ConstIterator Begin() const noexcept;
		ConstIterator End() const noexcept;
		ConstIterator CBegin() const noexcept;
		ConstIterator CEnd() const noexcept;
		Iterator Begin() noexcept;
		Iterator End() noexcept;

		ConstReverseIterator RBegin() const noexcept;
		ConstReverseIterator REnd() const noexcept;
		ConstReverseIterator CRBegin() const noexcept;
		ConstReverseIterator CREnd() const noexcept;
		ReverseIterator RBegin() noexcept;
		ReverseIterator REnd() noexcept;

		CharType Front() const noexcept;
		CharType Back() const noexcept;

		reference operator[](usize pos) noexcept;
		const_reference operator[](usize pos) const noexcept;

		usize Find(CharType ch, usize off = 0) const noexcept;
		usize Find(const BasicString& other, usize off = 0) const noexcept;
		usize Find(const CharType* str, usize off = 0) const noexcept;
		usize Find(const CharType* str, usize off, usize len) const noexcept;
		usize Find(const std::basic_string<CharType>& str, usize off = 0) const noexcept;
		usize Find(std::basic_string_view<CharType> str, usize off = 0) const noexcept;

		usize FindFirstOf(CharType ch, usize off = 0) const noexcept;
		usize FindFirstOf(const BasicString& other, usize off = 0) const noexcept;
		usize FindFirstOf(const CharType* str, usize off = 0) const noexcept;
		usize FindFirstOf(const CharType* str, usize off, usize len) const noexcept;
		usize FindFirstOf(const std::basic_string<CharType>& str, usize off = 0) const noexcept;
		usize FindFirstOf(std::basic_string_view<CharType> str, usize off = 0) const noexcept;

		usize FindLastOf(CharType ch, usize off = NPos) const noexcept;
		usize FindLastOf(const BasicString& other, usize off = NPos) const noexcept;
		usize FindLastOf(const CharType* str, usize off = NPos) const noexcept;
		usize FindLastOf(const CharType* str, usize off, usize len) const noexcept;
		usize FindLastOf(const std::basic_string<CharType>& str, usize off = NPos) const noexcept;
		usize FindLastOf(std::basic_string_view<CharType> str, usize off = NPos) const noexcept;

		usize FindFirstNotOf(CharType ch, usize off = 0) const noexcept;
		usize FindFirstNotOf(const BasicString& other, usize off = 0) const noexcept;
		usize FindFirstNotOf(const CharType* str, usize off = 0) const noexcept;
		usize FindFirstNotOf(const CharType* str, usize off, usize len) const noexcept;
		usize FindFirstNotOf(const std::basic_string<CharType>& str, usize off = 0) const noexcept;
		usize FindFirstNotOf(std::basic_string_view<CharType> str, usize off = 0) const noexcept;

		usize FindLastNotOf(CharType ch, usize off = NPos) const noexcept;
		usize FindLastNotOf(const BasicString& other, usize off = NPos) const noexcept;
		usize FindLastNotOf(const CharType* str, usize off = NPos) const noexcept;
		usize FindLastNotOf(const CharType* str, usize off, usize len) const noexcept;
		usize FindLastNotOf(const std::basic_string<CharType>& str, usize off = NPos) const noexcept;
		usize FindLastNotOf(std::basic_string_view<CharType> str, usize off = NPos) const noexcept;
	protected:
		void ResetSizeAndEos(usize size) noexcept;

		CharType* Buffer() noexcept;
		const CharType* Buffer() const noexcept;

		static usize CalculateAllocateCapacity(usize requestCapacity, usize currentCapacity, usize maxCapacity) noexcept;

		void ReallocateHeapBuffer(usize capacity);
		void ReallocateHeapBufferWithCapacity(usize capacity);
		void DeallocateBuffer() noexcept;

		void InitSSOBuffer() noexcept;
		void InitHeapBuffer(usize capacity);

		bool IsHeapBuffer() const noexcept;

		void MoveToStack() noexcept;
		void MoveToHeap(usize capacity);

		union
		{
			struct
			{
				u8 IsHeapBuffer : 1;
				u8 Size : 7;
				CharType Buffer[LocalStorageCapacity + 1];
			} Stack;
			struct
			{
				usize IsHeapBuffer : 1;
				usize Size : sizeof(usize)* BitsPerBytes - 1;
				usize Capacity;
				CharType* Buffer;
			} Heap;
		} m_buffer;
	};

	using String = BasicString<cch>;
	using U32String = BasicString<ch32>;

	template <typename CharType>
	BasicString<CharType>::BasicString() noexcept
	{
		InitSSOBuffer();
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(usize capacity)
	{
		if (capacity <= LocalStorageCapacity)
			InitSSOBuffer();
		else
			InitHeapBuffer(capacity);
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(CharType ch, usize count)
	{
		if (count <= LocalStorageCapacity)
		{
			InitSSOBuffer();

			m_buffer.Stack.Size = count;

			for (usize i = 0; i < count; ++i)
				m_buffer.Stack.Buffer[i] = ch;

			m_buffer.Stack.Buffer[count] = CharType();
		}
		else
		{
			InitHeapBuffer(count);

			m_buffer.Heap.Size = count;

			for (usize i = 0; i < count; ++i)
				m_buffer.Heap.Buffer[i] = ch;

			m_buffer.Heap.Buffer[count] = CharType();
		}
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const CharType* str) : BasicString(str, CharTraits::length(str))
	{
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const CharType* str, usize length)
	{
		if (length <= LocalStorageCapacity)
		{
			InitSSOBuffer();

			memcpy(m_buffer.Stack.Buffer, str, length * sizeof(CharType));

			m_buffer.Stack.Size = length;
			m_buffer.Stack.Buffer[length] = CharType();
		}
		else
		{
			InitHeapBuffer(length);

			memcpy(m_buffer.Heap.Buffer, str, length * sizeof(CharType));

			m_buffer.Heap.Size = length;
			m_buffer.Heap.Buffer[length] = CharType();
		}
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const BasicString& other) : BasicString(other.Data(), other.Size())
	{

	}

	template <typename CharType>
	BasicString<CharType>::BasicString(BasicString&& other) noexcept
	{
		InitSSOBuffer();
		std::swap(m_buffer, other.m_buffer);
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const BasicString& other, usize len) : BasicString(other.Data(), std::min(len, other.Size()))
	{
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(const BasicString& other)
	{
		DeallocateBuffer();

		usize len = other.Size();
		const CharType* ptr = other.Data();

		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			memcpy(m_buffer.Stack.Buffer, ptr, len * sizeof(CharType));
			m_buffer.Stack.Size = len;
			m_buffer.Stack.Buffer[len] = CharType();
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len * sizeof(CharType));
			m_buffer.Heap.Size = len;
			m_buffer.Heap.Buffer[len] = CharType();
		}

		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(BasicString&& other) noexcept
	{
		DeallocateBuffer();
		std::swap(m_buffer, other.m_buffer);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(const CharType* str)
	{
		DeallocateBuffer();
		usize len = CharTraits::length(str);

		if (len <= LocalStorageCapacity)
		{
			memcpy(m_buffer.Stack.Buffer, str, len);
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, str, len);
		}
		ResetSizeAndEos(len);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(const std::basic_string<CharType>& str)
	{
		DeallocateBuffer();
		usize len = str.size();
		const CharType* ptr = str.data();
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

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(std::basic_string_view<CharType> str)
	{
		DeallocateBuffer();
		usize len = str.size();
		const CharType* ptr = str.data();
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

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(CharType ch)
	{
		DeallocateBuffer();
		m_buffer.Stack.Buffer[0] = static_cast<cch>(ch);
		ResetSizeAndEos(1);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>::~BasicString() noexcept
	{
		DeallocateBuffer();
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(const BasicString& other)
	{
		usize curSize = Size();
		usize othSize = other.Size();

		ReserveExtra(othSize);

		memcpy(Buffer() + curSize, other.Data(), othSize * sizeof(CharType));

		ResetSizeAndEos(curSize + othSize);

		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(const CharType* str)
	{
		usize curSize = Size();
		usize strLength = CharTraits::length(str);

		ReserveExtra(strLength);

		memcpy(Buffer() + curSize, str, strLength * sizeof(CharType));

		ResetSizeAndEos(curSize + strLength);

		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(const std::basic_string<CharType>& str)
	{
		usize curSize = Size();
		usize strSize = str.size();

		ReserveExtra(strSize);

		memcpy(Buffer() + curSize, str.data(), strSize * sizeof(CharType));

		ResetSizeAndEos(curSize + strSize);

		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(std::basic_string_view<CharType> str)
	{
		usize curSize = Size();
		usize strSize = str.size();

		ReserveExtra(strSize);

		memcpy(Buffer() + curSize, str.data(), strSize * sizeof(CharType));

		ResetSizeAndEos(curSize + strSize);

		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(CharType ch)
	{
		usize curSize = Size();

		ReserveExtra(1);

		Buffer()[curSize] = ch;

		ResetSizeAndEos(curSize + 1);

		return *this;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(const BasicString& other)
	{
		BasicString tmp = *this;
		tmp += other;
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(const CharType* str)
	{
		BasicString tmp = *this;
		tmp += str;
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(const std::basic_string<CharType>& str)
	{
		BasicString tmp = *this;
		tmp += str;
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(std::basic_string_view<CharType> str)
	{
		BasicString tmp = *this;
		tmp += str;
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(CharType ch)
	{
		BasicString tmp = *this;
		tmp += ch;
		return tmp;
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(const BasicString& other) const noexcept
	{
		return Size() == other.Size() && CharTraits::compare(Data(), other.Data(), Size()) == 0;
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(const CharType* str) const noexcept
	{
		return CharTraits::compare(Data(), str, Size()) == 0 && str[Size()] == CharType();
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(const std::basic_string<CharType>& str) const noexcept
	{
		return CharTraits::compare(Data(), str.data(), Size()) == 0 && str.size() == Size();
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(std::basic_string_view<CharType> str) const noexcept
	{
		return CharTraits::compare(Data(), str.data(), Size()) == 0 && str.size() == Size();
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(CharType ch) const noexcept
	{
		return Size() == 1 && Buffer()[0] == ch;
	}

	template <typename CharType>
	usize BasicString<CharType>::Size() const noexcept
	{
		if (IsHeapBuffer())
			return m_buffer.Heap.Size;
		return m_buffer.Stack.Size;
	}

	template <typename CharType>
	usize BasicString<CharType>::Capacity() const noexcept
	{
		if (IsHeapBuffer())
			return m_buffer.Heap.Capacity;
		return LocalStorageCapacity;
	}

	template <typename CharType>
	bool BasicString<CharType>::Empty() const noexcept
	{
		return Size() == 0;
	}

	template <typename CharType>
	void BasicString<CharType>::Reserve(usize newCapacity)
	{
		if (IsHeapBuffer())
		{
			if (newCapacity > m_buffer.Heap.Capacity)
				ReallocateHeapBuffer(newCapacity);
		}
		else
		{
			if (newCapacity > LocalStorageCapacity)
				MoveToHeap(newCapacity);
		}
	}

	template <typename CharType>
	void BasicString<CharType>::ReserveExtra(usize extraCapacity)
	{
		Reserve(Size() + extraCapacity);
	}

	template <typename CharType>
	void BasicString<CharType>::Resize(usize size, CharType ch)
	{
		if (usize currentSize = Size(); size > currentSize)
		{
			ReserveExtra(size);
			CharType* buffer = Buffer();
			for (usize i = currentSize; i < currentSize + size; ++i)
				buffer[i] = ch;
		}
		ResetSizeAndEos(size);
	}

	template <typename CharType>
	void BasicString<CharType>::ShrinkToFit()
	{
		if (IsHeapBuffer())
		{
			if (m_buffer.Heap.Size <= LocalStorageCapacity)
				MoveToStack();
			else if (m_buffer.Heap.Size < m_buffer.Heap.Capacity)
				ReallocateHeapBufferWithCapacity(m_buffer.Heap.Size);
		}
	}

	template <typename CharType>
	CharType* BasicString<CharType>::Data() noexcept
	{
		return Buffer();
	}

	template <typename CharType>
	const CharType* BasicString<CharType>::Data() const noexcept
	{
		return Buffer();
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::Substr(usize off, usize len) const
	{
		usize size = Size();

		if (off >= size)
			return {};

		len = std::min(len, size - off);

		return BasicString(Buffer() + off, len);
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::Right(usize len) const
	{
		return Substr(Size() - len, len);
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::Left(usize len) const
	{
		return Substr(0, len);;
	}

	template <typename CharType>
	void BasicString<CharType>::Clear() noexcept
	{
		ResetSizeAndEos(0);
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const BasicString& other)
	{
		if (other.IsHeapBuffer())
			Append(other.m_buffer.Heap.Buffer, other.m_buffer.Heap.Size);
		else
			Append(other.m_buffer.Stack.Buffer, other.m_buffer.Stack.Size);
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const CharType* str)
	{
		return Append(str, CharTraits::length(str));
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const CharType* str, usize len)
	{
		if (len == 0)
			return;

		ReserveExtra(len);
		usize size = Size();
		CharType* buffer = Buffer();
		memcpy(buffer + size, str, len);

		ResetSizeAndEos(size + len);
	}

	template <typename CharType>
	void BasicString<CharType>::Append(CharType ch, usize count)
	{
		if (count == 0)
			return;

		ReserveExtra(count);

		CharType* buffer = Buffer();
		usize size = Size();
		for (usize i = size; i < size + count; ++i)
			buffer[i] = ch;

		ResetSizeAndEos(size + count);
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const std::basic_string<CharType>& str)
	{
		Append(str.data(), str.size());
	}

	template <typename CharType>
	void BasicString<CharType>::Append(std::basic_string_view<CharType> str)
	{
		Append(str.data(), str.size());
	}

	template<typename CharType>
	template<typename SourceCharType>
	BasicString<CharType>::BasicString(const SourceCharType* str, usize length, boost::locale::conv::method_type how)
	{
		InitSSOBuffer();
		ConvertAndAppend(str, length, how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndAppend(const BasicString<SourceCharType>& str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str.Data(), str.Size(), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndAppend(const SourceCharType* str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str, std::char_traits<SourceCharType>(str), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndAppend(const SourceCharType* str, usize len, boost::locale::conv::method_type how)
	{
		if (str == nullptr || len == 0)
			return;

		std::basic_string<CharType> tmp = boost::locale::conv::utf_to_utf<CharType>(str, str + len, how);

		Append(tmp);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndAppend(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str.data(), str.size(), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndAppend(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str.data(), str.size());
	}

	template <typename CharType>
	bool BasicString<CharType>::CanConvertToUnicode() const noexcept
	{
		CharType* str = const_cast<CharType*>(Data());
		CharType* end = str + Size();

		while (str != end) {
			boost::locale::utf::code_point c = boost::locale::utf::utf_traits<CharType>::decode(str, end);
			if (c == boost::locale::utf::illegal || c == boost::locale::utf::incomplete)
				return false;
		}

		return true;
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(const BasicString& other)
	{
		Append(other);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(const CharType* str)
	{
		Append(str);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(const CharType* str, usize len)
	{
		Append(str, len);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(CharType ch, usize count)
	{
		Append(ch, count);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(const std::basic_string<CharType>& str)
	{
		Append(str);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(std::basic_string_view<CharType> str)
	{
		Append(str);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const SourceCharType& other)
	{
		ConvertAndAppend(other);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const SourceCharType* str)
	{
		ConvertAndAppend(str);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const SourceCharType* str, usize len)
	{
		ConvertAndAppend(str, len);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const std::basic_string<SourceCharType>& str)
	{
		ConvertAndAppend(str);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(std::basic_string_view<SourceCharType> str)
	{
		ConvertAndAppend(str);
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const BasicString& other)
	{
		if (other.IsHeapBuffer())
			PushFront(other.m_buffer.Heap.Buffer, other.m_buffer.Heap.Size);
		else
			PushFront(other.m_buffer.Stack.Buffer, other.m_buffer.Stack.Size);
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const CharType* str)
	{
		PushFront(str, CharTraits::length(str));
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const CharType* str, usize len)
	{
		if (str == nullptr || len == 0)
			return;

		ReserveExtra(len);

		CharType* buffer = Buffer();
		usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + len);

		memcpy(buffer, str, len * sizeof(CharType));

		ResetSizeAndEos(size + len);
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(CharType ch, usize count)
	{
		if (count == 0)
			return;

		ReserveExtra(count);

		CharType* buffer = Buffer();
		usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + count);

		for (usize i = 0; i < count; ++i)
			buffer[i] = ch;

		ResetSizeAndEos(size + count);
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const std::basic_string<CharType>& str)
	{
		PushFront(str.data(), str.size());
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(std::basic_string_view<CharType> str)
	{
		PushFront(str.data(), str.size());
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushFront(const BasicString<SourceCharType>& other, boost::locale::conv::method_type how)
	{
		ConvertAndPushFront(other.Data(), other.Size(), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushFront(const SourceCharType* str, boost::locale::conv::method_type how)
	{
		ConvertAndPushFront(str, std::char_traits<SourceCharType>::length(str), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushFront(const SourceCharType* str, usize len, boost::locale::conv::method_type how)
	{
		if (str == nullptr || len == 0)
			return;

		usize requiredLength = 0;

		const SourceCharType* start = str;
		const SourceCharType* end = str + len;

		// 计算转换后需要分配字符串长度
		while (start != end)
		{
			const boost::locale::utf::code_point c = boost::locale::utf::utf_traits<SourceCharType>::decode(start, end);
			if (c == boost::locale::utf::illegal || c == boost::locale::utf::incomplete) {
				if (how == boost::locale::conv::method_type::stop)
					throw UtfConversionException("BasicString");
			}
			else
				requiredLength += boost::locale::utf::utf_traits<CharType>::width(c);
		}

		// 重分配
		ReserveExtra(requiredLength);

		// 移动原串
		CharType* buffer = Buffer();
		usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + requiredLength);

		start = str;

		// 解码串
		while (start != end) {
			const boost::locale::utf::code_point c = boost::locale::utf::utf_traits<SourceCharType>::decode(start, end);
			// 这里不会有异常情况，因为如果有，计算长度时就已经抛出
			boost::locale::utf::utf_traits<CharType>::encode(c, buffer);
		}

		// 设置大小与\0结尾符位置为新的长度
		ResetSizeAndEos(size + requiredLength);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushFront(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how)
	{
		ConvertAndPushFront(str.data(), str.size(), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushFront(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how)
	{
		ConvertAndPushFront(str.data(), str.size(), how);
	}

	template <typename CharType>
	template <typename T> requires (std::is_arithmetic_v<T> && !std::is_same_v<T, CharType>)
		void BasicString<CharType>::Append(T v)
	{
		constexpr usize bufferLength = std::numeric_limits<T>::digits10 + 2;
		char buffer[bufferLength] = {};
		auto [ec, ptr] = std::to_chars(buffer, bufferLength, v);

		DEBUG_VERIFY_REPORT(ec == std::errc(), "buffer length must be large enough to accomodate the value")
			Append(buffer, static_cast<usize>(ptr - buffer));
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(CharType ch, usize off) const noexcept
	{
		return Find(ch, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const BasicString& other, usize off) const noexcept
	{
		return Find(other, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const CharType* str, usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const CharType* str, usize off, usize len) const noexcept
	{
		return Find(str, off, len) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const std::basic_string<CharType>& str, usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(std::basic_string_view<CharType> str, usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::begin() const noexcept
	{
		return ConstIterator(Buffer());
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::end() const noexcept
	{
		return ConstIterator(Buffer() + Size());
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::cbegin() const noexcept
	{
		return ConstIterator(Buffer());
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::cend() const noexcept
	{
		return ConstIterator(Buffer() + Size());
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::begin() noexcept
	{
		return Iterator(Buffer());
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::end() noexcept
	{
		return Iterator(Buffer() + Size());
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::rbegin() const noexcept
	{
		return ConstReverseIterator(end());
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::rend() const noexcept
	{
		return ConstReverseIterator(begin());
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::crbegin() const noexcept
	{
		return ConstReverseIterator(cend());
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::crend() const noexcept
	{
		return ConstReverseIterator(cbegin());
	}

	template <typename CharType>
	BasicString<CharType>::ReverseIterator BasicString<CharType>::rbegin() noexcept
	{
		return ReverseIterator(end());
	}

	template <typename CharType>
	BasicString<CharType>::ReverseIterator BasicString<CharType>::rend() noexcept
	{
		return ReverseIterator(begin());
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::Begin() const noexcept
	{
		return begin();
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::End() const noexcept
	{
		return end();
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::CBegin() const noexcept
	{
		return cbegin();
	}

	template <typename CharType>
	BasicString<CharType>::ConstIterator BasicString<CharType>::CEnd() const noexcept
	{
		return cend();
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::Begin() noexcept
	{
		return begin();
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::End() noexcept
	{
		return end();
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::RBegin() const noexcept
	{
		return rbegin();
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::REnd() const noexcept
	{
		return rend();
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::CRBegin() const noexcept
	{
		return crbegin();
	}

	template <typename CharType>
	BasicString<CharType>::ConstReverseIterator BasicString<CharType>::CREnd() const noexcept
	{
		return crend();
	}

	template <typename CharType>
	BasicString<CharType>::ReverseIterator BasicString<CharType>::RBegin() noexcept
	{
		return rbegin();
	}

	template <typename CharType>
	BasicString<CharType>::ReverseIterator BasicString<CharType>::REnd() noexcept
	{
		return rend();
	}

	template <typename CharType>
	CharType BasicString<CharType>::Front() const noexcept
	{
		return Buffer()[0];
	}

	template <typename CharType>
	CharType BasicString<CharType>::Back() const noexcept
	{
		return Buffer()[Size() - 1];
	}

	template <typename CharType>
	BasicString<CharType>::reference BasicString<CharType>::operator[](usize pos) noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscription out of range")
			return Buffer()[pos];
	}

	template <typename CharType>
	BasicString<CharType>::const_reference BasicString<CharType>::operator[](usize pos) const noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscription out of range")
			return Buffer()[pos];
	}

	template <typename CharType>
	usize BasicString<CharType>::Find(CharType ch, usize off) const noexcept
	{
		return ChFind(ch, off, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::Find(const BasicString& other, usize off) const noexcept
	{
		return Find(other.Data(), off, other.Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::Find(const CharType* str, usize off) const noexcept
	{
		return Find(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	usize BasicString<CharType>::Find(const CharType* str, usize off, usize len) const noexcept
	{
		return StrFind(str, off, len, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::Find(const std::basic_string<CharType>& str, usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::Find(std::basic_string_view<CharType> str, usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstOf(CharType ch, usize off) const noexcept
	{
		return ChFindFirstOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstOf(const BasicString& other, usize off) const noexcept
	{
		return FindFirstOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstOf(const CharType* str, usize off) const noexcept
	{
		return FindFirstOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstOf(const CharType* str, usize off, usize len) const noexcept
	{
		return StrFindFirstOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstOf(const std::basic_string<CharType>& str, usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstOf(std::basic_string_view<CharType> str, usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastOf(CharType ch, usize off) const noexcept
	{
		return ChFindLastOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastOf(const BasicString& other, usize off) const noexcept
	{
		return FindLastOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastOf(const CharType* str, usize off) const noexcept
	{
		return FindLastOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastOf(const CharType* str, usize off, usize len) const noexcept
	{
		return StrFindLastOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastOf(const std::basic_string<CharType>& str, usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastOf(std::basic_string_view<CharType> str, usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstNotOf(CharType ch, usize off) const noexcept
	{
		return ChFindFirstNotOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstNotOf(const BasicString& other, usize off) const noexcept
	{
		return FindFirstNotOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstNotOf(const CharType* str, usize off) const noexcept
	{
		return FindFirstNotOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstNotOf(const CharType* str, usize off, usize len) const noexcept
	{
		return StrFindFirstNotOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstNotOf(const std::basic_string<CharType>& str, usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindFirstNotOf(std::basic_string_view<CharType> str, usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastNotOf(CharType ch, usize off) const noexcept
	{
		return ChFindLastNotOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastNotOf(const BasicString& other, usize off) const noexcept
	{
		return FindLastNotOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastNotOf(const CharType* str, usize off) const noexcept
	{
		return FindLastNotOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastNotOf(const CharType* str, usize off, usize len) const noexcept
	{
		return StrFindLastNotOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastNotOf(const std::basic_string<CharType>& str, usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	usize BasicString<CharType>::FindLastNotOf(std::basic_string_view<CharType> str, usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	void BasicString<CharType>::ResetSizeAndEos(usize size) noexcept
	{
		if (IsHeapBuffer())
		{
			m_buffer.Heap.Size = size;
			m_buffer.Heap.Buffer[size] = CharType();
		}
		else
		{
			DEBUG_VERIFY_REPORT(size <= LocalStorageCapacity, "Size exceeds local storage capacity");
			m_buffer.Stack.Size = size;
			m_buffer.Stack.Buffer[size] = CharType();
		}
	}

	template <typename CharType>
	CharType* BasicString<CharType>::Buffer() noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap.Buffer : m_buffer.Stack.Buffer;
	}

	template <typename CharType>
	const CharType* BasicString<CharType>::Buffer() const noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap.Buffer : m_buffer.Stack.Buffer;
	}

	template <typename CharType>
	usize BasicString<CharType>::CalculateAllocateCapacity(usize requestCapacity, usize currentCapacity,
														   usize maxCapacity) noexcept
	{
		usize masked = requestCapacity | AllocateMask;
		if (masked > maxCapacity)
			return maxCapacity;

		if (currentCapacity > maxCapacity - currentCapacity / 2)
			return maxCapacity;

		return std::max(masked, currentCapacity + currentCapacity / 2);
	}

	template <typename CharType>
	void BasicString<CharType>::ReallocateHeapBuffer(usize capacity)
	{
		ReallocateHeapBufferWithCapacity(CalculateAllocateCapacity(capacity, m_buffer.Heap.Capacity, MaxStorageCapacity));
	}

	template<typename CharType>
	inline void BasicString<CharType>::ReallocateHeapBufferWithCapacity(usize capacity)
	{
		CharType* newBuffer = Memory::Allocate<CharType>(capacity + 1);

		if (m_buffer.Heap.Size > 0)
			memcpy(newBuffer, m_buffer.Heap.Buffer, m_buffer.Heap.Size * sizeof(CharType));

		Memory::Deallocate(m_buffer.Heap.Buffer, m_buffer.Heap.Capacity + 1);

		m_buffer.Heap.Buffer = newBuffer;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer[m_buffer.Heap.Size] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::DeallocateBuffer() noexcept
	{
		if (IsHeapBuffer())
			Memory::Deallocate(m_buffer.Heap.Buffer, m_buffer.Heap.Capacity + 1);
		InitSSOBuffer();
	}

	template <typename CharType>
	void BasicString<CharType>::InitSSOBuffer() noexcept
	{
		m_buffer.Stack.IsHeapBuffer = false;
		m_buffer.Stack.Size = 0;
		m_buffer.Stack.Buffer[0] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::InitHeapBuffer(usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, LocalStorageCapacity, MaxStorageCapacity);

		CharType* buffer = Memory::Allocate<CharType>(capacity + 1);

		m_buffer.Heap.IsHeapBuffer = true;
		m_buffer.Heap.Size = 0;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer = buffer;
		m_buffer.Heap.Buffer[0] = CharType();
	}

	template <typename CharType>
	bool BasicString<CharType>::IsHeapBuffer() const noexcept
	{
		return m_buffer.Stack.IsHeapBuffer;
	}

	template <typename CharType>
	void BasicString<CharType>::MoveToStack() noexcept
	{
		// 这一步对heapSize的检查应该在调用前进行，这里不做防御性检查
		usize heapSize = m_buffer.Heap.Size;
		usize heapCapacity = m_buffer.Heap.Capacity;

		CharType* heapBuffer = m_buffer.Heap.Buffer;

		// 如果需要constexpr路径，需要先初始化StackBuffer
		// InitSSOBuffer();

		if (heapSize > 0)
			memcpy(m_buffer.Stack.Buffer, heapBuffer, heapSize * sizeof(CharType));

		Memory::Deallocate(heapBuffer, heapCapacity + 1);

		m_buffer.Stack.IsHeapBuffer = false;
		m_buffer.Stack.Size = heapSize;

		// 可以通过在memcpy中复制size + 1个元素来复制结尾符，但是为了规范性，仍然采用手动设置结尾符
		m_buffer.Stack.Buffer[heapSize] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::MoveToHeap(usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, LocalStorageCapacity, MaxStorageCapacity);

		// Allocate不会构造对象，但是CharType是一个POD的字符类型，所以不需要构造函数
		// 如果后续需要设计constexpr路径，则需要在if consteval路径下构造每个元素
		CharType* buffer = Memory::Allocate<CharType>(capacity + 1);

		usize size = m_buffer.Stack.Size;

		if (size > 0)
			memcpy(buffer, m_buffer.Stack.Buffer, size * sizeof(CharType));

		m_buffer.Heap.IsHeapBuffer = true;
		m_buffer.Heap.Size = size;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer = buffer;

		// 可以通过在memcpy中复制size + 1个元素来复制结尾符，但是为了规范性，仍然采用手动设置结尾符
		m_buffer.Heap.Buffer[size] = CharType();
	}
}

template <>
struct std::formatter<PenFramework::PenEngine::String> : std::formatter<std::string>
{
	static auto format(const PenFramework::PenEngine::String& str, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{}", str.Data());
	}
};

template <typename CharType>
struct std::hash<PenFramework::PenEngine::BasicString<CharType>>
{
	static PenFramework::PenEngine::usize operator()(const PenFramework::PenEngine::BasicString<CharType>& str) noexcept
	{
		return std::hash<std::string_view>::operator()(std::string_view(str.Data(), str.Size()));
	}
};