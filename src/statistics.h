#pragma once

#ifndef statistics_h__
#define statistics_h__

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <utility>

#include <QDataStream>


template <class T>
class Statistics
{
public:

	using T2 = decltype(T{} * T{});

public:

	Statistics()
		: m_count(T{0})
		, m_max(std::numeric_limits<T>::lowest())
		, m_min(std::numeric_limits<T>::max())
		, m_sum(T{0})
		, m_sumOfSquares(T2{0})
	{
	}

	explicit Statistics(T measurement)
		: m_count(1)
		, m_max(measurement)
		, m_min(measurement)
		, m_sum(measurement)
		, m_sumOfSquares(measurement * measurement)
	{
	}

	Statistics(std::initializer_list<T> init)
		: Statistics()
	{
		for (const auto& measurement : init)
			*this += measurement;
	}

	template <class InputIt>
	explicit Statistics(InputIt first, InputIt last)
		: Statistics()
	{
		for (InputIt itr = first; itr != last; ++itr)
			*this += *itr;
	}


	void clear() noexcept
	{
		*this = std::move(Statistics());
	}


	[[nodiscard]] size_t count() const noexcept
	{
		return m_count;
	}


	Statistics& insert(const T& measurement) noexcept
	{
		*this += measurement;
		return *this;
	}


	template <class InputIt>
	Statistics& insert(const InputIt& first, const InputIt& last) noexcept
	{
		for (InputIt itr = first; itr != last; ++itr)
			*this += *itr;

		return *this;
	}

	[[nodiscard]] T mean() const noexcept
	{
		return m_sum / std::max<size_t>(m_count, 1);
	}

	[[nodiscard]] T min() const noexcept
	{
		return m_min;
	}


	[[nodiscard]] T max() const noexcept
	{
		return m_max;
	}


	[[nodiscard]] T sum() const noexcept
	{
		return m_sum;
	}


	[[nodiscard]] T2 sumOfSquares() const noexcept
	{
		return m_sumOfSquares;
	}

	[[nodiscard]] auto variance() const noexcept
	{
		auto denom = std::max<size_t>(m_count, 1);
		return ((m_sumOfSquares / denom) - (m_sum / denom) * (m_sum / denom));
	}


	[[nodiscard]] T standardDeviation() const noexcept
	{
		return sqrt(variance());
	}


	Statistics& operator+=(const T& measurement) noexcept
	{
		if (measurement < m_min)
			m_min = measurement;
		else if (measurement > m_max)
			m_max = measurement;

		m_sum += measurement;
		m_sumOfSquares += measurement * measurement;

		++m_count;

		return *this;
	}


	Statistics& operator+=(const Statistics& rhs) noexcept
	{
		m_count += rhs.m_count;
		m_max = std::max(m_max, rhs.m_max);
		m_min = std::min(m_min, rhs.m_min);
		m_sum += rhs.m_sum;
		m_sumOfSquares += rhs.m_sumOfSquares;

		return *this;
	}


	template <class Ty>
	friend Statistics<Ty> operator+(const Statistics<Ty>& lhs, const T& rhs) noexcept;
	template <class Ty>
	friend Statistics<Ty> operator+(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;
	template <class Ty>
	friend bool operator==(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;
	template <class Ty>
	friend bool operator!=(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;
	template <class Ty>
	friend bool operator<(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;
	template <class Ty>
	friend bool operator<=(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;
	template <class Ty>
	friend bool operator>(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;
	template <class Ty>
	friend bool operator>=(const Statistics<Ty>& lhs, const Statistics<Ty>& rhs) noexcept;

	template <class Ty>
	friend QDataStream& operator<<(QDataStream& stream, const Statistics<Ty>& stats);
	template <class Ty>
	friend QDataStream& operator>>(QDataStream& stream, Statistics<Ty>& stats);

private:

	size_t m_count;
	T      m_max;
	T      m_min;
	T      m_sum;
	T2     m_sumOfSquares;
};


template <class T>
Statistics<T> operator+(const Statistics<T>& lhs, const T& rhs) noexcept
{
	Statistics<T> s;
	s += lhs;
	s += rhs;
	return s;
}


template <class T>
Statistics<T>& operator+(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	Statistics<T> s;
	s += lhs;
	s += rhs;
	return s;
}


template <class T>
bool operator==(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	if (lhs.m_count != rhs.m_count ||
		lhs.m_min != rhs.m_min ||
		lhs.m_max != rhs.m_max ||
		lhs.m_sum != rhs.m_sum)
		return false;
	else
		return true;
}

template <class T>
bool operator!=(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return !(lhs == rhs);
}

template <class T>
bool operator<(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	if (lhs.m_count < rhs.m_count || lhs.m_min < rhs.m_min ||
      lhs.m_max < rhs.m_max || lhs.m_sum < rhs.m_sum)
    return true;
  return false;
}

template <class T>
bool operator<=(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return !(rhs < lhs);
}

template <class T>
bool operator>(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return rhs < lhs;
}

template <class T>
bool operator>=(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return !(lhs < rhs);
}


template <class T>
QDataStream& operator<<(QDataStream& stream, const Statistics<T>& stats)
{
	stream << (quint64)stats.m_count << stats.m_max << stats.m_min << stats.m_sum << stats.m_sumOfSquares;
	return stream;
}


template <class T>
QDataStream& operator>>(QDataStream& stream, Statistics<T>& stats)
{
	quint64 count;
	stream >> count >> stats.m_max >> stats.m_min >> stats.m_sum >> stats.m_sumOfSquares;
	stats.m_count = count;

	return stream;
}

#endif // average_h__