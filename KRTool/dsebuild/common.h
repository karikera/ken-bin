#pragma once

#include <KR3/main.h>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>


using namespace std;

class Error
{
public:
	Error(ATTR_FORMAT const char * msg, ...);
	void print();

private:
	char m_message[1024];
};

extern wstring g_filename;
extern int g_line;

void must(char chr, char must);
