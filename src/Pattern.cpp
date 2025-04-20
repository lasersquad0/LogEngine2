#include "LogEngine.h"
#include "Pattern.h"

LOGENGINE_NS_BEGIN

std::string Pattern::Format(const LogEvent& event)
{
	std::string result;

	for (uint i = 0; i < FHolders.Count(); i++)
	{
		result.append(FHolders[i]->Format(event));
	}

	return result;
}


void Pattern::parsePattern(const std::string& pattern)
{
	static const THArray<std::string> placeHolders = { "", DateMacro, TimeMacro, DateTimeMacro, MessageMacro, ThreadMacro, AppNameMacro, AppVersionMacro, OSMacro, OSVersionMacro, LogLevelMacro };

	FPattern = pattern;

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
				FHolders.AddValue(new DateHolder());
				break;
			case 2:
				FHolders.AddValue(new TimeHolder());
				break;
			case 3:
				FHolders.AddValue(new DateTimeHolder());
				break;
			case 4:
				FHolders.AddValue(new MessageHolder());
				break;
			case 5:
				FHolders.AddValue(new ThreadHolder());
				break;
			case 6:
				FHolders.AddValue(new AppNameHolder(GetProperty(APPNAME_PROPERTY, DefaultAppName)));
				break;
			case 7:
				FHolders.AddValue(new AppVersionHolder(GetProperty(APPVERSION_PROPERTY, DefaultAppVersion))); 
				break;
			case 8:
				FHolders.AddValue(new OSHolder());
				break;
			case 9:
				FHolders.AddValue(new OSVersionHolder());
				break;
			case 10:
				FHolders.AddValue(new LogLevelHolder());
				break;

			default:
				FHolders.AddValue(new LiteralHolder(temp));
				break;
			}
		}
		else
		{
			std::string temp = "";
			while ((i < pattern.size()) && (pattern[i] != '%'))
				temp += pattern[i++];

			Holder* a = new LiteralHolder(temp);
			FHolders.AddValue(a);
		}
	}
}

void Pattern::clearHolders()
{
	for (auto hld: FHolders)
	{
		delete hld;
	}

	/*for (uint i = 0; i < FHolders.Count(); i++)
	{
		Holder* h = FHolders[i];
		delete h;
	}*/

	FHolders.Clear();
}


LOGENGINE_NS_END
