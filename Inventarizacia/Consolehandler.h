#pragma once
#include "data.h"

#define CONSOLEH_ALL_MISSING 1




class Consolehandler
{
public:
	Consolehandler(Data* data);
	std::vector<std::string> menues;

	void setcenter();
	void mainmenu();
	void scan();
	void listtoconsole(std::vector<std::string>& source);
	void listtoconsole(std::vector<data>& source);
	void listtoconsole(found& source);
	void listtoconsole(stats& source);
	void listtoconsole(data& source,int type);

public:
	Data* datahandle;
};


