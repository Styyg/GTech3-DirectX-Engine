#pragma once
#include <unordered_map>
#include <Windows.h>
#include <string>

class windowsMessageMap
{
public:
	windowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const;
private:
	std::unordered_map<DWORD, std::string> map;
};