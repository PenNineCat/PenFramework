// File /Engine/Analysis/Time/PerformanceTimer.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Utils/Logger.hpp"
#include <chrono>

namespace PenFramework::PenEngine
{
	class PerformanceTimer
	{
	public:
		PerformanceTimer() noexcept = default;
		explicit PerformanceTimer(std::string_view taskDescription) :m_taskDescription(taskDescription) {}
		~PerformanceTimer() noexcept 
		{
			try
			{
				Logger::GetInstance().Debug("当前任务：{}耗时：{}", m_taskDescription, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_startPoint));
			}
			catch (...){}  // NOLINT(bugprone-empty-catch)
		}
	private:
		std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
		std::string m_taskDescription;
	};
}