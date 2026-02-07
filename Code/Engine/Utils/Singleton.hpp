#pragma once

#include <type_traits>

namespace PenFramework::PenEngine
{
	template <typename T>
	class Singleton
	{
		friend T;
	public:
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;

		virtual ~Singleton() noexcept = default;

		static T& GetInstance() noexcept(std::is_nothrow_default_constructible_v<T>)
		{
			static T instance;
			return instance;
		}
	private:
		Singleton() noexcept = default;
	};
}