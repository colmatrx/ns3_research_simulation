#include<iostream>
#include<vector>
#include<string>
#include<iomanip> //needed to use setw()
#include<sstream>
#include<algorithm>	//needed to use the find() fucntion of a vector
#include<map>
#include<functional>
#include<bits/stdc++.h>
#include "IoT_Taskv1.1.cc"

using namespace std;

class TaskQueue{ //start of TaskQueue class

public:
map <string, vector<Task>> taskMap;	//declare a map of key-value pair where key is the taskID and value is a vector of server objects


int addTask(Task task){	//returns 1 if successful and -1 if unsuccessful. It prevents duplicate taskIDs and duplicate serverIDs

	map <string, vector<Task>>::iterator it;
	
	if (taskMap.size() == 0){	//if the map is empty....will be true for the first time the map is used
	
		taskMap[task.TaskID].push_back(task);
		
		cout<<"Map was found empty: First Task -> "<<task.TaskID<<" has been added to the map."<<endl;
		
		return 1;	
	}
		
	//if above condition is not true, then we make sure the task.serverID is not existing in the queue before adding the task
	
	it = taskMap.find(task.TaskID);	//find the taskID in the map
	
	if (it != taskMap.end()){ 		//true if the taskID is found in the map
	
		for (Task taskFinder : it->second){	//traverse the corresponding vector to search for the server ID
		
			if (taskFinder.server.serverID == task.server.serverID){	//if there is match of the same taskID and serverID, we cannot add the task
			
				cout<<"Inside taskFinder if block: Task "<<task.TaskID<< " has been seen by " <<task.server.serverID<<" before. Cannot add same task for the same server. Consider update function"<<endl;
				
				return -1;
			}		
		}
		
		//if taskID exists but serverID is not found for the same TaskID, then add the task for the calling task serverID
		
		taskMap[task.TaskID].push_back(task);
		
		cout<<"Existing Task "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl;
		
		return 1;	
	}
	
	//if the TaskID is not found in the map, it means it is a new task and should be added to the map for the corresponding server
	
	taskMap[task.TaskID].push_back(task);
		
	cout<<"New Task -> "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl;
		
	return 1;
}



vector<string> getTaskServerIDsByID(string taskID){	//returns the vector of the serverIDs that have seen this TaskID before or NULL if taskID is not found
	
	vector<string> taskServerIDs; 			//declare a vector of string to store the found serverIDs for the given TaskiD
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = taskMap.find(taskID);			//check whether the taskID is in the map		
	
	if (it != taskMap.end()){ 			//true if the taskID is found in the map
	
		for (Task serverIDFinder : it->second){	//traverse the vector to search for each object's serverID	
		
			taskServerIDs.push_back(serverIDFinder.server.serverID);	//store each serverID of the task object in a vector		
		}	
		
		cout<<"Inside getTaskServerIDsByID(), after the serverIDFinder for loop "<<endl;
		
		return taskServerIDs;	
	}
	
	//if taskID is not found, return a vector of one NULL taskServerID
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector"<<endl;
	
	taskServerIDs.push_back("NULL");	//this will be checked by comparing the first vector object to "NULL" e.g if (vector<string> serverIDVector[0] == "NULL")
		
	return taskServerIDs;	//return null pointer if object not found	
}


vector<Servers> getTaskServersByID(string taskID){	//returns the vector of the servers that have seen this TaskID before or NULL if taskID is not found
	
	vector<Servers> taskServers; 			//declare a vector of Servers to store the found server objects for the given TaskID
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = taskMap.find(taskID);			//check whether the taskID is in the map		
	
	if (it != taskMap.end()){ 			//true if the taskID is found in the map
	
		for (Task serverFinder : it->second){	//traverse the vector to search for each object's server
		
			taskServers.push_back(serverFinder.server);	//store server of each task object in taskServers vector		
		}	
		
		cout<<"Inside getTaskServersByID(), after the serverIDFinder for loop "<<endl;
		
		return taskServers;	
	}
	
	//if taskID is not found, return a vector of one NULL server
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector"<<endl;
	
	Servers serv; serv.serverID = "NULL"; serv.serverValue = -1;	//construct a Server object of serverID NULL and serverValue -1 in order to be able to return a Server object
	
	taskServers.push_back(serv);	//push the invalid Server object to the vecotr if the passed TaskID is not found in the map
		
	return taskServers;	//this should be checked when called by comparing the first vector object to "NULL" e.g if (vector<Servers> serverVector[0].serverID == "NULL")	
}


vector<Task> getTaskObjectsByID(string taskID){	//returns the vector of all the tasks that are associated with this taskID
	
	vector<Task> taskObjects; 			//declare a vector of Task to store the found task objects for the given TaskiD
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = taskMap.find(taskID);			//check whether the taskID is in the map		
	
	if (it != taskMap.end()){ 			//true if the taskID is found in the map
	
		for (Task taskFinder : it->second){	//traverse the vector to search for each object	
		
			taskObjects.push_back(taskFinder);	//store each object in the taskObject vector		
		}	
		
		cout<<"Inside getTaskObjectsByID(), after the serverIDFinder for loop "<<endl;
		
		return taskObjects;	
	}
	
	//if taskID is not found, return a vector of one NULL taskObject
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector as taskID"<<endl;
	
	Task invalidTask("NULL", "NULL", "NULL", "NULL", "NULL", -1);
	
	taskObjects.push_back(invalidTask);	//this may be checked when called by comparing the first vector object taskID to "NULL" e.g if (vector<Task> taskObject[0].taskID == "NULL")
		
	return taskObjects;	//return null pointer if object not found	
}

int updateTaskResult(string taskID, string newTaskResult){	//updates the result for the given taskID, returns 1 on success, and -1 on failure

	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = taskMap.find(taskID);			//check whether the taskID is in the map		
	
	if (it != taskMap.end()){ 			//true if the taskID is found in the map
	
		for (int i = 0; i < it->second.size(); i++){	//traverse the vector to search for each object's taskResult	
		
			it->second[i].TaskResult = newTaskResult;	//store new TaskResult of the taskID in the object queue		
		}	
		
		cout<<"Inside updateTaskResult(), task result has been updated. "<<endl;
		
		return 1;	
	}
	
	//if taskID is not found, return -1
	
	cout<<"Task "<<taskID<<" not found. -1 is returned."<<endl;
	
	return -1;	
}


int deleteTaskByID(string taskID){

	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map

	it = taskMap.find(taskID);		//find the taskID in the map
	
	if (it != taskMap.end()){ 			//if taskID is found in the map
	
		taskMap.erase(it);			//erase taskID referenced by the iterator	
		
		cout<<"Task "<<taskID<<" has been deleted from the map"<<endl;
		
		return 1;	
	}
	
	cout<<"Task "<<taskID<<" not found in map. Nothing to delete."<<endl;
	
	return -1;
}

void displayQueue(){
	
	for (auto it = taskMap.begin(); it != taskMap.end(); it++){	//use of auto keyword
	
		cout<<endl;
		
		cout<<it->second[0].TaskID<<"\t"<<it->second[0].clientID<<"\t"<<it->second[0].TaskTimestamp<<"\t"<<it->second[0].TaskResult<<"\t"<<it->second[0].server.serverID<<"\t";
		
		if (it->second.size() > 1){	//check to see if there is more objects in the vector to print after printing the first object above
		
			for (int i = 1; i < it->second.size(); i++){	//print other servers that have seen the current task
		
				cout<<it->second[i].server.serverID<<"\t";
			}	
		}
	}
	
	cout<<endl;
}


int selectServers();
		
};	//end of TaskQueue class

int main(){

	TaskQueue myQueue;		//create object of class TaskQueue
	vector<string> TaskServers;	//vector variable to receive the result of getTaskServerIDsByID() function
	vector<Servers> servers;	//vector variable to receive the result of getTaskServersByID() function
	vector<Task> taskObj;		//vector variable to receive the result of getTaskObjectsByID() function
	int newtaskresult;		//to get the result of updateTaskResult()
	
	Task task1("101", "21", "12:30", "33", "server1", 1);
	Task task2("102", "22", "22:30", "35", "server2", 1);
	Task task3("103", "23", "13:30", "73", "server3", 1);
	Task task4("101", "23", "13:30", "33", "server2", 1);
	Task task5("103", "23", "13:30", "73", "server1", 1);
	Task task6("118", "23", "13:30", "73", "server6", 1);
	Task task7("128", "23", "13:30", "73", "server2", 1);
	
	myQueue.addTask(task1);
	myQueue.addTask(task2);
	myQueue.addTask(task3);
	myQueue.addTask(task1);
	myQueue.addTask(task4);
	myQueue.addTask(task5);
	myQueue.addTask(task5);
	myQueue.addTask(task6);
	myQueue.addTask(task7);
	
	TaskServers = myQueue.getTaskServerIDsByID("101");	//testing getTaskServerIDsByID() function with TaskID 101
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned NULL"<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl;
	}	
	
	TaskServers = myQueue.getTaskServerIDsByID("103");	//testing getTaskServerIDsByID() function with TaskID 103
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned NULL"<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl;
	}
	
	TaskServers = myQueue.getTaskServerIDsByID("102");	//testing getTaskServerIDsByID() function with TaskID 102
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned NULL"<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl;
	}
	
	TaskServers = myQueue.getTaskServerIDsByID("108");	//testing getTaskServerIDsByID() function with TaskID 108
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned "<<TaskServers[i]<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl;
	}
	
	cout<<"Calling getTaskServers() "<<endl;
	
	servers = myQueue.getTaskServersByID("101");		//testing getTaskServersByID() function with TaskID 101	
	
	for(int i=0; i<servers.size(); i++){
	
		if(servers[i].serverID == "NULL"){
		
			cout<<"serverID is NULL. Vector is empty."<<endl;
			break;
		}
		
		else
			cout<<servers[i].serverID<<"\t"<<servers[i].serverValue<<endl;
	}
	
	cout<<"Calling getTaskObjectsByID() "<<endl;
	
	taskObj = myQueue.getTaskObjectsByID("103");		//testing getTaskObjectsByID() function with TaskID 103	
	
	if (taskObj[0].TaskID == "NULL")
		cout<<"No valid task in vector. Task ID is "<<taskObj[0].TaskID<<endl;
		
	else{
	
		for (int i=0; i<taskObj.size(); i++){
		
			cout<<taskObj[i].TaskID<<"\t"<<taskObj[i].clientID<<"\t"<<taskObj[i].TaskTimestamp<<"\t"<<taskObj[i].TaskResult<<"\t"<<taskObj[i].server.serverID<<"\t"<<taskObj[i].server.serverValue<<endl;
		
		}
	}
	
	cout<<"Calling updateTaskResult() to change result of task 101 to 909"<<endl;
	
	newtaskresult = myQueue.updateTaskResult("101", "909");	//testing updateTaskResult() function with TaskID 101 and TaskResult 909	
	
	if (newtaskresult == 1 ){
	
		taskObj = myQueue.getTaskObjectsByID("101");	
	
		if (taskObj[0].TaskID == "NULL")
			cout<<"No valid task in vector. Task ID is "<<taskObj[0].TaskID<<endl;
		
		else{
	
			for (int i=0; i<taskObj.size(); i++){
		
			cout<<taskObj[i].TaskID<<"\t"<<taskObj[i].clientID<<"\t"<<taskObj[i].TaskTimestamp<<"\t"<<taskObj[i].TaskResult<<"\t"<<taskObj[i].server.serverID<<"\t"<<taskObj[i].server.serverValue<<endl;
		
			}
		}		
	
	}
	
	//this block tests the deleteTaskByID() function
	
	int val = myQueue.deleteTaskByID("101");	//testing deleteTaskByID() function with TaskID 101
	
	if (val == 1){
	
		cout<<"After removing task 101, Queue now contains:"<<endl;
		
		myQueue.displayQueue();	//display the map to confirm Task 101 has been deleted
		
	}
	
	else
		cout<<"No Task to be deleted found";
	
	return 0;
}

