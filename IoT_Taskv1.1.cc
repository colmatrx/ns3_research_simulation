#include<iostream>
#include<string>
#include<vector>

using namespace std;

class Servers{
	
	public:
	string serverID;
	int serverValue;
};



class Task{

	//class variables
	
	public:
	string TaskID;
	string clientID;
	string TaskTimestamp;
	string TaskResult;
	Servers server;	//an object of the class Servers above
	//vector<Servers> serverQueue;
	
	//class constructor
	
	Task(string TaskID, string clientID, string TaskTimestamp, string TaskResult, string serverID, int serverValue){	//pass 0 for serverValue for constructor
	
	
		this->TaskID = TaskID;
		this->clientID = clientID;
		this->TaskTimestamp = TaskTimestamp;
		this->TaskResult = TaskResult;
		this->server.serverID = serverID;	
		this->server.serverValue = serverValue;
	}
	
	Task(){	//class default constructor to set TaskResult to 0 so that I can print out the structure as conceived on paper
	
		this->TaskResult = "0";
	}
	
	//class member functions
	
	string getTaskID(Task taskObject){
	
		if (taskObject.TaskID != "")
			return taskObject.TaskID;	
			
		else
			return "NULL";	
	}	
	
	string getclientID(Task taskObject){
	
		if (taskObject.clientID != "")
			return taskObject.clientID;	
			
		else
			return "NULL";	
	}
	
};
