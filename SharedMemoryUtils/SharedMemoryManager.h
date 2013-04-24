#pragma once
#include <Windows.h>
#include "iostream"

class SharedMemoryManager
{

static const LONG BufferSize=6;
LPCWSTR MemoryName;
double (*Memory)[BufferSize];
HANDLE hMemory;
HANDLE mutex;

public:
	SharedMemoryManager(void);
	~SharedMemoryManager(void);
	double * readFile();
	void writeFile(double * tab);

	void writeHeadPosition(double x, double y);
	void writeCrossBowOrientation(double w, double x, double y, double z);
	double * readHeadPosition();
	double * readCrossBowOrientation();
};

