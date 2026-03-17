// File /Engine/Objects/PObject.h
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
#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"
#include "../Math/Vec4.hpp"
#include "../String/String.hpp"
#include <any>
#include <optional>
#include <unordered_map>
#include <variant>
#include <expected>
#include "../String/StringUnorderedMap.hpp"

#define P_OBJECT(ClassType,InheritClassType)

namespace PenFramework::PenEngine
{
	using ResourceID = u64;
	using ObjectID = u64;

	class PObject
	{
	public:
		virtual ~PObject() = default;

		//元数据
		virtual StringView GetMetaType() const noexcept = 0;
		virtual Usize GetMetaHash() const noexcept = 0;

		//属性
		enum class TryGetPropertyError : U8
		{
			KeyNotFound,
			PropertyIsEmpty,
			TypeNotMatched
		};

		template <typename T>
		std::expected<T,TryGetPropertyError> TryGetProperty(StringView propertyName);
		void SetProperty(StringView propertyName,const std::any& property);
	private:
		ObjectID m_objectID = 0;

		StringUnorderedMap<std::any> m_properties;
	};

	template <typename T>
	std::expected<T, PObject::TryGetPropertyError> PObject::TryGetProperty(StringView propertyName)
	{
		auto it = m_properties.find(propertyName);
		if(it == m_properties.end())
			return std::unexpected(TryGetPropertyError::KeyNotFound);

		if(!it->second.has_value())
			return std::unexpected(TryGetPropertyError::PropertyIsEmpty);

		T* ptr = std::any_cast<T*>(&it->second);

		if(ptr == nullptr)
			return std::unexpected(TryGetPropertyError::TypeNotMatched);

		return std::expected(*ptr);
	}
}
