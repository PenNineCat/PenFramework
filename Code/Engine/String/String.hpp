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

#include "../Utils/Iterator.hpp"
#include "../Common/Type.hpp"
#include "../DebugTools/Verify.hpp"
#include "../Exception/Exception.hpp"
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
		static constexpr U8 LocalStorageCapacity = 22ull / sizeof(CharType);
		static constexpr U8 AllocateMask = sizeof(CharType) <= 1 ? 15
			: sizeof(CharType) <= 2 ? 7
			: sizeof(CharType) <= 4 ? 3
			: sizeof(CharType) <= 8 ? 1
			: 0;
		static constexpr Usize MaxStorageCapacity = (std::numeric_limits<Usize>::max() >> 1) / sizeof(CharType);
		static constexpr Usize NPos = static_cast<Usize>(-1);

		using CharTraits = std::char_traits<CharType>;

		using value_type = CharType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = Usize;
		using difference_type = Isize;

		using is_contiguous = std::true_type;

		class Iterator : public StringConstIterator<CharType>
		{
		public:
			using iterator_category = std::contiguous_iterator_tag;
			using value_type = value_type;
			using difference_type = Isize;
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

		explicit BasicString(Usize capacity);
		BasicString(CharType ch, Usize count);
		explicit BasicString(const CharType* str);
		BasicString(const CharType* str, Usize length);

		template <typename SourceCharType>
		BasicString(const SourceCharType* str, Usize length, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		BasicString(const BasicString& other);
		BasicString(BasicString&& other) noexcept;

		BasicString(const BasicString& other, Usize len);

		explicit BasicString(BasicStringView<CharType> str) : BasicString(str.Data(), str.Size()) {}

		BasicString& operator=(const BasicString& str);
		BasicString& operator=(BasicStringView<CharType> str);
		BasicString& operator=(BasicString&& other) noexcept;
		BasicString& operator=(const CharType* str);
		BasicString& operator=(const std::basic_string<CharType>& str);
		BasicString& operator=(std::basic_string_view<CharType> str);
		BasicString& operator=(CharType ch);

		~BasicString() noexcept;

		BasicString& operator+=(const BasicString& str);
		BasicString& operator+=(const CharType* str);
		BasicString& operator+=(const std::basic_string<CharType>& str);
		BasicString& operator+=(std::basic_string_view<CharType> str);
		BasicString& operator+=(CharType ch);

		BasicString operator+(const BasicString& str);
		BasicString operator+(const CharType* str);
		BasicString operator+(const std::basic_string<CharType>& str);
		BasicString operator+(std::basic_string_view<CharType> str);
		BasicString operator+(CharType ch);

		bool operator==(const BasicString& str) const noexcept;
		bool operator==(BasicStringView<CharType> str) const noexcept;
		bool operator==(const CharType* str) const noexcept;
		bool operator==(const std::basic_string<CharType>& str) const noexcept;
		bool operator==(std::basic_string_view<CharType> str) const noexcept;
		bool operator==(CharType ch) const noexcept;

		Usize Size() const noexcept;
		Usize Capacity() const noexcept;
		bool Empty() const noexcept;

		void Reserve(Usize newCapacity);
		void ReserveExtra(Usize extraCapacity);
		void Resize(Usize size, CharType ch = CharType());
		void ShrinkToFit();

		CharType* Data() noexcept;
		const CharType* Data() const noexcept;

		BasicString Substr(Usize off = 0, Usize len = NPos) const;
		BasicString Right(Usize len) const;
		BasicString Left(Usize len) const;

		std::vector<BasicString> Split(CharType ch) const;

		void Clear() noexcept;

		template <typename TargetCharType>
		std::basic_string<TargetCharType> ToStdString(boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method) const;

		/* implicit */ operator BasicStringView<CharType>() const noexcept
		{
			return BasicStringView<CharType>(Data(), Size());
		}

		void Append(const BasicString& other);
		void Append(const CharType* str);
		void Append(const CharType* str, Usize len);
		void Append(CharType ch);
		void Append(CharType ch, Usize count);
		void Append(const std::basic_string<CharType>& str);
		void Append(std::basic_string_view<CharType> str);

		Iterator Remove(Usize off = 0) noexcept;
		Iterator Remove(ConstIterator it, Usize count = 1);
		Iterator Remove(ConstIterator begin, ConstIterator end);
		Iterator Remove(CharType ch, Usize off = 0);

		template <typename SourceCharType>
		void ConvertAndAppend(const BasicString<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(const SourceCharType* str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		 // @todo 这个地方需要一次Benchmark，以判断是临时缓冲区性能更高，还是双循环性能更高
		template <typename SourceCharType>
		void ConvertAndAppend(const SourceCharType* str, Usize len, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndAppend(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		bool IsValidUnicodeFormat() const noexcept;

		void PushBack(const BasicString& other);
		void PushBack(const CharType* str);
		void PushBack(const CharType* str, Usize len);
		void PushBack(CharType ch, Usize count);
		void PushBack(CharType ch);
		void PushBack(const std::basic_string<CharType>& str);
		void PushBack(std::basic_string_view<CharType> str);

		template <typename SourceCharType>
		void ConvertAndPushBack(const SourceCharType& other, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushBack(const SourceCharType* str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushBack(const SourceCharType* str, Usize len, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushBack(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushBack(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		void PushFront(const BasicString& other);
		void PushFront(const CharType* str);
		void PushFront(const CharType* str, Usize len);
		void PushFront(CharType ch, Usize count = 1);
		void PushFront(const std::basic_string<CharType>& str);
		void PushFront(std::basic_string_view<CharType> str);

		template <typename SourceCharType>
		void ConvertAndPushFront(const BasicString<SourceCharType>& other, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(const SourceCharType* str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(const SourceCharType* str, Usize len, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);
		template <typename SourceCharType>
		void ConvertAndPushFront(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how = boost::locale::conv::method_type::default_method);

		template <typename TargetCharType>
		BasicString<TargetCharType> ConvertTo(boost::locale::conv::method_type how = boost::locale::conv::method_type::how);

		template <typename T> requires (std::is_arithmetic_v<T> && !std::is_same_v<T, CharType>)
			void Append(T v);

		bool Contain(CharType ch, Usize off = 0) const noexcept;
		bool Contain(const CharType* str, Usize off = 0) const noexcept;
		bool Contain(const CharType* str, Usize off, Usize len) const noexcept;
		bool Contain(const BasicString& other, Usize off = 0) const noexcept;
		bool Contain(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		bool Contain(std::basic_string_view<CharType> str, Usize off = 0) const noexcept;

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

		reference operator[](Usize pos) noexcept;
		const_reference operator[](Usize pos) const noexcept;

		Usize Find(CharType ch, Usize off = 0) const noexcept;
		Usize Find(const BasicString& other, Usize off = 0) const noexcept;
		Usize Find(const CharType* str, Usize off = 0) const noexcept;
		Usize Find(const CharType* str, Usize off, Usize len) const noexcept;
		Usize Find(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		Usize Find(std::basic_string_view<CharType> str, Usize off = 0) const noexcept;

		Usize FindFirstOf(CharType ch, Usize off = 0) const noexcept;
		Usize FindFirstOf(const BasicString& other, Usize off = 0) const noexcept;
		Usize FindFirstOf(const CharType* str, Usize off = 0) const noexcept;
		Usize FindFirstOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindFirstOf(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		Usize FindFirstOf(std::basic_string_view<CharType> str, Usize off = 0) const noexcept;

		Usize FindLastOf(CharType ch, Usize off = NPos) const noexcept;
		Usize FindLastOf(const BasicString& other, Usize off = NPos) const noexcept;
		Usize FindLastOf(const CharType* str, Usize off = NPos) const noexcept;
		Usize FindLastOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindLastOf(const std::basic_string<CharType>& str, Usize off = NPos) const noexcept;
		Usize FindLastOf(std::basic_string_view<CharType> str, Usize off = NPos) const noexcept;

		Usize FindFirstNotOf(CharType ch, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(const BasicString& other, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(const CharType* str, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindFirstNotOf(const std::basic_string<CharType>& str, Usize off = 0) const noexcept;
		Usize FindFirstNotOf(std::basic_string_view<CharType> str, Usize off = 0) const noexcept;

		Usize FindLastNotOf(CharType ch, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(const BasicString& other, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(const CharType* str, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(const CharType* str, Usize off, Usize len) const noexcept;
		Usize FindLastNotOf(const std::basic_string<CharType>& str, Usize off = NPos) const noexcept;
		Usize FindLastNotOf(std::basic_string_view<CharType> str, Usize off = NPos) const noexcept;
	protected:
		void ResetSizeAndEos(Usize size) noexcept;

		CharType* Buffer() noexcept;
		const CharType* Buffer() const noexcept;

		static Usize CalculateAllocateCapacity(Usize requestCapacity, Usize currentCapacity, Usize maxCapacity) noexcept;

		void ReallocateHeapBuffer(Usize capacity);
		void ReallocateHeapBufferByCapacity(Usize capacity);
		void CleanAndReBuild(const CharType* str, Usize len);
		void DeallocateBuffer() noexcept;

		void InternalRemove(Usize off, Usize count) noexcept;

		void InitSSOBuffer() noexcept;
		void InitHeapBuffer(Usize capacity);

		bool IsHeapBuffer() const noexcept;

		void MoveToStack() noexcept;
		void MoveToHeap(Usize capacity);

		union Buffer
		{
			CharType Stack[LocalStorageCapacity + 1];
			CharType* Heap;
		} m_buffer;

		Usize m_size;
		Usize m_capacity;
	};

	using String = BasicString<Ch>;
	using U32String = BasicString<Ch32>;

	template <typename CharType>
	BasicString<CharType>::BasicString() noexcept
	{
		InitSSOBuffer();
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(Usize capacity)
	{
		if (capacity <= LocalStorageCapacity)
			InitSSOBuffer();
		else
			InitHeapBuffer(capacity);
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(CharType ch, Usize count)
	{
		if (count <= LocalStorageCapacity)
		{
			InitSSOBuffer();

			m_size = count;

			CharTraits::assign(m_buffer.Stack, count, ch);
			m_buffer.Stack[count] = CharType();
		}
		else
		{
			InitHeapBuffer(count);

			m_size = count;

			CharTraits::assign(m_buffer.Heap, count, ch);
			m_buffer.Heap[count] = CharType();
		}
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const CharType* str) : BasicString(str, CharTraits::length(str))
	{
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const CharType* str, Usize length)
	{
		if (length <= LocalStorageCapacity)
		{
			InitSSOBuffer();

			CharTraits::copy(m_buffer.Stack, str, length);

			m_size = length;
			m_buffer.Stack[length] = CharType();
		}
		else
		{
			InitHeapBuffer(length);

			CharTraits::copy(m_buffer.Heap, str, length);

			m_size = length;
			m_buffer.Heap[length] = CharType();
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
		std::swap(m_size,other.m_size);
		std::swap(m_capacity,other.m_capacity);
	}

	template <typename CharType>
	BasicString<CharType>::BasicString(const BasicString& other, Usize len) : BasicString(other.Data(), std::min(len, other.Size()))
	{
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(const BasicString& str)
	{
		CleanAndReBuild(str.Data(), str.Size());
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(BasicStringView<CharType> str)
	{
		CleanAndReBuild(str.Data(), str.Size());
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
		CleanAndReBuild(str, CharTraits::length(str));
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(const std::basic_string<CharType>& str)
	{
		CleanAndReBuild(str.data(), str.size());
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(std::basic_string_view<CharType> str)
	{
		CleanAndReBuild(str.data(), str.size());
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator=(CharType ch)
	{
		DeallocateBuffer();
		m_buffer.Stack[0] = ch;
		m_size = 1;
		m_buffer.Stack[1] = CharType();
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>::~BasicString() noexcept
	{
		DeallocateBuffer();
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(const BasicString& str)
	{
		Append(str);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(const CharType* str)
	{
		Append(str);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(const std::basic_string<CharType>& str)
	{
		Append(str);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(std::basic_string_view<CharType> str)
	{
		Append(str);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType>& BasicString<CharType>::operator+=(CharType ch)
	{
		Append(ch);
		return *this;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(const BasicString& str)
	{
		BasicString tmp = *this;
		tmp.Append(str);
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(const CharType* str)
	{
		BasicString tmp = *this;
		tmp.Append(str);
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(const std::basic_string<CharType>& str)
	{
		BasicString tmp = *this;
		tmp.Append(str);
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(std::basic_string_view<CharType> str)
	{
		BasicString tmp = *this;
		tmp.Append(str);
		return tmp;
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::operator+(CharType ch)
	{
		BasicString tmp = *this;
		tmp.Append(ch);
		return tmp;
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(const BasicString& str) const noexcept
	{
		return Size() == str.Size() && CharTraits::compare(Data(), str.Data(), Size()) == 0;
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(BasicStringView<CharType> str) const noexcept
	{
		return Size() == str.Size() && CharTraits::compare(Data(), str.Data(), Size()) == 0;
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(const CharType* str) const noexcept
	{
		return CharTraits::compare(Data(), str, Size()) == 0 && str[Size()] == CharType();
	}

	template <typename CharType>
	bool BasicString<CharType>::operator==(const std::basic_string<CharType>& str) const noexcept
	{
		return str == Data();
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
	Usize BasicString<CharType>::Size() const noexcept
	{
		return m_size;
	}

	template <typename CharType>
	Usize BasicString<CharType>::Capacity() const noexcept
	{
		return m_capacity;
	}

	template <typename CharType>
	bool BasicString<CharType>::Empty() const noexcept
	{
		return Size() == 0;
	}

	template <typename CharType>
	void BasicString<CharType>::Reserve(Usize newCapacity)
	{
		if (newCapacity <= m_capacity)
			return;

		if (IsHeapBuffer())
			ReallocateHeapBuffer(newCapacity);
		else
			MoveToHeap(newCapacity);
	}

	template <typename CharType>
	void BasicString<CharType>::ReserveExtra(Usize extraCapacity)
	{
		Reserve(Size() + extraCapacity);
	}

	template <typename CharType>
	void BasicString<CharType>::Resize(Usize size, CharType ch)
	{
		if (Usize currentSize = Size(); size > currentSize)
		{
			ReserveExtra(size);
			CharType* buffer = Buffer();

			CharTraits::assign(buffer + currentSize, size, ch);
			buffer[currentSize + size] =  CharType();
		}

		m_size = size;
	}

	template <typename CharType>
	void BasicString<CharType>::ShrinkToFit()
	{
		if (IsHeapBuffer())
		{
			if (m_size <= LocalStorageCapacity)
				MoveToStack();
			else if (m_size < m_capacity)
				ReallocateHeapBufferByCapacity(m_size);
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
	BasicString<CharType> BasicString<CharType>::Substr(Usize off, Usize len) const
	{
		Usize size = Size();

		if (off >= size)
			return {};

		len = std::min(len, size - off);

		return BasicString(Buffer() + off, len);
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::Right(Usize len) const
	{
		return Substr(Size() - len, len);
	}

	template <typename CharType>
	BasicString<CharType> BasicString<CharType>::Left(Usize len) const
	{
		return Substr(0, len);
	}

	template <typename CharType>
	std::vector<BasicString<CharType>> BasicString<CharType>::Split(CharType ch) const
	{
		Usize off = 0;
		Usize st = 0;
		std::vector<BasicString> res;
		while (off != NPos)
		{
			off = Find(ch, off);
			res.emplace_back(Substr(st, off));
			st = off;
		}

		return res;
	}

	template <typename CharType>
	void BasicString<CharType>::Clear() noexcept
	{
		ResetSizeAndEos(0);
	}

	template <typename CharType>
	template <typename TargetCharType>
	std::basic_string<TargetCharType> BasicString<CharType>::ToStdString(boost::locale::conv::method_type how) const
	{
		if constexpr (std::same_as<CharType, TargetCharType>)
			return std::basic_string<TargetCharType>(Data(), Data() + Size());
		else
			return boost::locale::conv::utf_to_utf<TargetCharType>(Data(), Data() + Size(), how);
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const BasicString& other)
	{
		Append(other.Buffer(), other.Size());
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const CharType* str)
	{
		return Append(str, CharTraits::length(str));
	}

	template <typename CharType>
	void BasicString<CharType>::Append(const CharType* str, Usize len)
	{
		if (len == 0)
			return;

		ReserveExtra(len);
		Usize size = Size();
		CharType* buffer = Buffer();
		memcpy(buffer + size, str, len);

		ResetSizeAndEos(size + len);
	}

	template <typename CharType>
	void BasicString<CharType>::Append(CharType ch)
	{
		if (m_size < m_capacity)
		{
			CharType* buffer = Buffer();

			buffer[m_size] = ch;
			m_size += 1;
			buffer[m_size] = CharType();

			return;
		}

		ReserveExtra(1);

		CharType* buffer = Buffer();

		buffer[m_size] = CharType();
		m_size += 1;
		buffer[m_size] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::Append(CharType ch, Usize count)
	{
		if (count == 0)
			return;

		ReserveExtra(count);

		CharType* buffer = Buffer();
		Usize size = Size();

		CharTraits::assign(buffer + size, count, ch);

		m_size += count;
		buffer[m_size] = CharType();
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

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::Remove(Usize off) noexcept
	{
		DEBUG_VERIFY_REPORT(off < Size(), "Invalid Position")
			ResetSizeAndEos(off);
		return Begin();
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::Remove(ConstIterator it, Usize count)
	{
		DEBUG_VERIFY_REPORT(it.m_ptr == Buffer(), "String iterator incompatible")
			Usize off = static_cast<Usize>(it.m_ptr - Data());
		DEBUG_VERIFY_REPORT(off < Size(), "Invalid Position")
			InternalRemove(off, count);
		return Begin() + off;
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::Remove(ConstIterator begin, ConstIterator end)
	{
		DEBUG_VERIFY_REPORT(begin.m_ptr == Buffer(), "String iterator incompatible")
			DEBUG_VERIFY_REPORT(end.m_ptr == Buffer(), "String iterator incompatible")

			Usize off = static_cast<Usize>(begin.m_ptr - Data());
		DEBUG_VERIFY_REPORT(off < Size(), "Invalid Position")
			Usize len = end - begin;
		InternalRemove(off, len);

		return Begin() + off;
	}

	template <typename CharType>
	BasicString<CharType>::Iterator BasicString<CharType>::Remove(CharType ch, Usize off)
	{
		Usize begin = Find(ch, off);
		if (begin != NPos)
			InternalRemove(begin, 1);
		return Begin() + begin;
	}

	template<typename CharType>
	template<typename SourceCharType>
	BasicString<CharType>::BasicString(const SourceCharType* str, Usize length, boost::locale::conv::method_type how)
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
		ConvertAndAppend(str, std::char_traits<SourceCharType>::length(str), how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndAppend(const SourceCharType* str, Usize len, boost::locale::conv::method_type how)
	{
		if (str == nullptr || len == 0)
			return;

		auto begin = str;
		auto end = str + len;

		BasicString result;
		result.Reserve(len);
		auto inserter = BackInserter(result);
		while (begin != end) {
			const boost::locale::utf::code_point c = boost::locale::utf::utf_traits<SourceCharType>::decode(begin, end);
			if (c == boost::locale::utf::illegal || c == boost::locale::utf::incomplete) {
				if (how == boost::locale::conv::method_type::stop)
					throw boost::locale::conv::conversion_error();
			}
			else
			{
				boost::locale::utf::utf_traits<CharType>::encode(c, inserter);
			}

		}

		Append(result);
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
	bool BasicString<CharType>::IsValidUnicodeFormat() const noexcept
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
	void BasicString<CharType>::PushBack(const CharType* str, Usize len)
	{
		Append(str, len);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(CharType ch, Usize count)
	{
		Append(ch, count);
	}

	template <typename CharType>
	void BasicString<CharType>::PushBack(CharType ch)
	{
		Append(ch);
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
	void BasicString<CharType>::ConvertAndPushBack(const SourceCharType& other, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(other, how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const SourceCharType* str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str, how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const SourceCharType* str, Usize len, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str, len, how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(const std::basic_string<SourceCharType>& str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str, how);
	}

	template <typename CharType>
	template <typename SourceCharType>
	void BasicString<CharType>::ConvertAndPushBack(std::basic_string_view<SourceCharType> str, boost::locale::conv::method_type how)
	{
		ConvertAndAppend(str, how);
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const BasicString& other)
	{
		PushFront(other.Data(), other.Size());
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const CharType* str)
	{
		PushFront(str, CharTraits::length(str));
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(const CharType* str, Usize len)
	{
		if (str == nullptr || len == 0)
			return;

		ReserveExtra(len);

		CharType* buffer = Buffer();
		Usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + len);

		memcpy(buffer, str, len * sizeof(CharType));

		ResetSizeAndEos(size + len);
	}

	template <typename CharType>
	void BasicString<CharType>::PushFront(CharType ch, Usize count)
	{
		if (count == 0)
			return;

		ReserveExtra(count);

		CharType* buffer = Buffer();
		Usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + count);

		m_size += count;
		CharTraits::assign(buffer, count, ch);
		buffer[m_size] = CharType();
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
	void BasicString<CharType>::ConvertAndPushFront(const SourceCharType* str, Usize len, boost::locale::conv::method_type how)
	{
		// 预先处理
		if (str == nullptr || len == 0)
			return;

		Usize requiredLength = 0;

		const SourceCharType* currentStartPosition = str;
		const SourceCharType* endPosition = str + len;

		// 计算转换后需要分配字符串长度
		while (currentStartPosition != endPosition)
		{
			const boost::locale::utf::code_point c = boost::locale::utf::utf_traits<SourceCharType>::decode(currentStartPosition, endPosition);
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
		Usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + requiredLength);

		// 重定向到开头
		currentStartPosition = str;

		// 解码串
		while (currentStartPosition != endPosition) {
			const boost::locale::utf::code_point c = boost::locale::utf::utf_traits<SourceCharType>::decode(currentStartPosition, endPosition);
			// 这里不会有异常情况，因为如果有，计算长度时就已经抛出
			boost::locale::utf::utf_traits<CharType>::encode(c, buffer++);
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
	template <typename TargetCharType>
	BasicString<TargetCharType> BasicString<CharType>::ConvertTo(boost::locale::conv::method_type how)
	{
		BasicString<TargetCharType> res;
		res.ConvertAndAppend(*this, how);
		return *this;
	}

	template <typename CharType>
	template <typename T> requires (std::is_arithmetic_v<T> && !std::is_same_v<T, CharType>)
		void BasicString<CharType>::Append(T v)
	{
		constexpr Usize bufferLength = std::numeric_limits<T>::digits10 + 2;
		char buffer[bufferLength] = {};
		auto [ec, ptr] = std::to_chars(buffer, bufferLength, v);

		DEBUG_VERIFY_REPORT(ec == std::errc(), "buffer length must be large enough to accomodate the value")
			Append(buffer, static_cast<Usize>(ptr - buffer));
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(CharType ch, Usize off) const noexcept
	{
		return Find(ch, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const BasicString& other, Usize off) const noexcept
	{
		return Find(other, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const CharType* str, Usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const CharType* str, Usize off, Usize len) const noexcept
	{
		return Find(str, off, len) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename CharType>
	bool BasicString<CharType>::Contain(std::basic_string_view<CharType> str, Usize off) const noexcept
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
	BasicString<CharType>::reference BasicString<CharType>::operator[](Usize pos) noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscription out of range")
			return Buffer()[pos];
	}

	template <typename CharType>
	BasicString<CharType>::const_reference BasicString<CharType>::operator[](Usize pos) const noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscription out of range")
			return Buffer()[pos];
	}

	template <typename CharType>
	Usize BasicString<CharType>::Find(CharType ch, Usize off) const noexcept
	{
		return ChFind(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::Find(const BasicString& other, Usize off) const noexcept
	{
		return Find(other.Data(), off, other.Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::Find(const CharType* str, Usize off) const noexcept
	{
		return Find(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicString<CharType>::Find(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFind(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::Find(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::Find(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstOf(CharType ch, Usize off) const noexcept
	{
		return ChFindFirstOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstOf(const BasicString& other, Usize off) const noexcept
	{
		return FindFirstOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstOf(const CharType* str, Usize off) const noexcept
	{
		return FindFirstOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindFirstOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastOf(CharType ch, Usize off) const noexcept
	{
		return ChFindLastOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastOf(const BasicString& other, Usize off) const noexcept
	{
		return FindLastOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastOf(const CharType* str, Usize off) const noexcept
	{
		return FindLastOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindLastOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstNotOf(CharType ch, Usize off) const noexcept
	{
		return ChFindFirstNotOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstNotOf(const BasicString& other, Usize off) const noexcept
	{
		return FindFirstNotOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstNotOf(const CharType* str, Usize off) const noexcept
	{
		return FindFirstNotOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstNotOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindFirstNotOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstNotOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindFirstNotOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastNotOf(CharType ch, Usize off) const noexcept
	{
		return ChFindLastNotOf(ch, off, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastNotOf(const BasicString& other, Usize off) const noexcept
	{
		return FindLastNotOf(other.Data(), off, other.Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastNotOf(const CharType* str, Usize off) const noexcept
	{
		return FindLastNotOf(str, off, CharTraits::length(str));
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastNotOf(const CharType* str, Usize off, Usize len) const noexcept
	{
		return StrFindLastNotOf(str, off, len, Data(), Size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastNotOf(const std::basic_string<CharType>& str, Usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	Usize BasicString<CharType>::FindLastNotOf(std::basic_string_view<CharType> str, Usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename CharType>
	void BasicString<CharType>::ResetSizeAndEos(Usize size) noexcept
	{
		DEBUG_VERIFY_REPORT(size <= m_capacity, "Size exceeds storage capacity")
			m_size = size;
		Buffer()[size] = CharType();
	}

	template <typename CharType>
	CharType* BasicString<CharType>::Buffer() noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap : m_buffer.Stack;
	}

	template <typename CharType>
	const CharType* BasicString<CharType>::Buffer() const noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap : m_buffer.Stack;
	}

	template <typename CharType>
	Usize BasicString<CharType>::CalculateAllocateCapacity(Usize requestCapacity, Usize currentCapacity,
														   Usize maxCapacity) noexcept
	{
		Usize masked = requestCapacity | AllocateMask;
		if (masked > maxCapacity)
			return maxCapacity;

		if (currentCapacity > maxCapacity - currentCapacity / 2)
			return maxCapacity;

		return std::max(masked, currentCapacity + currentCapacity / 2);
	}

	template <typename CharType>
	void BasicString<CharType>::ReallocateHeapBuffer(Usize capacity)
	{
		ReallocateHeapBufferByCapacity(CalculateAllocateCapacity(capacity, m_capacity, MaxStorageCapacity));
	}

	template<typename CharType>
	void BasicString<CharType>::ReallocateHeapBufferByCapacity(Usize capacity)
	{
		CharType* newBuffer = Memory::Allocate<CharType>(capacity + 1);

		// 不需要m_size > 0判断，memcpy会检查，并且就算为0并且触发了复制，其也不会造成副作用，因为m_buffer.Heap是有效的
		CharTraits::copy(newBuffer, m_buffer.Heap, m_size);

		Memory::Deallocate(m_buffer.Heap, m_capacity + 1);

		m_buffer.Heap = newBuffer;
		m_capacity = capacity;
		m_buffer.Heap[m_size] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::CleanAndReBuild(const CharType* str, Usize len)
	{
		DeallocateBuffer();

		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			CharTraits::copy(m_buffer.Stack, str, len);
			m_size = len;
			m_buffer.Stack[m_size] = CharType();
		}
		else
		{
			InitHeapBuffer(len);
			CharTraits::copy(m_buffer.Heap, str, len);
			m_size = len;
			m_buffer.Heap[m_size] = CharType();
		}
	}

	template <typename CharType>
	void BasicString<CharType>::DeallocateBuffer() noexcept
	{
		if (IsHeapBuffer())
			Memory::Deallocate(m_buffer.Heap, m_capacity + 1);
		InitSSOBuffer();
	}

	template <typename CharType>
	void BasicString<CharType>::InternalRemove(Usize off, Usize count) noexcept
	{
		Usize oldSize = Size();

		CharType* buffer = Buffer();

		count = std::min(count, oldSize - off);

		CharType* startPosition = buffer + off;
		Usize newSize = oldSize - count;

		CharTraits::move(startPosition, startPosition + count, newSize - off + 1);

		m_size = newSize;
		buffer[newSize] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::InitSSOBuffer() noexcept
	{
		m_capacity = LocalStorageCapacity;
		m_size = 0;
		m_buffer.Stack[0] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::InitHeapBuffer(Usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, LocalStorageCapacity, MaxStorageCapacity);

		CharType* buffer = Memory::Allocate<CharType>(capacity + 1);

		m_size = 0;
		m_capacity = capacity;
		m_buffer.Heap = buffer;
		m_buffer.Heap[0] = CharType();
	}

	template <typename CharType>
	bool BasicString<CharType>::IsHeapBuffer() const noexcept
	{
		return m_capacity > LocalStorageCapacity;
	}

	template <typename CharType>
	void BasicString<CharType>::MoveToStack() noexcept
	{
		// 这一步对m_size的检查应该在调用前进行，这里不做防御性检查
		CharType* heapBuffer = m_buffer.Heap;

		// 如果需要constexpr路径，需要先初始化StackBuffer
		// 并且InitSSOBuffer()需要在if consteval路径下构造每个元素

		CharTraits::copy(m_buffer.Stack, heapBuffer, m_size);

		Memory::Deallocate(heapBuffer, m_capacity + 1);

		m_capacity = LocalStorageCapacity;

		// 可以通过在memcpy中复制size + 1个元素来复制结尾符，但是为了规范性，仍然采用手动设置结尾符
		m_buffer.Stack[m_size] = CharType();
	}

	template <typename CharType>
	void BasicString<CharType>::MoveToHeap(Usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, LocalStorageCapacity, MaxStorageCapacity);

		// Allocate不会构造对象，但是CharType是一个POD的字符类型，所以不需要构造函数
		// @todo 对于将来可能得constexpr路径，需要在if consteval路径下构造每个元素
		CharType* buffer = Memory::Allocate<CharType>(capacity + 1);

		CharTraits::copy(buffer, m_buffer.Stack, m_size);

		m_capacity = capacity;
		m_buffer.Heap = buffer;

		// 可以通过在memcpy中复制size + 1个元素来复制结尾符，但是为了规范性，仍然采用手动设置结尾符
		m_buffer.Heap[m_size] = CharType();
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
	static PenFramework::PenEngine::Usize operator()(const PenFramework::PenEngine::BasicString<CharType>& str) noexcept
	{
		return std::hash<std::string_view>::operator()(std::string_view(str.Data(), str.Size()));
	}
};