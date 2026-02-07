#pragma once

#define SPDLOG_USE_STD_FORMAT

#include "../Common/Type.hpp"
#include "Singleton.hpp"
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/wincolor_sink.h>


namespace PenFramework::PenEngine
{
	class Logger : public Singleton<Logger>
	{
	public:
		struct LoggerConfiguration
		{
			std::string_view FileName;
			usize MaxFileSize;
			usize MaxFileCapacity;
			std::vector<std::shared_ptr<spdlog::sinks::sink>> AdditionalSink;
		};
		Logger() noexcept = default;
		virtual ~Logger() noexcept override = default;

		void Init(LoggerConfiguration& configuration);

		template <typename... Args>
		void Info(std::format_string<Args...> fmt, Args&&... args);
		template <typename... Args>
		void Warn(std::format_string<Args...> fmt, Args&&... args);
		template <typename... Args>
		void Error(std::format_string<Args...> fmt, Args&&... args);
		template <typename... Args>
		void Debug(std::format_string<Args...> fmt, Args&&... args);
	private:
		std::shared_ptr<spdlog::logger> m_logger;
	};

	template <typename ... Args>
	void Logger::Info(std::format_string<Args...> fmt, Args&&... args)
	{
		m_logger->info(fmt,std::forward<Args>(args)...);
	}

	template <typename ... Args>
	void Logger::Warn(std::format_string<Args...> fmt, Args&&... args)
	{
		m_logger->warn(fmt, std::forward<Args>(args)...);
	}

	template <typename ... Args>
	void Logger::Error(std::format_string<Args...> fmt, Args&&... args)
	{
		m_logger->error(fmt, std::forward<Args>(args)...);
	}

	template <typename ... Args>
	void Logger::Debug(std::format_string<Args...> fmt, Args&&... args)
	{
		m_logger->debug(fmt, std::forward<Args>(args)...);
	}

	inline void Logger::Init(LoggerConfiguration& configuration)
	{
		configuration.AdditionalSink.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
		configuration.AdditionalSink.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(configuration.FileName.data(), configuration.MaxFileSize, configuration.MaxFileCapacity));

		m_logger = std::make_shared<spdlog::logger>("", configuration.AdditionalSink.begin(),configuration.AdditionalSink.end());

#ifdef _DEBUG
		m_logger->set_level(spdlog::level::debug);
#else
		m_logger->set_level(spdlog::level::info);
#endif // _DEBUG
	}
}
