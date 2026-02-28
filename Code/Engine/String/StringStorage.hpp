// File /Engine/String/StringStorage.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <charconv>

#include "../Common/Type.hpp"
#include "../DebugTools/Verify.hpp"
#include "../Math/MathFunction.hpp"
#include "../Memory/Memory.hpp"
#include "../Utils/Concept.hpp"

namespace PenFramework::PenEngine
{
	template <typename StorageType>
	class StringStorage
	{
	public:
		static constexpr u8 LocalStorageCapacity = 22ull / sizeof(StorageType);
		static constexpr u8 AllocateMask = sizeof(StorageType) <= 1 ? 15
			: sizeof(StorageType) <= 2 ? 7
			: sizeof(StorageType) <= 4 ? 3
			: sizeof(StorageType) <= 8 ? 1
			: 0;
		static constexpr usize MaxStorageCapacity = Pow(2ull, sizeof(usize) * BitsPerBytes - 1) - 1;
		static constexpr usize NPos = static_cast<usize>(-1);

		using CharTraits = std::char_traits<StorageType>;

		using value_type = StorageType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = usize;
		using difference_type = isize;

		class ConstIterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = value_type;
			using difference_type = difference_type;
			using pointer = const_pointer;
			using reference = const_reference;

			ConstIterator() noexcept = default;
			explicit ConstIterator(pointer ptr) noexcept : m_ptr(ptr) {}

			reference operator*() const noexcept { return *m_ptr; }
			pointer operator->() const noexcept { return m_ptr; }

			ConstIterator& operator++() noexcept { ++m_ptr; return *this; }
			ConstIterator operator++(int) noexcept { ConstIterator tmp = *this; ++(*this); return tmp; }

			ConstIterator& operator--() noexcept { --m_ptr; return *this; }
			ConstIterator operator--(int) noexcept { ConstIterator tmp = *this; --(*this); return tmp; }

			friend ConstIterator operator+(ConstIterator it, difference_type n) noexcept {
				it += n; return it;
			}
			friend ConstIterator operator+(difference_type n, ConstIterator it) noexcept {
				it += n; return it;
			}
			friend ConstIterator operator-(ConstIterator it, difference_type n) noexcept {
				it -= n; return it;
			}

			friend difference_type operator-(ConstIterator lhs, ConstIterator rhs) noexcept {
				return lhs.m_ptr - rhs.m_ptr;
			}

			reference operator[](difference_type n) const noexcept { return m_ptr[n]; }

			bool operator==(const ConstIterator& other) const noexcept = default;
			bool operator<=>(const ConstIterator& other) const noexcept = default;

		protected:
			pointer m_ptr;
		};

		class Iterator : public ConstIterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = value_type;
			using difference_type = difference_type;
			using pointer = pointer;
			using reference = reference;

			constexpr Iterator() noexcept : ConstIterator() {}
			explicit constexpr Iterator(pointer ptr) noexcept : ConstIterator(ptr) {}

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

			friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) noexcept {
				return lhs.m_ptr - rhs.m_ptr;
			}

			friend difference_type operator-(const Iterator& lhs, const ConstIterator& rhs) noexcept {
				return lhs.m_ptr - rhs.m_ptr;
			}

			reference operator[](difference_type n) const noexcept { return const_cast<reference>(this->m_ptr[n]); }

			bool operator==(const Iterator&) const noexcept = default;
			bool operator==(const ConstIterator& other) const noexcept { return this->m_ptr == other.m_ptr; }

			bool operator<=>(const Iterator&) const noexcept = default;
			bool operator<=>(const ConstIterator& other) const noexcept { return this->m_ptr <=> other.m_ptr; }
		};

		using ReverseIterator = std::reverse_iterator<Iterator>;
		using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

		StringStorage() noexcept;

		explicit StringStorage(usize capacity);
		StringStorage(StorageType ch, usize count);
		explicit StringStorage(const StorageType* str);
		StringStorage(const StorageType* str, usize length);

		StringStorage(const StringStorage& other);
		StringStorage(StringStorage&& other) noexcept;

		StringStorage(const StringStorage& other, usize len);

		StringStorage& operator=(const StringStorage& other);
		StringStorage& operator=(StringStorage&& other) noexcept;
		StringStorage& operator=(const StorageType* str);
		StringStorage& operator=(const std::basic_string<StorageType>& str);
		StringStorage& operator=(std::basic_string_view<StorageType> str);
		StringStorage& operator=(StorageType ch);

		~StringStorage() noexcept;

		StringStorage& operator+=(const StringStorage& other);
		StringStorage& operator+=(const StorageType* str);
		StringStorage& operator+=(const std::basic_string<StorageType>& str);
		StringStorage& operator+=(std::basic_string_view<StorageType> str);
		StringStorage& operator+=(StorageType ch);

		StringStorage operator+(const StringStorage& other);
		StringStorage operator+(const StorageType* str);
		StringStorage operator+(const std::basic_string<StorageType>& str);
		StringStorage operator+(std::basic_string_view<StorageType> str);
		StringStorage operator+(StorageType ch);

		bool operator==(const StringStorage& other) const noexcept;
		bool operator==(const StorageType* str) const noexcept;
		bool operator==(const std::basic_string<StorageType>& str) const noexcept;
		bool operator==(std::basic_string_view<StorageType> str) const noexcept;
		bool operator==(StorageType ch) const noexcept;

		usize Size() const noexcept;
		usize Capacity() const noexcept;
		bool Empty() const noexcept;

		void Reserve(usize newCapacity);
		void ReserveExtra(usize extraCapacity);
		void Resize(usize size, StorageType ch = StorageType());
		void ShrinkToFit();

		const StorageType* Data() noexcept;
		const StorageType* Data() const noexcept;

		void Clear() noexcept;

		void Append(const StringStorage& other);
		void Append(const StorageType* str);
		void Append(const StorageType* str, usize len);
		void Append(StorageType ch, usize count = 1);
		void Append(const std::basic_string<StorageType>& str);
		void Append(std::basic_string_view<StorageType>& str);

		void PushBack(const StringStorage& other);
		void PushBack(const StorageType* str);
		void PushBack(const StorageType* str, usize len);
		void PushBack(StorageType ch, usize count = 1);
		void PushBack(const std::basic_string<StorageType>& str);
		void PushBack(std::basic_string_view<StorageType>& str);

		void PushFront(const StringStorage& other);
		void PushFront(const StorageType* str);
		void PushFront(const StorageType* str, usize len);
		void PushFront(StorageType ch, usize count = 1);
		void PushFront(const std::basic_string<StorageType>& str);
		void PushFront(std::basic_string_view<StorageType>& str);

		template <typename T> requires (std::is_arithmetic_v<T> && !std::is_same_v<T, StorageType>)
			void Append(T v);

		bool Contain(StorageType ch, usize off = 0) const noexcept;
		bool Contain(const StringStorage& other, usize off = 0) const noexcept;
		bool Contain(const StorageType* str, usize off = 0) const noexcept;
		bool Contain(const std::basic_string<StorageType>& str, usize off = 0) const noexcept;
		bool Contain(std::basic_string_view<StorageType>& str, usize off = 0) const noexcept;

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

		StorageType Front() const noexcept;
		StorageType Back() const noexcept;

		reference operator[](usize pos) noexcept;
		const_reference operator[](usize pos) const noexcept;

		usize Find(StorageType ch, usize off = 0) const noexcept;
		usize Find(const StringStorage& other, usize off = 0) const noexcept;
		usize Find(const StorageType* str, usize off = 0) const noexcept;
		usize Find(const StorageType* str, usize off, usize len) const noexcept;
		usize Find(const std::basic_string<StorageType>& str, usize off = 0) const noexcept;
		usize Find(std::basic_string_view<StorageType>& str, usize off = 0) const noexcept;

		usize FindFirstOf(StorageType ch, usize off = 0) const noexcept;
		usize FindFirstOf(const StringStorage& other, usize off = 0) const noexcept;
		usize FindFirstOf(const StorageType* str, usize off = 0) const noexcept;
		usize FindFirstOf(const StorageType* str, usize off, usize len) const noexcept;
		usize FindFirstOf(const std::basic_string<StorageType>& str, usize off = 0) const noexcept;
		usize FindFirstOf(std::basic_string_view<StorageType> str, usize off = 0) const noexcept;

		usize FindLastOf(StorageType ch, usize off = NPos) const noexcept;
		usize FindLastOf(const StringStorage& other, usize off = NPos) const noexcept;
		usize FindLastOf(const StorageType* str, usize off = NPos) const noexcept;
		usize FindLastOf(const StorageType* str, usize off, usize len) const noexcept;
		usize FindLastOf(const std::basic_string<StorageType>& str, usize off = NPos) const noexcept;
		usize FindLastOf(std::basic_string_view<StorageType> str, usize off = NPos) const noexcept;

		usize FindFirstNotOf(StorageType ch, usize off = 0) const noexcept;
		usize FindFirstNotOf(const StringStorage& other, usize off = 0) const noexcept;
		usize FindFirstNotOf(const StorageType* str, usize off = 0) const noexcept;
		usize FindFirstNotOf(const StorageType* str, usize off, usize len) const noexcept;
		usize FindFirstNotOf(const std::basic_string<StorageType>& str, usize off = 0) const noexcept;
		usize FindFirstNotOf(std::basic_string_view<StorageType> str, usize off = 0) const noexcept;

		usize FindLastNotOf(StorageType ch, usize off = NPos) const noexcept;
		usize FindLastNotOf(const StringStorage& other, usize off = NPos) const noexcept;
		usize FindLastNotOf(const StorageType* str, usize off = NPos) const noexcept;
		usize FindLastNotOf(const StorageType* str, usize off, usize len) const noexcept;
		usize FindLastNotOf(const std::basic_string<StorageType>& str, usize off = NPos) const noexcept;
		usize FindLastNotOf(std::basic_string_view<StorageType> str, usize off = NPos) const noexcept;
	protected:
		void ResetSizeAndEos(usize size) noexcept;

		StorageType* Buffer() noexcept;
		const StorageType* Buffer() const noexcept;

		static usize CalculateAllocateCapacity(usize requestCapacity, usize currentCapacity, usize maxCapacity) noexcept;

		void ReallocateHeapBuffer(usize capacity);
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
				StorageType Buffer[LocalStorageCapacity + 1];
			} Stack;
			struct
			{
				usize IsHeapBuffer : 1;
				usize Size : sizeof(usize)* BitsPerBytes - 1;
				usize Capacity;
				StorageType* Buffer;
			} Heap;
		} m_buffer;
	};


	template <typename StorageType>
	StringStorage<StorageType>::StringStorage() noexcept
	{
		InitSSOBuffer();
	}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(usize capacity)
	{
		if (capacity <= LocalStorageCapacity)
			InitSSOBuffer();
		else
			InitHeapBuffer(capacity);
	}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(StorageType ch, usize count)
	{
		if (count <= LocalStorageCapacity)
		{
			InitSSOBuffer();

			m_buffer.Stack.Size = count;

			for (usize i = 0; i < count; ++i)
				m_buffer.Stack.Buffer[i] = ch;

			m_buffer.Stack.Buffer[count] = StorageType();
		}
		else
		{
			InitHeapBuffer(count);

			m_buffer.Heap.Size = count;

			for (usize i = 0; i < count; ++i)
				m_buffer.Heap.Buffer[i] = ch;

			m_buffer.Heap.Buffer[count] = StorageType();
		}
	}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(const StorageType* str) : StringStorage(str, CharTraits::length(str))
	{}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(const StorageType* str, usize length)
	{
		if (length <= LocalStorageCapacity)
		{
			InitSSOBuffer();

			memcpy(m_buffer.Stack.Buffer, str, length * sizeof(StorageType));

			m_buffer.Stack.Size = length;
			m_buffer.Stack.Buffer[length] = StorageType();
		}
		else
		{
			InitHeapBuffer(length);

			memcpy(m_buffer.Heap.Buffer, str, length * sizeof(StorageType));

			m_buffer.Heap.Size = length;
			m_buffer.Heap.Buffer[length] = StorageType();
		}
	}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(const StringStorage& other) : StringStorage(other.Data(), other.Size())
	{

	}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(StringStorage&& other) noexcept
	{
		InitSSOBuffer();
		std::swap(m_buffer, other.m_buffer);
	}

	template <typename StorageType>
	StringStorage<StorageType>::StringStorage(const StringStorage& other, usize len) : StringStorage(other.Data(), std::min(len, other.Size()))
	{}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator=(const StringStorage& other)
	{
		DeallocateBuffer();

		usize len = other.Size();
		const StorageType* ptr = other.Data();

		if (len <= LocalStorageCapacity)
		{
			InitSSOBuffer();
			memcpy(m_buffer.Stack.Buffer, ptr, len * sizeof(StorageType));
			m_buffer.Stack.Size = len;
			m_buffer.Stack.Buffer[len] = StorageType();
		}
		else
		{
			InitHeapBuffer(len);
			memcpy(m_buffer.Heap.Buffer, ptr, len * sizeof(StorageType));
			m_buffer.Heap.Size = len;
			m_buffer.Heap.Buffer[len] = StorageType();
		}

		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator=(StringStorage&& other) noexcept
	{
		DeallocateBuffer();
		std::swap(m_buffer, other.m_buffer);
		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator=(const StorageType* str)
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

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator=(const std::basic_string<StorageType>& str)
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

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator=(std::basic_string_view<StorageType> str)
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

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator=(StorageType ch)
	{
		DeallocateBuffer();
		m_buffer.Stack.Buffer[0] = static_cast<cch>(ch);
		ResetSizeAndEos(1);
		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>::~StringStorage() noexcept
	{
		DeallocateBuffer();
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator+=(const StringStorage& other)
	{
		usize curSize = Size();
		usize othSize = other.Size();

		ReserveExtra(othSize);

		memcpy(Buffer() + curSize, other.Data(), othSize * sizeof(StorageType));

		ResetSizeAndEos(curSize + othSize);

		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator+=(const StorageType* str)
	{
		usize curSize = Size();
		usize strLength = CharTraits::length(str);

		ReserveExtra(strLength);

		memcpy(Buffer() + curSize, str, strLength * sizeof(StorageType));

		ResetSizeAndEos(curSize + strLength);

		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator+=(const std::basic_string<StorageType>& str)
	{
		usize curSize = Size();
		usize strSize = str.size();

		ReserveExtra(strSize);

		memcpy(Buffer() + curSize, str.data(), strSize * sizeof(StorageType));

		ResetSizeAndEos(curSize + strSize);

		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator+=(std::basic_string_view<StorageType> str)
	{
		usize curSize = Size();
		usize strSize = str.size();

		ReserveExtra(strSize);

		memcpy(Buffer() + curSize, str.data(), strSize * sizeof(StorageType));

		ResetSizeAndEos(curSize + strSize);

		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType>& StringStorage<StorageType>::operator+=(StorageType ch)
	{
		usize curSize = Size();

		ReserveExtra(1);

		Buffer()[curSize] = ch;

		ResetSizeAndEos(curSize + 1);

		return *this;
	}

	template <typename StorageType>
	StringStorage<StorageType> StringStorage<StorageType>::operator+(const StringStorage& other)
	{
		StringStorage tmp = *this;
		tmp += other;
		return tmp;
	}

	template <typename StorageType>
	StringStorage<StorageType> StringStorage<StorageType>::operator+(const StorageType* str)
	{
		StringStorage tmp = *this;
		tmp += str;
		return tmp;
	}

	template <typename StorageType>
	StringStorage<StorageType> StringStorage<StorageType>::operator+(const std::basic_string<StorageType>& str)
	{
		StringStorage tmp = *this;
		tmp += str;
		return tmp;
	}

	template <typename StorageType>
	StringStorage<StorageType> StringStorage<StorageType>::operator+(std::basic_string_view<StorageType> str)
	{
		StringStorage tmp = *this;
		tmp += str;
		return tmp;
	}

	template <typename StorageType>
	StringStorage<StorageType> StringStorage<StorageType>::operator+(StorageType ch)
	{
		StringStorage tmp = *this;
		tmp += ch;
		return tmp;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::operator==(const StringStorage& other) const noexcept
	{
		return Size() == other.Size() && CharTraits::compare(Data(), other.Data(), Size()) == 0;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::operator==(const StorageType* str) const noexcept
	{
		return CharTraits::compare(Data(), str, Size()) == 0 && str[Size()] == StorageType();
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::operator==(const std::basic_string<StorageType>& str) const noexcept
	{
		return CharTraits::compare(Data(), str.data(), Size()) == 0 && str.size() == Size();
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::operator==(std::basic_string_view<StorageType> str) const noexcept
	{
		return CharTraits::compare(Data(), str.data(), Size()) == 0 && str.size() == Size();
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::operator==(StorageType ch) const noexcept
	{
		return Size() == 1 && Buffer()[0] == ch;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Size() const noexcept
	{
		if (IsHeapBuffer())
			return m_buffer.Heap.Size;
		return m_buffer.Stack.Size;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Capacity() const noexcept
	{
		if (IsHeapBuffer())
			return m_buffer.Heap.Capacity;
		return LocalStorageCapacity;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::Empty() const noexcept
	{
		return Size() == 0;
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Reserve(usize newCapacity)
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

	template <typename StorageType>
	void StringStorage<StorageType>::ReserveExtra(usize extraCapacity)
	{
		Reserve(Size() + extraCapacity);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Resize(usize size, StorageType ch)
	{
		if (usize currentSize = Size(); size > currentSize)
		{
			ReserveExtra(size);
			StorageType* buffer = Buffer();
			for (usize i = currentSize; i < currentSize + size; ++i)
				buffer[i] = ch;
		}
		ResetSizeAndEos(size);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::ShrinkToFit()
	{
		if (IsHeapBuffer())
		{
			if (m_buffer.Heap.Size <= LocalStorageCapacity)
				MoveToStack();
			else if (m_buffer.Heap.Size < m_buffer.Heap.Capacity)
				ReallocateHeapBuffer(m_buffer.Heap.Size);
		}
	}

	template <typename StorageType>
	const StorageType* StringStorage<StorageType>::Data() noexcept
	{
		return Buffer();
	}

	template <typename StorageType>
	const StorageType* StringStorage<StorageType>::Data() const noexcept
	{
		return Buffer();
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Clear() noexcept
	{
		ResetSizeAndEos(0);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Append(const StringStorage& other)
	{
		if (other.IsHeapBuffer())
			Append(other.m_buffer.Heap.Buffer, other.m_buffer.Heap.Size);
		else
			Append(other.m_buffer.Stack.Buffer, other.m_buffer.Stack.Size);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Append(const StorageType* str)
	{
		return Append(str, CharTraits::length(str));
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Append(const StorageType* str, usize len)
	{
		if (len == 0)
			return;

		ReserveExtra(len);
		usize size = Size();
		cch* buffer = Buffer();
		memcpy(buffer + size, str, len);

		ResetSizeAndEos(size + len);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Append(StorageType ch, usize count)
	{
		if (count == 0)
			return;

		ReserveExtra(count);

		cch* buffer = Buffer();
		usize size = Size();
		for (usize i = size; i < size + count; ++i)
			buffer[i] = ch;

		ResetSizeAndEos(size + count);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Append(const std::basic_string<StorageType>& str)
	{
		Append(str.data(), str.size());
	}

	template <typename StorageType>
	void StringStorage<StorageType>::Append(std::basic_string_view<StorageType>& str)
	{
		Append(str.data(), str.size());
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushBack(const StringStorage& other)
	{
		Append(other);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushBack(const StorageType* str)
	{
		Append(str);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushBack(const StorageType* str, usize len)
	{
		Append(str, len);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushBack(StorageType ch, usize count)
	{
		Append(ch, count);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushBack(const std::basic_string<StorageType>& str)
	{
		Append(str);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushBack(std::basic_string_view<StorageType>& str)
	{
		Append(str);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushFront(const StringStorage& other)
	{
		if (other.IsHeapBuffer())
			PushFront(other.m_buffer.Heap.Buffer, other.m_buffer.Heap.Size);
		else
			PushFront(other.m_buffer.Stack.Buffer, other.m_buffer.Stack.Size);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushFront(const StorageType* str)
	{
		PushFront(str, CharTraits::length(str));
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushFront(const StorageType* str, usize len)
	{
		ReserveExtra(len);

		StorageType* buffer = Buffer();
		usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size + len);

		memcpy(buffer, str, len * sizeof(StorageType));
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushFront(StorageType ch, usize count)
	{
		ReserveExtra(count);

		StorageType* buffer = Buffer();
		usize size = Size();

		std::copy_backward(buffer, buffer + size, buffer + size +  count);
		
		for (usize i = 0; i < count; ++i)
			buffer[i] = ch;
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushFront(const std::basic_string<StorageType>& str)
	{
		PushFront(str.data(), str.size());
	}

	template <typename StorageType>
	void StringStorage<StorageType>::PushFront(std::basic_string_view<StorageType>& str)
	{
		PushFront(str.data(), str.size());
	}

	template <typename StorageType>
	template <typename T> requires (std::is_arithmetic_v<T> && !std::is_same_v<T, StorageType>)
		void StringStorage<StorageType>::Append(T v)
	{
		constexpr usize bufferLength = std::numeric_limits<T>::digits10 + 2;
		char buffer[bufferLength] = {};
		auto [ec, ptr] = std::to_chars(buffer, bufferLength, v);

		DEBUG_VERIFY_REPORT(ec == std::errc(), "buffer length must be large enough to accomodate the value")
			Append(buffer, static_cast<usize>(ptr - buffer));
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::Contain(StorageType ch, usize off) const noexcept
	{
		return Find(ch, off) != NPos;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::Contain(const StringStorage& other, usize off) const noexcept
	{
		return Find(other, off) != NPos;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::Contain(const StorageType* str, usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::Contain(const std::basic_string<StorageType>& str, usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::Contain(std::basic_string_view<StorageType>& str, usize off) const noexcept
	{
		return Find(str, off) != NPos;
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::begin() const noexcept
	{
		return ConstIterator(Buffer());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::end() const noexcept
	{
		return ConstIterator(Buffer() + Size());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::cbegin() const noexcept
	{
		return ConstIterator(Buffer());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::cend() const noexcept
	{
		return ConstIterator(Buffer() + Size());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::Iterator StringStorage<StorageType>::begin() noexcept
	{
		return Iterator(Buffer());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::Iterator StringStorage<StorageType>::end() noexcept
	{
		return Iterator(Buffer() + Size());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::rbegin() const noexcept
	{
		return ConstReverseIterator(end());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::rend() const noexcept
	{
		return ConstReverseIterator(begin());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::crbegin() const noexcept
	{
		return ConstReverseIterator(cend());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::crend() const noexcept
	{
		return ConstReverseIterator(cbegin());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ReverseIterator StringStorage<StorageType>::rbegin() noexcept
	{
		return ReverseIterator(end());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ReverseIterator StringStorage<StorageType>::rend() noexcept
	{
		return ReverseIterator(begin());
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::Begin() const noexcept
	{
		return begin();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::End() const noexcept
	{
		return end();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::CBegin() const noexcept
	{
		return cbegin();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstIterator StringStorage<StorageType>::CEnd() const noexcept
	{
		return cend();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::Iterator StringStorage<StorageType>::Begin() noexcept
	{
		return begin();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::Iterator StringStorage<StorageType>::End() noexcept
	{
		return end();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::RBegin() const noexcept
	{
		return rbegin();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::REnd() const noexcept
	{
		return rend();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::CRBegin() const noexcept
	{
		return crbegin();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ConstReverseIterator StringStorage<StorageType>::CREnd() const noexcept
	{
		return crend();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ReverseIterator StringStorage<StorageType>::RBegin() noexcept
	{
		return rbegin();
	}

	template <typename StorageType>
	typename StringStorage<StorageType>::ReverseIterator StringStorage<StorageType>::REnd() noexcept
	{
		return rend();
	}

	template <typename StorageType>
	StorageType StringStorage<StorageType>::Front() const noexcept
	{
		return Buffer()[0];
	}

	template <typename StorageType>
	StorageType StringStorage<StorageType>::Back() const noexcept
	{
		return Buffer()[Size() - 1];
	}

	template <typename StorageType>
	StringStorage<StorageType>::reference StringStorage<StorageType>::operator[](usize pos) noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscription out of range")
			return Buffer()[pos];
	}

	template <typename StorageType>
	StringStorage<StorageType>::const_reference StringStorage<StorageType>::operator[](usize pos) const noexcept
	{
		DEBUG_VERIFY_REPORT(pos < Size(), "string subscription out of range")
			return Buffer()[pos];
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Find(StorageType ch, usize off) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (sourceLength == 0 || off > sourceLength - 1)
			return NPos;

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;
		#ifdef _USE_STD_VECTOR_ALGORITHMS
		const StorageType* res = std::_Find_vectorized(sourceStart, sourceEnd, ch);
		if (res == sourceEnd)
			return NPos;

		return static_cast<usize>(res - source);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		const StorageType* match = CharTraits::find(source, sourceLength, ch);
		if (match == nullptr)
			return NPos;

		return static_cast<usize>(match - source);
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Find(const StringStorage& other, usize off) const noexcept
	{
		return Find(other.Data(), off, other.Size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Find(const StorageType* str, usize off) const noexcept
	{
		return Find(str, off, CharTraits::length(str));
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Find(const StorageType* str, usize off, usize len) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (len > sourceLength || off > sourceLength - len)
			return NPos;

		if (len == 0)
			return off;

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		const StorageType* res = std::_Search_vectorized(sourceStart, sourceEnd, str, len);

		if (res != sourceEnd)
			return static_cast<usize>(res - source);
		return NPos;

		#endif // _USE_STD_VECTOR_ALGORITHMS

		const StorageType* matchesEnd = source + (sourceLength - len) + 1;

		for (const StorageType* matchPosition = sourceStart;; ++matchPosition)
		{
			matchPosition = CharTraits::find(matchPosition, static_cast<size_t>(matchesEnd - matchPosition), *str);
			if (matchPosition == nullptr)
			{
				return NPos;
			}

			if (CharTraits::compare(matchPosition, str, len) == 0)
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Find(const std::basic_string<StorageType>& str, usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::Find(std::basic_string_view<StorageType>& str, usize off) const noexcept
	{
		return Find(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstOf(StorageType ch, usize off) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (off >= sourceLength)
			return NPos;

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;
		#ifdef _USE_STD_VECTOR_ALGORITHMS
		const StorageType* res = std::_Find_vectorized(sourceStart, sourceEnd, ch);
		if (res == sourceEnd)
			return NPos;

		return static_cast<usize>(res - source);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		const StorageType* match = CharTraits::find(source, sourceLength, ch);
		if (match == nullptr)
			return NPos;

		return static_cast<usize>(match - source);
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstOf(const StringStorage& other, usize off) const noexcept
	{
		return FindFirstOf(other.Data(), off, other.Size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstOf(const StorageType* str, usize off) const noexcept
	{
		return FindFirstOf(str, off, CharTraits::length(str));
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstOf(const StorageType* str, usize off, usize len) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (off >= sourceLength)
			return NPos;

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (usize vectorizedTargetLength = sourceLength - off; vectorizedTargetLength >= std::_Threshold_find_first_of)
		{
			usize pos = std::_Find_first_of_pos_vectorized(sourceStart, vectorizedTargetLength, str, len);
			return pos != NPos ? pos + off : NPos;
		}
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<StorageType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (usize i = 0; i < len; ++i)
				bitmap[static_cast<u8>(str[i])] = true;

			for (usize i = off; i < sourceLength; ++i)
				if (bitmap[static_cast<u8>(source[i])] == true)
					return i;

			return NPos;
		}

		for (const StorageType* matchPosition = sourceStart; matchPosition < sourceEnd; ++matchPosition) {
			if (CharTraits::find(str, len, *matchPosition)) {
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstOf(const std::basic_string<StorageType>& str, usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstOf(std::basic_string_view<StorageType> str, usize off) const noexcept
	{
		return FindFirstOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastOf(StorageType ch, usize off) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (sourceLength == 0) { // no room for match
			return NPos;
		}

		off = std::min(off, sourceLength - 1);

		const StorageType* sourceStart = source + off + 1;

		#if _USE_STD_VECTOR_ALGORITHMS
		const StorageType* sourceEnd = source + off + 1;
		const StorageType* res = std::_Find_last_vectorized(source, sourceEnd, ch);

		if (res != sourceEnd)
			return static_cast<size_t>(res - source);

		return NPos;
		#endif // _USE_STD_VECTOR_ALGORITHMS

		for (auto matchPosition = sourceStart;; --matchPosition) {
			if (CharTraits::eq(*matchPosition, ch)) {
				return static_cast<size_t>(matchPosition - source);
			}

			if (matchPosition == source) {
				return NPos;
			}
		}
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastOf(const StringStorage& other, usize off) const noexcept
	{
		return FindLastOf(other.Data(), off, other.Size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastOf(const StorageType* str, usize off) const noexcept
	{
		return FindLastOf(str, off, CharTraits::length(str));
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastOf(const StorageType* str, usize off, usize len) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (len == 0 || sourceLength == 0)
			return NPos;

		off = std::min(sourceLength - 1, off);

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (usize vectorizedTargetLength = off + 1; vectorizedTargetLength >= std::_Threshold_find_first_of)
			return std::_Find_last_of_pos_vectorized(source, off + 1, str, len);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<StorageType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (usize i = 0; i < len; ++i)
				bitmap[static_cast<u8>(str[i])] = true;

			for (usize i = off; i > 0; --i)
				if (bitmap[static_cast<u8>(source[i - 1])] == true)
					return i - 1;

			return NPos;
		}

		for (const StorageType* matchPosition = source + off; matchPosition > source; --matchPosition)
		{
			if (CharTraits::find(str, len, *matchPosition))
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastOf(const std::basic_string<StorageType>& str, usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastOf(std::basic_string_view<StorageType> str, usize off) const noexcept
	{
		return FindLastOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstNotOf(StorageType ch, usize off) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (off >= sourceLength)
			return NPos;

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;

		#if _USE_STD_VECTOR_ALGORITHMS
		const StorageType* res = _STD _Find_not_ch_vectorized(sourceStart, sourceEnd, ch);
		if (res != sourceEnd)
			return static_cast<size_t>(res - source);
		return NPos;
		#endif // _USE_STD_VECTOR_ALGORITHMS

		for (auto matchPosition = sourceStart; matchPosition < sourceEnd; ++matchPosition)
		{
			if (!CharTraits::eq(*matchPosition, ch))
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstNotOf(const StringStorage& other, usize off) const noexcept
	{
		return FindFirstNotOf(other.Data(), off, other.Size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstNotOf(const StorageType* str, usize off) const noexcept
	{
		return FindFirstNotOf(str, off, CharTraits::length(str));
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstNotOf(const StorageType* str, usize off, usize len) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (off >= sourceLength)
			return NPos;

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (usize vectorizedTargetLength = sourceLength - off; vectorizedTargetLength >= std::_Threshold_find_first_of)
		{
			usize pos = std::_Find_first_not_of_pos_vectorized(sourceStart, sourceLength - off, str, len);
			return pos != NPos ? pos + off : NPos;
		}
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<StorageType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (usize i = 0; i < len; ++i)
				bitmap[static_cast<u8>(str[i])] = true;

			for (usize i = off; i < sourceLength; ++i)
				if (bitmap[static_cast<u8>(source[i])] == false)
					return i;

			return NPos;
		}

		for (const StorageType* matchPosition = sourceStart; matchPosition < sourceEnd; ++matchPosition) {
			if (!CharTraits::find(str, len, *matchPosition)) {
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstNotOf(const std::basic_string<StorageType>& str,
													 usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindFirstNotOf(std::basic_string_view<StorageType> str, usize off) const noexcept
	{
		return FindFirstNotOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastNotOf(StorageType ch, usize off) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (sourceLength == 0) { // no room for match
			return NPos;
		}

		off = std::min(off, sourceLength - 1);

		const StorageType* sourceStart = source + off + 1;

		#if _USE_STD_VECTOR_ALGORITHMS
		const StorageType* sourceEnd = source + off + 1;
		return std::_Find_last_not_ch_pos_vectorized(source, sourceEnd, ch);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		for (auto matchPosition = sourceStart;; --matchPosition) {
			if (!CharTraits::eq(*matchPosition, ch)) {
				return static_cast<size_t>(matchPosition - source);
			}

			if (matchPosition == source) {
				return NPos;
			}
		}
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastNotOf(const StringStorage& other, usize off) const noexcept
	{
		return FindLastNotOf(other.Data(), off, other.Size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastNotOf(const StorageType* str, usize off) const noexcept
	{
		return FindLastNotOf(str, off, CharTraits::length(str));
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastNotOf(const StorageType* str, usize off, usize len) const noexcept
	{
		const StorageType* source = Data();
		usize sourceLength = Size();

		if (len == 0 || sourceLength == 0)
			return NPos;

		off = std::min(sourceLength - 1, off);

		const StorageType* sourceStart = source + off;
		const StorageType* sourceEnd = source + sourceLength;

		#ifdef _USE_STD_VECTOR_ALGORITHMS
		if (usize vectorizedTargetLength = off + 1; vectorizedTargetLength >= std::_Threshold_find_first_of)
			return std::_Find_last_not_of_pos_vectorized(source, off + 1, str, len);
		#endif // _USE_STD_VECTOR_ALGORITHMS

		if constexpr (IsOneOf<StorageType, char, char8_t>)
		{
			bool bitmap[256] = {};

			for (usize i = 0; i < len; ++i)
				bitmap[static_cast<u8>(str[i])] = true;

			for (usize i = off; i > 0; --i)
				if (bitmap[static_cast<u8>(source[i - 1])] == false)
					return i - 1;

			return NPos;
		}

		for (const StorageType* matchPosition = source + off; matchPosition > source; --matchPosition)
		{
			if (!CharTraits::find(str, len, *matchPosition))
			{
				return static_cast<size_t>(matchPosition - source);
			}
		}

		return NPos;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastNotOf(const std::basic_string<StorageType>& str, usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::FindLastNotOf(std::basic_string_view<StorageType> str, usize off) const noexcept
	{
		return FindLastNotOf(str.data(), off, str.size());
	}

	template <typename StorageType>
	void StringStorage<StorageType>::ResetSizeAndEos(usize size) noexcept
	{
		if (IsHeapBuffer())
		{
			m_buffer.Heap.Size = size;
			m_buffer.Heap.Buffer[size] = StorageType();
		}
		else
		{
			DEBUG_VERIFY_REPORT(size <= LocalStorageCapacity, "Size exceeds local storage capacity");
			m_buffer.Stack.Size = size;
			m_buffer.Stack.Buffer[size] = StorageType();
		}
	}

	template <typename StorageType>
	StorageType* StringStorage<StorageType>::Buffer() noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap.Buffer : m_buffer.Stack.Buffer;
	}

	template <typename StorageType>
	const StorageType* StringStorage<StorageType>::Buffer() const noexcept
	{
		return IsHeapBuffer() ? m_buffer.Heap.Buffer : m_buffer.Stack.Buffer;
	}

	template <typename StorageType>
	usize StringStorage<StorageType>::CalculateAllocateCapacity(usize requestCapacity, usize currentCapacity,
																usize maxCapacity) noexcept
	{
		usize masked = requestCapacity | AllocateMask;
		if (masked > maxCapacity)
			return maxCapacity;

		if (currentCapacity > maxCapacity - currentCapacity / 2)
			return maxCapacity;

		return std::max(masked, currentCapacity + currentCapacity / 2);
	}

	template <typename StorageType>
	void StringStorage<StorageType>::ReallocateHeapBuffer(usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, m_buffer.Heap.Capacity, MaxStorageCapacity);
		StorageType* newBuffer = Memory::Allocate<StorageType>(capacity + 1);

		if (m_buffer.Heap.Size > 0)
			memcpy(newBuffer, m_buffer.Heap.Buffer, m_buffer.Heap.Size * sizeof(StorageType));

		Memory::Deallocate(m_buffer.Heap.Buffer, m_buffer.Heap.Capacity + 1);

		m_buffer.Heap.Buffer = newBuffer;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer[m_buffer.Heap.Size] = StorageType();
	}

	template <typename StorageType>
	void StringStorage<StorageType>::DeallocateBuffer() noexcept
	{
		if (IsHeapBuffer())
			Memory::Deallocate(m_buffer.Heap.Buffer, m_buffer.Heap.Capacity + 1);
		InitSSOBuffer();
	}

	template <typename StorageType>
	void StringStorage<StorageType>::InitSSOBuffer() noexcept
	{
		m_buffer.Stack.IsHeapBuffer = false;
		m_buffer.Stack.Size = 0;
		m_buffer.Stack.Buffer[0] = StorageType();
	}

	template <typename StorageType>
	void StringStorage<StorageType>::InitHeapBuffer(usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, LocalStorageCapacity, MaxStorageCapacity);

		StorageType* buffer = Memory::Allocate<StorageType>(capacity + 1);

		m_buffer.Heap.IsHeapBuffer = true;
		m_buffer.Heap.Size = 0;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer = buffer;
		m_buffer.Heap.Buffer[0] = StorageType();
	}

	template <typename StorageType>
	bool StringStorage<StorageType>::IsHeapBuffer() const noexcept
	{
		return m_buffer.Stack.IsHeapBuffer;
	}

	template <typename StorageType>
	void StringStorage<StorageType>::MoveToStack() noexcept
	{
		// 这一步对heapSize的检查应该在调用前进行，这里不做防御性检查
		usize heapSize = m_buffer.Heap.Size;
		usize heapCapacity = m_buffer.Heap.Capacity;

		StorageType* heapBuffer = m_buffer.Heap.Buffer;

		// 如果需要constexpr路径，需要先初始化StackBuffer
		// InitSSOBuffer();

		if (heapSize > 0)
			memcpy(m_buffer.Stack.Buffer, heapBuffer, heapSize * sizeof(StorageType));

		Memory::Deallocate(heapBuffer, heapCapacity + 1);

		m_buffer.Stack.IsHeapBuffer = false;
		m_buffer.Stack.Size = heapSize;

		// 可以通过在memcpy中复制size + 1个元素来复制结尾符，但是为了规范性，仍然采用手动设置结尾符
		m_buffer.Stack.Buffer[heapSize] = StorageType();
	}

	template <typename StorageType>
	void StringStorage<StorageType>::MoveToHeap(usize capacity)
	{
		capacity = CalculateAllocateCapacity(capacity, LocalStorageCapacity, MaxStorageCapacity);

		// Allocate不会构造对象，但是StorageType是一个POD的字符类型，所以不需要构造函数
		// 如果后续需要设计constexpr路径，则需要在if consteval路径下构造每个元素
		StorageType* buffer = Memory::Allocate<StorageType>(capacity + 1);

		usize size = m_buffer.Stack.Size;

		if (size > 0)
			memcpy(buffer, m_buffer.Stack.Buffer, size * sizeof(StorageType));

		m_buffer.Heap.IsHeapBuffer = true;
		m_buffer.Heap.Size = size;
		m_buffer.Heap.Capacity = capacity;
		m_buffer.Heap.Buffer = buffer;

		// 可以通过在memcpy中复制size + 1个元素来复制结尾符，但是为了规范性，仍然采用手动设置结尾符
		m_buffer.Heap.Buffer[size] = StorageType();
	}
}