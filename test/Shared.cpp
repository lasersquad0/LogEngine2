
#include "Common.h"
#include "Shared.h"

using namespace LogEngine;

std::string CutLogLine(const std::string& str)
{
	// # is a marker of ThreadID in log line pattern, because ':' can be present in TimeMacro as well (goes before ThreadMacro). 
	// ':' is a divider before MsgMacro
	size_t pos = str.find_first_of('#', 1);
	if (pos == std::string::npos) return LogEngine::DelCRLF(str);

	pos = str.find_first_of(':', pos + 1);
	if (pos == std::string::npos) return LogEngine::DelCRLF(str);

	return LogEngine::DelCRLF(str.substr(pos + 1));
}


