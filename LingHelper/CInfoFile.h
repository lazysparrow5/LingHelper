#pragma once
#include <list>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#define _F_LOGIN "./login.ini"

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();
	bool ReadLogin(CString &name, CString &pwd);
	void WritePwd(char *name, char *pwd);
};
