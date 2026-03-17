#include "PathAnalyzer.h"

PenFramework::PenEngine::Internal::PathAnalyzer::Iterator PenFramework::
PenEngine::Internal::PathAnalyzer::Search()
{
	const Ch* start = GetStartPointer();
	const Ch* end = m_rawPath.Data() + m_rawPath.Size();

	if (start == end)
	{
		m_state = State::AtEnd;
		return Iterator{this,m_state,m_searchedPath};
	}

	switch (m_state)
	{
	case State::AtStart:
	{
		const Ch* ret = TrySkipAllSeparator(start, end);
		if (ret != nullptr)
		{
			m_state = State::AtRootName;
			m_searchedPath = StringView(start, ret);
			return Iterator{this, m_state,m_searchedPath };
		}
	}
	case State::AtRootName:
	{
		const Ch* ret = TrySkipAllSeparator(start, end);
		if (ret != nullptr)
		{
			m_state = State::AtRootDir;
			m_searchedPath = StringView(start, ret);
			return Iterator{this, m_state,m_searchedPath };
		}

		m_state = State::AtFilename;
		m_searchedPath = StringView(start, TrySkipName(start, end));
		return Iterator{this, m_state,m_searchedPath };
	}
	case State::AtRootDir:
	{
		m_state = State::AtFilename;
		m_searchedPath = StringView(start, TrySkipName(start, end));
		return Iterator{this, m_state,m_searchedPath };
	}
	case State::AtFilename:
	{
		const Ch* sasRet = TrySkipAllSeparator(start, end);
		if (sasRet != end)
		{
			const Ch* snRet = TrySkipName(sasRet, end);
			if (snRet != nullptr)
			{
				m_state = State::AtFilename;
				m_searchedPath = StringView(sasRet, snRet);
				return Iterator{this, m_state,m_searchedPath };
			}
		}

		m_state = State::AtRemainingSeparator;
		m_searchedPath = StringView(start, sasRet);
		return Iterator{this, m_state,m_searchedPath };
	}
	case State::AtRemainingSeparator:
	{
		m_state = State::AtEnd;
		m_searchedPath = {};
		return Iterator{this, m_state,m_searchedPath };
	}
	case State::AtEnd:
		break;
	}

	std::unreachable();
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::GetStartPointer() const noexcept
{
	switch (m_state)
	{
	case State::AtStart:
		return m_rawPath.Data();
	case State::AtRootName:
	case State::AtRootDir:
	case State::AtFilename:
		return m_searchedPath.Data() + 1;
	case State::AtRemainingSeparator:
	case State::AtEnd:
		return m_rawPath.Data() + m_rawPath.Size();
	}

	std::unreachable();
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::TrySkipRootName(const Ch* start, const Ch* end) noexcept
{
	#ifdef _WIN32

	// 先检查是不是本地驱动器标识路径
	if (auto ret = TrySkipDriveRoot(start, end); ret != nullptr)
		return ret;
	// 再检查是不是网络驱动器标识路径
	if (auto ret = TrySkipNetworkRoot(start, end); ret != nullptr)
		return ret;

	#endif // _WIN32
	return nullptr;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::TrySkipDriveRoot(const Ch* start, const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	// 示例情况
	// start -> "1:\.."
	// start[0] != 'a'~'z' || 'A'~'Z' 
	// start[1] == ':'
	// ret nullptr

	// start -> "C\..."
	// start[0] == 'a'~'z' || 'A'~'Z' 
	// start[1] != ':'
	// ret nullptr

	// start -> "C:"
	// end == ":"
	// start + 1 == end
	// ret nullptr

	if (start + 1 == end || !IsValidDriveLetter(start[0]) || start[1] != ':')
		return nullptr;

	return start + 2;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::TrySkipNetworkRoot(const Ch* start, const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	return TrySkipName(TrySkipCountedSeparator(start, end, 2), end);
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::TrySkipName(const Ch* start,
																								const Ch* end) noexcept
{
	if (start == nullptr || start == end || IsValidSeparator(*start))
		return nullptr;

	start += 1;

	while (start != end && !IsValidSeparator(*start))
		start += 1;

	return start;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::TrySkipAllSeparator(const Ch* start, const Ch* end) noexcept
{
	if (start == nullptr || start == end)
		return nullptr;

	if (!IsValidSeparator(*start))
		return nullptr;

	start += 1;

	while (start != end && IsValidSeparator(*start))
		start += 1;

	return start;
}

const PenFramework::PenEngine::Ch* PenFramework::PenEngine::Internal::PathAnalyzer::TrySkipCountedSeparator(const Ch* start,
																											const Ch* end, Usize count) noexcept
{
	const Ch* ret = TrySkipAllSeparator(start, end);

	if (ret == nullptr)
		return nullptr;

	if (ret == start + count)
		return ret;

	return nullptr;
}

bool PenFramework::PenEngine::Internal::PathAnalyzer::IsValidSeparator(Ch ch) noexcept
{
	if (ch == '/')
		return true;
	#if _WIN32
	if (ch == '\\')
		return true;
	#endif // _WIN32
	return false;
}

bool PenFramework::PenEngine::Internal::PathAnalyzer::IsValidDriveLetter(Ch ch) noexcept
{
	return (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A');
}
