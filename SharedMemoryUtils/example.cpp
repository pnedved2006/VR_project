#include <iostream>
#include <stdio.h>
#include "SharedMemoryManager.h"

using namespace std;

int main(int argc, char * argv[]){
	// instanciate the manager of the shared memory
	SharedMemoryManager * mem=new SharedMemoryManager();

	// examples : 
	// write head position (2 numbers between 0 and 1) in the shared memory
	mem->writeHeadPosition(0.1,0.3);
	// write crossbow orientation (4 doubles, if we use the system of Ogre)
	//mem->writeCrossBowOrientation(0.1,0.2,0.3,0.4);
	
	// read head position every 200ms
	double alpha[4]={0,0,0,0};
	while(true)
    {
		alpha[0]++;
		mem->writeCrossBowOrientation(alpha);
		cout<<alpha[0]<<"\n";
		Sleep(200);
    }

	// the same method (readCrossBowOrientation) exists to get the crossbow orientation stored in 
	// the shared folder

	delete mem;
	system("pause");
	return 0;
}