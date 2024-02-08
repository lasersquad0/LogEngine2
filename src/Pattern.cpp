#include "Pattern.h"

LOGENGINE_NS_BEGIN

std::string Pattern::Format(const LogEvent& event)
{
	std::string result;

	for (uint i = 0; i < holders.Count(); i++)
	{
		result.append(holders[i]->format(event));
	}

	return result;
}


void Pattern::parsePattern(const std::string& pattern)
{
	static THArray<std::string> placeHolders = { "", DateMacro, TimeMacro, DateTimeMacro, MessageMacro, ThreadMacro, AppNameMacro, AppVersionMacro, OSMacro, OSVersionMacro, LogLevelMacro };

	clearHolders();

	uint i = 0;
	while (i < pattern.size())
	{
		if (pattern[i] == '%')
		{
			std::string temp = "%";
			i++;

			while ((i < pattern.size()) && (pattern[i] != '%'))
				temp += pattern[i++];

			if (i < pattern.size())
				temp += pattern[i++];

			int j = placeHolders.IndexOf<CompareStringNCase>(temp);

			switch (j)
			{
			case 1:
				holders.AddValue(new DateHolder());
				break;
			case 2:
				holders.AddValue(new TimeHolder());
				break;
			case 3:
				holders.AddValue(new DateTimeHolder());
				break;
			case 4:
				holders.AddValue(new MessageHolder());
				break;
			case 5:
				holders.AddValue(new ThreadHolder());
				break;
			case 6:
				holders.AddValue(new AppNameHolder(DefaultAppName)); //TODO pass proper value to AppNameHolder
				break;
			case 7:
				holders.AddValue(new AppVersionHolder(DefaultAppVersion)); //TODO pass proper value to AppVersionHolder
				break;
			case 8:
				holders.AddValue(new OSHolder());
				break;
			case 9:
				holders.AddValue(new OSVersionHolder());
				break;
			case 10:
				holders.AddValue(new LogLevelHolder());
				break;

			default:
				holders.AddValue(new LiteralHolder(temp));
				break;
			}
		}
		else
		{
			std::string temp = "";
			while ((i < pattern.size()) && (pattern[i] != '%'))
				temp += pattern[i++];

			Holder* a = new LiteralHolder(temp);
			holders.AddValue(a);
		}
	}
}

void Pattern::clearHolders()
{
	for (uint i = 0; i < holders.Count(); i++)
	{
		Holder* h = holders[i];
		delete h;
	}

	holders.Clear();
}


LOGENGINE_NS_END
