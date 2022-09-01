#include "UtilStopwatch.h"

namespace Util
{
	Stopwatch::Stopwatch()
		: m_bRunning(false), m_startTime(0), m_stopTime(0)
	{
		
	}
	Stopwatch::~Stopwatch()
	{
	}

	void Stopwatch::start()
	{
		if (m_bRunning == true)
			return;

		m_startTime = clock();
		m_stopTime = 0;

		m_bRunning = true;
	}
	void Stopwatch::stop()
	{
		if (m_bRunning == false)
			return;

		m_stopTime = clock();

		m_bRunning = false;
	}
	double Stopwatch::getTime()
	{
		return (double)((m_stopTime == 0 ? clock() : m_stopTime) - m_startTime) / CLOCKS_PER_SEC;
	}
}