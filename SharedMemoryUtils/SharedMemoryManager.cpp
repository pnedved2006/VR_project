#include "SharedMemoryManager.h"
#include "WinBase.h"



SharedMemoryManager::SharedMemoryManager(void)
{
	MemoryName=L"Share Memory Name";
	mutex=CreateMutexW(NULL, false, L"basicmutex");
	
    hMemory=CreateFileMappingW((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,sizeof(double [BufferSize]),MemoryName);
	if (hMemory==NULL)
		OutputDebugStringW(L"error while mapping\n");

    Memory=(double (*)[BufferSize])MapViewOfFile(hMemory,FILE_MAP_WRITE,0,0,sizeof(double [BufferSize]));
    if (hMemory==NULL)
		OutputDebugStringW(L"error while mapping\n");
	OutputDebugStringW(L"\n\n\nfile succesfully mapped\n");
}


SharedMemoryManager::~SharedMemoryManager(void)
{
	UnmapViewOfFile(Memory); 
    CloseHandle(mutex);
}

double * SharedMemoryManager::readFile(){
	OutputDebugStringW(L"reading file\n");
	double * answer=new double[BufferSize];
	WaitForSingleObject(mutex,INFINITE);
	for (int i=0;i<BufferSize; i++){
		answer[i]=(*Memory)[i];
	}
	ReleaseMutex(mutex);
	return answer;
}

void SharedMemoryManager::writeFile(double * tab){
	WaitForSingleObject(mutex,INFINITE);
	for (int i=0;i<BufferSize; i++){
		(*Memory)[i]=tab[i];
	}
	ReleaseMutex(mutex);
}

double * SharedMemoryManager::readHeadPosition(){
	double * tab=this->readFile();
	double answer[2]={tab[0],tab[1]};
	return answer;
}

double * SharedMemoryManager::readCrossBowOrientation(){
	double * tab=this->readFile();
	double answer[4]={tab[2],tab[3],tab[4], tab[5]};
	return answer;
}

void SharedMemoryManager::writeCrossBowOrientation(double orientation[4]){
	WaitForSingleObject(mutex,INFINITE);
	(*Memory)[2]=orientation[0];
	(*Memory)[3]=orientation[1];
	(*Memory)[4]=orientation[2];
	(*Memory)[5]=orientation[3];
	ReleaseMutex(mutex);
}

void SharedMemoryManager::writeHeadPosition(double x, double y){
	WaitForSingleObject(mutex,INFINITE);
	(*Memory)[0]=x;
	(*Memory)[1]=y;
	ReleaseMutex(mutex);
}