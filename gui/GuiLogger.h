#pragma once
#include "gui/gui.h"
#include <list>
#include <map>
#include <string>
#include <array>

// Set the logger buffer max size (lines)
const size_t k_gui_logger_buffer_max_size = 1500;

enum class GUILoggerLevel
{
	E_Info = 1 << 0,
	E_Warning = 1 << 1,
	E_Error = 1 << 2,
	E_Max
};

bool operator&(int l, GUILoggerLevel r);

/*
* LoggerBuffer is a kind of circular buffer. In particular, this class also keeps the order of messages in buffer
*/
template<typename T, size_t buffer_max_size_>
class LoggerBuffer
{
public:
	typedef typename std::list<T>::iterator iterator;
	typedef typename std::list<T>::const_iterator const_iterator;

	LoggerBuffer()
	{
		m_iterator_head_ = m_data_.end();
		m_iterator_tail_ = m_data_.end();
	}

	~LoggerBuffer() = default;

	constexpr size_t max_size() const noexcept
	{
		return buffer_max_size_;
	}

	void Push(const T& v)
	{
		if (m_data_.size() < buffer_max_size_)
		{
			m_data_.emplace_back(v);

			if (m_data_.size() > 1)
			{
				++m_iterator_tail_;
			}
			else
			{
				m_iterator_head_ = m_iterator_tail_ = m_data_.begin();
			}
		}
		else
		{
			if (std::next(m_iterator_head_) != m_data_.end())
			{
				++m_iterator_head_;
			}
			else
			{
				m_iterator_head_ = m_data_.begin();
			}

			if (std::next(m_iterator_tail_) != m_data_.end())
			{
				++m_iterator_tail_;
			}
			else
			{
				m_iterator_tail_ = m_data_.begin();
			}

			*m_iterator_tail_ = v;
		}
	}

	void Clear()
	{
		m_data_.clear();
		m_iterator_head_ = m_data_.end();
		m_iterator_tail_ = m_data_.end();
	}

	iterator Head() noexcept
	{
		return m_iterator_head_;
	}

	iterator Tail() noexcept
	{
		return m_iterator_tail_;
	}

	iterator Begin() noexcept
	{
		return m_data_.begin();
	}

	iterator End() noexcept
	{
		return m_data_.end();
	}

	const_iterator Head() const noexcept
	{
		return m_iterator_head_;
	}

	const_iterator Tail() const noexcept
	{
		return m_iterator_tail_;
	}

	const_iterator Begin() const noexcept
	{
		return m_data_.begin();
	}

	const_iterator End() const noexcept
	{
		return m_data_.end();
	}

	template<typename F>
	void ForEach(F f)
	{
		auto itrHead = Head();
		auto itrTail = Tail();
		while (itrHead != End())
		{
			f(*itrHead);
			if (itrHead == itrTail) break;

			++itrHead;

			if (itrHead == End())
			{
				itrHead = Begin();
			}
		}
	}

private:
	iterator m_iterator_head_;

	iterator m_iterator_tail_;

	std::list<T> m_data_;
};


class GuiLogger : public Gui
{
public:
	GuiLogger();

	void Info(const std::string& msg);

	void Warning(const std::string& msg);

	void Error(const std::string& msg);

	void Push(const std::string& msg, GUILoggerLevel level);

	void Clear();

	/**
	* @brief Set a filter level combination. For example, using E_Info | E_Warning to enable info and warning
	*/
	void SetFilter(int flag);

	void Draw() override;

private:
	LoggerBuffer < std::pair<std::string, GUILoggerLevel>, k_gui_logger_buffer_max_size> m_buffer_;

	std::array<bool, static_cast<size_t>(GUILoggerLevel::E_Max)> m_filter_;

	bool m_update_scrolling_bar_;
};
