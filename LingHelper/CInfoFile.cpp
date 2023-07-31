#include "pch.h"
#include "CInfoFile.h"

CInfoFile::CInfoFile()
{
}

CInfoFile::~CInfoFile()
{
}

bool CInfoFile::ReadLogin(CString& name, CString& pwd)
{
	ifstream ifs;
	ifs.open(_F_LOGIN,ios::in);
	if (!ifs.is_open())
	{
		return false;
	}
	char buf[1024] = {};
	ifs.getline(buf, sizeof(buf));
	name = CString(buf);
	ifs.getline(buf, sizeof(buf));
	pwd = CString(buf);
	ifs.close();
	return true;
}

void CInfoFile::WritePwd(char* name, char* pwd)
{
	ofstream ofs;
	ofs.open(_F_LOGIN, ios::out);
	ofs << name<<endl;
	ofs << pwd <<endl;
	ofs.close();
}


