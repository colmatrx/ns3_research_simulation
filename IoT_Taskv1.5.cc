#include "IoT_Taskv1.5.h"

using namespace std;

	//Task class constructors
	
	Task::Task(string TaskID, string TaskTimestamp, string serverID, int serverFlag){	//constructor with 4 parameters, exlcuding serverResult and TaskVotedResult.	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskVotedResult = "";
		this->server.serverID = serverID;
		this->server.serverResult = "";
		this->server.serverFlag = serverFlag;
	}
	
	Task::Task(string TaskID, string TaskTimestamp, string serverID, string serverResult, int serverFlag){	//constructor with 5 parameters, exlcuding TaskVotedResult.	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskVotedResult = "";
		this->server.serverID = serverID;
		this->server.serverResult = serverResult;
		this->server.serverFlag = serverFlag;
	}
	
	Task::Task(string TaskID, string TaskTimestamp, string TaskVotedResult, string serverID, string serverResult, int serverFlag){ //constructor with all 6 parameters	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskVotedResult = TaskVotedResult;
		this->server.serverID = serverID;
		this->server.serverResult = serverResult;			
		this->server.serverFlag = serverFlag;
	}	
		
	//TaskToVerify class constructor
	TaskToVerify::TaskToVerify(string TaskID, string TaskVotedResult){
	
		this->TaskID = TaskID;
		this->TaskVotedResult = TaskVotedResult;
	}
