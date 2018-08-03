#include <system/czlog.h>

#include <iostream>

using namespace std;

CZLogHelper::CZLogHelper(string &func_name)
{
    this->func_name = func_name;

	DebugLog<<func_name<<" {";
}

CZLogHelper::CZLogHelper(char const * const func_name)
{
    this->func_name = func_name;

	DebugLog<<func_name<<" {";
	DebugLog<<func_name<<"";
}

CZLogHelper::~CZLogHelper()
{
	DebugLog<<"} //"<<func_name;
	DebugLog<<func_name<<"";
}