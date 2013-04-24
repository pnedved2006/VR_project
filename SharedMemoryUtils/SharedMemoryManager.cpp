#include "SharedMemoryManager.h"



SharedMemoryManager::SharedMemoryManager(void)
{
	MemoryName=L"Share Memory Name";
	mutex=CreateMutexW(NULL, false, L"basicmutex");

    hMemory=CreateFileMappingW((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,sizeof(double [BufferSize]),MemoryName);
	if (hMemory==NULL)
		printf("error while creating the memorymap file");

    Memory=(double (*)[BufferSize])MapViewOfFile(hMemory,FILE_MAP_WRITE,0,0,sizeof(double [BufferSize]));
    if (hMemory==NULL)
		printf("error while handleling the memorymap file");
}


SharedMemoryManager::~SharedMemoryManager(void)
{
	UnmapViewOfFile(Memory); 
    CloseHandle(mutex);
}

double * SharedMemoryManager::readFile(){
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
	double answer[4]={tab[2],tab[3],tab[4],tab[5]};
	return answer;
}

void SharedMemoryManager::writeCrossBowOrientation(double w, double x, double y, double z){
	WaitForSingleObject(mutex,INFINITE);
	(*Memory)[2]=w;
	(*Memory)[3]=x;
	(*Memory)[4]=y;
	(*Memory)[5]=z;
	ReleaseMutex(mutex);
}

void SharedMemoryManager::writeHeadPosition(double x, double y){
	WaitForSingleObject(mutex,INFINITE);
	(*Memory)[0]=x;
	(*Memory)[1]=y;
	ReleaseMutex(mutex);
}