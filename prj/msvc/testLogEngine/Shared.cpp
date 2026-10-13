
#include "Common.h"
#include "Shared.h"

// str is passed by value intentionally
std::string CutLogLine(std::string str)
{
	// # is a marker of ThreadID in log line pattern, because ':' can be present in TimeMacro as well (goes before ThreadMacro). 
	// ':' is a divider before MsgMacro
	size_t pos = str.find_first_of('#', 1);
	if (pos == std::string::npos) return str;

	pos = str.find_first_of(':', pos + 1);
	if (pos == std::string::npos) return str;

	return DelCRLF(str.erase(1ull, pos + 1));
}

/*
std::string millisecToStr(long long ms)
{
	int milliseconds = ms % 1000;
	int seconds = (ms / 1000) % 60;
	int minutes = (ms / 60000) % 60;
	int hours = (ms / 3600000) % 24;

	char buf[100];
#ifdef WIN32
	if (hours > 0)
		sprintf_s(buf, 100, "%u hours %u minutes %u seconds %u ms", hours, minutes, seconds, milliseconds);
	else if (minutes > 0)
		sprintf_s(buf, 100, "%u minutes %u seconds %u ms", minutes, seconds, milliseconds);
	else
		sprintf_s(buf, 100, "%u seconds %u ms", seconds, milliseconds);
#else
	if (hours > 0)
		sprintf(buf, "%u hours %u minutes %u seconds %u ms", hours, minutes, seconds, milliseconds);
	else if (minutes > 0)
		sprintf(buf, "%u minutes %u seconds %u ms", minutes, seconds, milliseconds);
	else
		sprintf(buf, "%u seconds %u ms", seconds, milliseconds);
#endif

	return buf;
}
*/
