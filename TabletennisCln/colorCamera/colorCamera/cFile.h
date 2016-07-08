#pragma once

#include "time.h" // time
#include "stdio.h"

#include <io.h> //_access
#include"string.h"
#include "direct.h" // mkdir
#include "time.h" // time

#include "conio.h"//¼üÅÌ

#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")


class cFile
{
	char	fileName[100];
	FILE	*fp;
	bool	bFileCreate;
	int		fileNameIndex;
	char	DirName[100];
public:
	cFile();

	bool m_bStartSave;
	bool FileInit();
	void SaveTraceData(double x, double y, double z, double t, double vx, double vy, double vz, int flag);
};
