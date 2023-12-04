#include "IoT_Taskv1.5.h"

using namespace std;

	//Task class constructors
	
	Task::Task(int TaskID, int taskFeature[4], string serverID){	//constructor with all Task properties
	
		this->TaskID = TaskID;		
		this->taskFeature[0] = 0;
		this->taskFeature[1] = 0;
		this->taskFeature[2] = 0;
		this->taskFeature[3] = 0;
		this->server.serverID = serverID;	
	}
	
	Task::Task(int TaskID, string serverID){	//constructor with 2 parameters
	
		this->TaskID = TaskID;
		this->server.serverID = serverID;
	}
