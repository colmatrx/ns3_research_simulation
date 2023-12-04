#include<iostream>
#include<string>
#include<vector>

using namespace std;

class Servers{
	
	public:
	string serverID;
	int serverValue;
};	//end of Servers class

class TaskToVerify{	//this class is for returning an object to the function selectTaskToVerify()
	
	public:
	string TaskID;
	string TaskResult;
	
	//class constructor
	TaskToVerify(string TaskID, string TaskResult){
	
		this->TaskID = TaskID;
		this->TaskResult = TaskResult;
	}
	
};	//end of class TaskToVerify


class Task{

	//class variables
	
	public:
	string TaskID;
	string TaskTimestamp;
	string TaskResult;
	Servers server;	//an object of the class Servers above
	//vector<Servers> serverQueue;
	
	//class constructor
	
	Task(string TaskID, string TaskTimestamp, string TaskResult, string serverID, int serverValue){	//pass 0 for serverValue for constructor
	
	
		this->TaskID = TaskID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskResult = TaskResult;
		this->server.serverID = serverID;	
		this->server.serverValue = serverValue;
	}
	
	
};//end of class Task

