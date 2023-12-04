#ifndef IOT_TASK_H
#define IOT_TASK_H

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
	
	//class constructor declarations
	
	Task(string TaskID, string TaskTimestamp, string serverID, int serverFlag);	//constructor with 4 parameters, exlcuding serverResult and TaskVotedResult.		
	
	Task(string TaskID, string TaskTimestamp, string serverID, string serverResult, int serverFlag);	//constructor with 5 parameters, exlcuding TaskVotedResult.	
	
	Task(string TaskID, string TaskTimestamp, string TaskVotedResult, string serverID, string serverResult, int serverFlag); //constructor with all 6 parameters	
		
};//end of class Task


class TaskToVerify{	//this class is for returning an object to the function selectTaskToVerify()
	
	public:
	string TaskID;
	string TaskVotedResult;
	
	//class constructor
	TaskToVerify(string TaskID, string TaskVotedResult);
	
};	//end of class TaskToVerify

#endif
