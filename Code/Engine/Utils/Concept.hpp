#pragma once
#include <type_traits>

namespace PenFramework::PenEngine
{
	template <typename T, typename... Ts>
	concept OneOf = (std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Ts>> || ...);

	template <typename From, typename To>
	concept StaticCastable = requires(From && t) { static_cast<To>(std::forward<From>(t)); };
}
