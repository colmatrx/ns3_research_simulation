#include<iostream>
#include<string>
#include<vector>

using namespace std;

class Servers{
	
	public:
	string serverID;
	string serverResult;
	int serverFlag;
};	//end of Servers class


class Task{

	//class variables
	
	public:
	string TaskID;
	string TaskTimestamp;
	string TaskVotedResult;
	Servers server;	//an object of the class Servers above
	
	//class constructors
	
	Task(string TaskID, string TaskTimestamp, string serverID, int serverFlag){	//constructor with 4 parameters, exlcuding serverResult and TaskVotedResult.	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskVotedResult = "";
		this->server.serverID = serverID;
		this->server.serverResult = "";
		this->server.serverFlag = serverFlag;
	}
	
	Task(string TaskID, string TaskTimestamp, string serverID, string serverResult, int serverFlag){	//constructor with 5 parameters, exlcuding TaskVotedResult.	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskVotedResult = "";
		this->server.serverID = serverID;
		this->server.serverResult = serverResult;
		this->server.serverFlag = serverFlag;
	}
	
	Task(string TaskID, string TaskTimestamp, string TaskVotedResult, string serverID, string serverResult, int serverFlag){ //constructor with all 6 parameters	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskVotedResult = TaskVotedResult;
		this->server.serverID = serverID;
		this->server.serverResult = serverResult;			
		this->server.serverFlag = serverFlag;
	}	
	
};//end of class Task


class TaskToVerify{	//this class is for returning an object to the function selectTaskToVerify()
	
	public:
	string TaskID;
	string TaskVotedResult;
	
	//class constructor
	TaskToVerify(string TaskID, string TaskVotedResult){
	
		this->TaskID = TaskID;
		this->TaskVotedResult = TaskVotedResult;
	}
	
};	//end of class TaskToVerify

