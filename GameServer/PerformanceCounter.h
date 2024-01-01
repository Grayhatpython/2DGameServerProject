#pragma once

class PerformanceCounter
{
public:
	static void Initialize()
	{
		::QueryPerformanceFrequency(&_frequency);
	}

	void Measure()
	{
		::QueryPerformanceCounter(&_startCount);
	}

public:
	auto ElapsedTimeMilliSecond() const
	{
		LARGE_INTEGER currentCount;
		::QueryPerformanceCounter(&currentCount);

		return (((currentCount.QuadPart - _startCount.QuadPart) * 1000) / _frequency.QuadPart);
	}

	auto ElapsedTimeMicroSecond() const
	{
		LARGE_INTEGER currentCount;
		::QueryPerformanceCounter(&currentCount);

		return (((currentCount.QuadPart - _startCount.QuadPart) * 1000000) / _frequency.QuadPart);
	}

private:
	static LARGE_INTEGER	_frequency;
	LARGE_INTEGER			_startCount;
};

