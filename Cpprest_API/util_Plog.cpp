#include "util_Plog.h"

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Appenders/EventLogAppender.h>

#include <sys/stat.h> 
#include <sys/types.h>

#include <algorithm>	// std::replace
#include <iostream>


void MakePlogFile_Multi(void)
{
	static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("TX_Log.txt", 1000000000, 3); // Create the 1st appender.
	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

#if _DEBUG
	plog::init(plog::verbose, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.
#else
	plog::init(plog::verbose, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.
#endif

}

void MakePlogFile_Single(void)
{
	char mbLogFileName[256];

	int nResult = mkdir("Logs",0777);
	if (false == nResult)
	{
		std::cout << "make dir failed" << std::endl;
	}

	auto time = std::time(nullptr);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%F_%p %I:%M:%S");
	auto s = ss.str();
	std::replace(s.begin(), s.end(), ':', '-');


#if _DEBUG
	sprintf(mbLogFileName, "Logs\\[DEBUG]MSLM TX LOG %s.txt", s.c_str());
	plog::init(plog::verbose, mbLogFileName);
#else
	sprintf(mbLogFileName, "Logs\\MSLM TX LOG %s.txt", s.c_str());
	plog::init(plog::info, mbLogFileName);
#endif
}