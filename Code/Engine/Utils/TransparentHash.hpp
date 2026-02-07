#pragma once
#include "Concept.hpp"

namespace PenFramework::PenEngine
{

	template <typename Bridge, typename Hash = std::hash<Bridge>, typename... From> requires (StaticCastable<From, Bridge> && ...)
		struct TransparentHash
	{
		using is_transparent = void;

		template <typename T> requires OneOf<T, From...>
		size_t operator()(T&& element) const noexcept(noexcept(std::declval<Hash>()(std::declval<Bridge>())))
		{
			return Hash()(static_cast<Bridge>(std::forward<T>(element)));
		}
	};
}
