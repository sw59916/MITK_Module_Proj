#pragma once
#include "MitkExampleModuleExports.h"
#include "time.h"

namespace Util
{
	class MITKEXAMPLEMODULE_EXPORT Stopwatch
	{
	public:
		Stopwatch();
		~Stopwatch();

	public:
		void start();
		void stop();

		double getTime();

	private:
		bool m_bRunning;

		clock_t m_startTime;
		clock_t m_stopTime;
	};
}