#include<iostream>
#include<vector>
#include<string>
#include<iomanip> //needed to use setw()
#include<sstream>
#include<algorithm>	//needed to use the find() fucntion of a vector
#include<map>
#include<functional>
#include<bits/stdc++.h>
#include "IoT_Taskv1.2.cc"

using namespace std;

bool sortByFrequency(pair<string, int> &a, pair<string, int> &b){	//this is sort function called in selectServers() as a comparator for sorting the vector of serverID and frequency pairs

	return (a.second < b.second);
}


class TaskQueue{ //start of TaskQueue class

public:
map <string, vector<Task>> taskMap;	//declare a map of key-value pair where key is the taskID and value is a vector of server objects

//this constructor initializes the Task Map data structure with as many servers as the integer arguments passed. It initializes the servers with a special TaskID 000, TaskTimeStamp 00:00
//and TaskResult of 000 using the naming convention of server(i). This TaskID 000 will be excluded from cleanUp so

TaskQueue(int numberOfServers){	//server naming convention is server1, server2.....servern

	cout<<"Server initialization started"<<endl<<endl;

	for (int i = 1; i <= numberOfServers; i++){
	
		Task task0("000", "00:00", "000", "server"+to_string(i), 1);	//contruct initialization task
		
		this->add_Update_Task(task0);	//add initialization task to the Queue
	}	
	
	cout<<"Server initialization completed. server1 through server"<<numberOfServers<<" initialized."<<endl<<endl;
}


int add_Update_Task(Task task){	//returns 1 if successful and -1 if unsuccessful. It prevents duplicate taskIDs and duplicate serverIDs

	map <string, vector<Task>>::iterator it;
	
	if (taskMap.size() == 0){	//if the map is empty....will be true for the first time the map is used
	
		taskMap[task.TaskID].push_back(task);
		
		cout<<"Map was found empty: First Task -> "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl<<endl;
		
		return 1;	
	}
		
	//this block updates the Task object's TaskResult and serverValue if TaskID and serverID are found ie if a server has seen this Task before
	
	it = taskMap.find(task.TaskID);	//find the taskID in the map
	
	if (it != taskMap.end()){ 		//true if the taskID is found in the map
	
		for (int i = 0; i < it->second.size(); i++){	//traverse the corresponding vector to search for the server ID
		
			if (it->second[i].server.serverID == task.server.serverID){	//if the serverID is found, update the corresponding task result and server value
			
				it->second[i].TaskResult = task.TaskResult;
				it->second[i].server.serverValue = task.server.serverValue;				
			
				cout<<"Inside taskFinder if block: Task "<<task.TaskID<< " has been seen by " <<task.server.serverID<<" before, therefore we have updated its task result and server value."<<endl<<endl;
				
				return 1;
			}		
		}
		
		//if taskID exists but serverID is not found for the same TaskID, then add the task for the calling task serverID
		
		taskMap[task.TaskID].push_back(task);
		
		cout<<"Existing Task "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl<<endl;
		
		return 1;	
	}
	
	//if the TaskID is not found in the map, it means it is a new task and should be added to the map for the corresponding server
	
	else if (it == taskMap.end()){
	
	taskMap[task.TaskID].push_back(task);
		
	cout<<"New Task -> "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl<<endl;
		
	return 1;
	
	}
	
	else 
		return -1;	//return -1 if all conditions failed
}



vector<string> getTaskServerIDsByID(string taskID){	//returns the vector of the serverIDs that have seen this TaskID before or NULL if taskID is not found
	
	vector<string> taskServerIDs; 			//declare a vector of string to store the found serverIDs for the given TaskiD
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = taskMap.find(taskID);			//check whether the taskID is in the map		
	
	if (it != taskMap.end()){ 			//true if the taskID is found in the map
	
		for (Task serverIDFinder : it->second){	//traverse the vector to search for each object's serverID	
		
			taskServerIDs.push_back(serverIDFinder.server.serverID);	//store each serverID of the task object in a vector		
		}	
		
		return taskServerIDs;	
	}
	
	//if taskID is not found, return a vector of one NULL taskServerID
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector"<<endl<<endl;
	
	taskServerIDs.push_back("NULL");	//this will be checked by comparing the first vector object to "NULL" e.g if (vector<string> serverIDVector[0] == "NULL")
		
	return taskServerIDs;	//return null pointer if object not found	
}


vector<Servers> getTaskServersByID(string taskID){	//returns the vector of the server objects that have seen this TaskID before or NULL if taskID is not found
	
	vector<Servers> taskServers; 			//declare a vector of Servers to store the found server objects for the given TaskID
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = taskMap.find(taskID);			//check whether the taskID is in the map		
	
	if (it != taskMap.end()){ 			//true if the taskID is found in the map
	
		for (Task serverFinder : it->second){	//traverse the vector to search for each object's server
		
			taskServers.push_back(serverFinder.server);	//store server of each task object in taskServers vector		
		}	
		
		return taskServers;	
	}
	
	//if taskID is not found, return a vector of one NULL server
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector"<<endl<<endl;
	
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
		
		return taskObjects;	
	}
	
	//if taskID is not found, return a vector of one NULL taskObject
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector as taskID"<<endl<<endl;
	
	Task invalidTask("NULL", "NULL", "NULL", "NULL", -1);
	
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
		
		cout<<"Inside updateTaskResult(), task result has been updated. "<<endl<<endl;
		
		return 1;	
	}
	
	//if taskID is not found, return -1
	
	cout<<"Task "<<taskID<<" not found. -1 is returned."<<endl<<endl;
	
	return -1;	
}



int cleanUp(float thresholdPercentage){		//receives float value as a percentage and deletes Tasks that that have been seen by the threshold percentage and above
							//it returns the number of TaskIDs deleted or 0 if no TaskID meets the thersholdPercentage argument. It returns -1 if thresholdPercentage is not 								//between 1 and 100

	set <string> serverIDSet;	//set to store unique serverIDs
	
	vector<string> serverIDsPerTaskID;	//keep a vector of serverIDs for each TaskID; clear this vector after each TaskID iteration
	
	vector<string> deleteVector;	//to store the list of TaskIDs to be deleted because deleting a TaskID in the middle of map iteration causes segmentation fault
	
	int serverIDSetCount;		//store the total number of servers in the data structure map, i.e size of the serverIDSet
	
	int nServer=0; int serverCount;			//nServer counts the number of server that have seen a TaskID
	
	float deleteThreshold;		//to compute the threshold for deleting the TaskID
	
	set <string>::iterator set_itr;	//declare an iterator to traverse through the set
	
	if ((thresholdPercentage < 1) or (thresholdPercentage > 100)){	//check for valid input range between 1 and 100
	
		cout<<"Error: Threshold out of range (1 to 100)."<<endl<<endl;
	
		return -1;
	}
	
	//make a set of all the servers in the data structure map
	
	for (auto it = taskMap.begin(); it != taskMap.end(); it++){	//loop to traverse the map
	
		if (it->first == "000")	//condition to skip the initialization TaskID 000 from the cleanUp process
			continue;
	
		for (int i = 0; i < it->second.size(); i++){		//loop to traverse the vector of the taskID key in the map
		
			serverIDSet.insert(it->second[i].server.serverID);	//insert each serverID into the set. Set stores unique copies and sort them in ascending order by default		
		}
	}
	
	serverIDSetCount = serverIDSet.size();	//returns the number of servers we have in the entire data structure
	
	//compare how many servers in the set with the servers that have seen each taskID
	
	for (auto it = taskMap.begin(); it != taskMap.end(); it++){	//loop to traverse the map
	
		if (it->first == "000")	//condition to skip the initialization TaskID 000 from the cleanUp process
			continue;
	
		for (int i = 0; i < it->second.size(); i++){		//loop to traverse the vector of each taskID key in the map
				
			serverIDsPerTaskID.push_back(it->second[i].server.serverID);	//push each serverID into the vector for every TaskID found in taskMap.		
		}
		
		for (set_itr = serverIDSet.begin(); set_itr != serverIDSet.end(); set_itr++){	//iterate through the serverIDset
		
			serverCount = count(serverIDsPerTaskID.begin(), serverIDsPerTaskID.end(), *set_itr);	//counts the number of occurences of each set element that occur the vector. It is 																expected to be 1 if it occurs, or 0 if not because the serverID is only stored 																once for each TaskID in the taskMap
			
			if (serverCount > 0){
			
				nServer++;	//increment nServer if the element of serverIDset is found in the vector of serverIDsPerTask					
			}				
		}
				
		deleteThreshold =  (float(nServer) / float(serverIDSetCount)) * 100;	//compute the percentage of servers that have seen a TaskID
		
		cout<<"Task "<<it->first<<" threshold is "<<deleteThreshold<<"%"<<endl<<endl;
		
		if (deleteThreshold >= thresholdPercentage){			//check if threshold is equal or greater than the argument threshold
		
			deleteVector.push_back(it->first);	//store the TaskID for deletion if it meets the threshold	
			
			cout<<"Delete threshold was met::Task "<<it->first<<" has been flagged for deletion"<<endl<<endl;
		}
		
		nServer = 0;	//reset nServer for the next TaskID
		
		serverIDsPerTaskID.clear();	//clear the vector before going through the next TaskID
	}
	
	//This block calls detele function on deleteVector to delete the flagged TaskIDs
	
	int deleteVectorSize = deleteVector.size();	//keep a copy of the size of the vector before performing delete operations on it
	
	if (deleteVectorSize == 0){
		
		return 0;
	}
	
	else{
	
		for (int i = 0; i < deleteVector.size(); i++){
		
			deleteTaskByID(deleteVector[i]); //call deleteTaskByID() on each TaskID in the deleteVector		
		}
	}
	
	return deleteVectorSize;
	
}//end of cleanUp() function



int deleteTaskByID(string taskID){

	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map

	it = taskMap.find(taskID);		//find the taskID in the map
	
	if (it != taskMap.end()){ 			//if taskID is found in the map
	
		taskMap.erase(it);			//erase taskID referenced by the iterator	
		
		cout<<"Task "<<taskID<<" has been deleted from the map"<<endl<<endl;
		
		return 1;	
	}
	
	cout<<"Task "<<taskID<<" not found in map. Nothing to delete."<<endl<<endl;
	
	return -1;
}


void displayQueue(){
	
	for (auto it = taskMap.begin(); it != taskMap.end(); it++){	//use of auto keyword
	
		cout<<endl;
		
		cout<<it->second[0].TaskID<<"\t"<<it->second[0].TaskTimestamp<<"\t"<<it->second[0].TaskResult<<"\t"<<it->second[0].server.serverID<<"\t";
		
		if (it->second.size() > 1){	//check to see if there is more objects in the vector to print after printing the first object above
		
			for (int i = 1; i < it->second.size(); i++){	//print other servers that have seen the current task
		
				cout<<it->second[i].server.serverID<<"\t";
			}	
		}
	}
	
	cout<<endl;
}


vector<string> selectServers(string currentTaskID, string currentServerID, int numberOfSelectServers){    //returns a vector of serverIDs if successful, or a vector of size of 0 if all servers have 														      //seen the currentTaskID

	set <string> serverIDSet;	//declare a set to store unique server IDs of the entire data structure
	
	set <string> excludedServerIDSet;	//declare a set to store the serverIDs to be excluded from the selected servers
	
	set <string>::iterator setxitr;	//iterator for excludedServerIDSet
	
	vector <string> allServerIDs;	//declare a vector of all server IDs to accommodate multiple entries
	
	vector <pair<string, int>> serverID_Frequency_Vector; //to store the serverID - frequncy pairs ie no of times a serverID occurs in the entire data structure map
	
	vector <string> servers_returned;		//vector to store the "numberOfSelectServers" returned per the argument
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	map <string, vector<Task>>::iterator itxset;	//iterator for excludedServerIDSet
	
	int frequency;			//counts how many times a serverID appears in the data structure map
		
	//collect the excluded serverIDs in a set for comparison with other serverIDs
	
	itxset = taskMap.find(currentTaskID);		//find the currentTaskID in the map
	
	if (itxset != taskMap.end()){ 	//if found, collect all the serverIDs that have seen it in a set
	
		for (int i = 0; i < itxset->second.size(); i++){
		
			excludedServerIDSet.insert(itxset->second[i].server.serverID);			
		}
	}
	
	excludedServerIDSet.insert(currentServerID);	//now add the currentServerID to the set since we want to exclude it from selected servers as well	
	
	//create a unique set of qualified serverIDs and an overallvector of serverIDs in this block		
	
	for (auto it = taskMap.begin(); it != taskMap.end(); it++){	//loop to traverse the map
	
		if (it->first == currentTaskID){	//this line skips the current TaskID from the iteration since we already collected its servers inside the excludedServerIDSet above
			continue;			//because they have all seen the currentTaskID
		}
	
		for (int i = 0; i < it->second.size(); i++){		//loop to traverse the vector of the corresponding taskID key in the map
		
			setxitr = excludedServerIDSet.find(it->second[i].server.serverID);	//find the serverID in the excludedServerIDSet
			
			if (setxitr == excludedServerIDSet.end()){	//true if serverID is not found in excludedServerIDSet
	
				serverIDSet.insert(it->second[i].server.serverID);	//insert each serverID into the set. Set stores unique copies and sort them in ascending order by default
				allServerIDs.push_back(it->second[i].server.serverID);	//insert each serverID into a vector	
			}
		}			
	}
	
	//count the number of times each serverID occur in the overall vector and store the pair in another vector
	
	set <string>::iterator set_itr;	//declare an iterator to traverse through the set
	
	for (set_itr = serverIDSet.begin(); set_itr != serverIDSet.end(); set_itr++){	//traverse the set of unique serverIDs
	
		frequency = count(allServerIDs.begin(), allServerIDs.end(), *set_itr);	//count how many times each serverID occur in the overall vector of allserverIDs
		
		serverID_Frequency_Vector.push_back(make_pair(*set_itr, frequency));		//store the serverID - frequency pair in a vector
	}	
	
	//sort the serverID_Frequency_Vector by frequency (value) in ascending order and return the first "int numberOfSelectServers" argument
	
	sort(serverID_Frequency_Vector.begin(), serverID_Frequency_Vector.end(), sortByFrequency);
	
	if (serverID_Frequency_Vector.size() == 0){	//if there is nothing in the vector
	
		return servers_returned;	//size of servers_returned here will be zero
	}
		
	
	if ((numberOfSelectServers) <= (serverID_Frequency_Vector.size())){	//if true, it is safe to return the number of selected servers
		
		for (int i = 0; i < numberOfSelectServers; i++){	//iterate the serverID_Frequency_Vector by the number passed as argument to the function
		
			servers_returned.push_back(serverID_Frequency_Vector[i].first);	//store the least used numberOfSelectServers in the vector servers_returned
		}
		
		return servers_returned;
	}
	
	if ((serverID_Frequency_Vector.size()) < (numberOfSelectServers)){	//if true, it is safer to return the number of servers in the serverID_Frequency_Vector
		
		for (int i = 0; i < serverID_Frequency_Vector.size(); i++){	//iterate the serverID_Frequency_Vector by the number passed as argument to the function
		
			servers_returned.push_back(serverID_Frequency_Vector[i].first);	//store the least used numberOfSelectServers in the vector servers_returned
		}
		
		return servers_returned;
	}
	
	return servers_returned;
	
}//end of selectServers() function	
		
};//end of TaskQueue class


int main(){

	TaskQueue myQueue(5);		//create object of class TaskQueue
	vector<string> TaskServers;	//vector variable to receive the result of getTaskServerIDsByID() function
	vector<Servers> servers;	//vector variable to receive the result of getTaskServersByID() function
	vector<Task> taskObj;		//vector variable to receive the result of getTaskObjectsByID() function
	int newtaskresult;		//to get the result of updateTaskResult()
	
	Task task1("101", "12:30", "33", "server1", 1);
	Task task2("102", "22:30", "35", "server2", 1);
	Task task3("101", "12:30", "33", "server2", 1);
	Task task4("102", "22:30", "35", "server1", 1);
	myQueue.add_Update_Task(task1);
	myQueue.add_Update_Task(task2);
	myQueue.add_Update_Task(task3);
	myQueue.add_Update_Task(task4);
	Task task5("103", "13:30", "73", "server1", 1);
	Task task6("118", "13:30", "73", "server6", 1);
	Task task7("128", "13:30", "73", "server2", 1);
	
	myQueue.add_Update_Task(task1);
	myQueue.add_Update_Task(task2);
	myQueue.add_Update_Task(task3);
	Task task8("101", "12:30", "89", "server1", 0);
	myQueue.add_Update_Task(task8);
	Task task9("118", "13:30", "73", "server1", 1);
	myQueue.add_Update_Task(task4);
	myQueue.add_Update_Task(task5);
	myQueue.add_Update_Task(task5);
	myQueue.add_Update_Task(task6);
	myQueue.add_Update_Task(task7);
	myQueue.add_Update_Task(task9);
	
	cout<<"Calling getTaskServerIDsByID(101) to get the servers that have seen Task 101 before."<<endl<<endl;
	
	TaskServers = myQueue.getTaskServerIDsByID("101");	//testing getTaskServerIDsByID() function with TaskID 101
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned NULL"<<endl<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl<<endl;
	}	
	
	cout<<"Calling getTaskServerIDsByID(103) to get the servers that have seen Task 103 before."<<endl<<endl;
	
	TaskServers = myQueue.getTaskServerIDsByID("103");	//testing getTaskServerIDsByID() function with TaskID 103
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned NULL"<<endl<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl<<endl;
	}
	
	cout<<"Calling getTaskServerIDsByID(102) to get the servers that have seen Task 102 before."<<endl<<endl;
	
	TaskServers = myQueue.getTaskServerIDsByID("102");	//testing getTaskServerIDsByID() function with TaskID 102
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned NULL"<<endl<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl<<endl;
	}
	
	cout<<"Calling getTaskServerIDsByID(108) to get the servers that have seen Task 108 before."<<endl<<endl;
	
	TaskServers = myQueue.getTaskServerIDsByID("108");	//testing getTaskServerIDsByID() function with TaskID 108
	
	for (int i=0; i<TaskServers.size(); i++){
		
		if (TaskServers[i] == "NULL"){
		
			cout<<"serverID returned "<<TaskServers[i]<<endl<<endl;
			break;
		}
		
		else		
			cout<<TaskServers[i]<<endl<<endl;
	}
	
	cout<<"Calling getTaskServersByID(101) to get the server objects from which we can get both server values and serverIDs."<<endl<<endl;
	
	servers = myQueue.getTaskServersByID("101");		//testing getTaskServersByID() function with TaskID 101	
	
	for(int i=0; i<servers.size(); i++){
	
		if(servers[i].serverID == "NULL"){
		
			cout<<"serverID is NULL. Vector is empty."<<endl<<endl;
			break;
		}
		
		else
			cout<<servers[i].serverID<<"\t"<<servers[i].serverValue<<endl<<endl;
	}
	
	cout<<"Calling getTaskObjectsByID(103) "<<endl<<endl;	//comment out this part
	
	taskObj = myQueue.getTaskObjectsByID("101");		//testing getTaskObjectsByID() function with TaskID 103	
	
	if (taskObj[0].TaskID == "NULL")
		cout<<"No valid task in vector. Task ID is "<<taskObj[0].TaskID<<endl<<endl;
		
	else{
	
		for (int i=0; i<taskObj.size(); i++){
		
			cout<<taskObj[i].TaskID<<"\t"<<taskObj[i].TaskTimestamp<<"\t"<<taskObj[i].TaskResult<<"\t"<<taskObj[i].server.serverID<<"\t"<<taskObj[i].server.serverValue<<endl;
		
		}
	}
	
	cout<<endl;
	
	cout<<"Calling updateTaskResult() to change result of task 101 to 909"<<endl<<endl;
	
	newtaskresult = myQueue.updateTaskResult("101", "909");	//testing updateTaskResult() function with TaskID 101 and TaskResult 909	
	
	if (newtaskresult == 1 ){
	
		taskObj = myQueue.getTaskObjectsByID("101");	
	
		if (taskObj[0].TaskID == "NULL")
			cout<<"No valid task in vector. Task ID is "<<taskObj[0].TaskID<<endl<<endl;
		
		else{
	
			for (int i=0; i<taskObj.size(); i++){
		
			cout<<taskObj[i].TaskID<<"\t"<<taskObj[i].TaskTimestamp<<"\t"<<taskObj[i].TaskResult<<"\t"<<taskObj[i].server.serverID<<"\t"<<taskObj[i].server.serverValue<<endl;
		
			}
		}		
	
	}
	
	//this block tests the deleteTaskByID() function
	
	cout<<endl;
	
	cout<<"Calling deleteTaskByID(103) function"<<endl<<endl;
	
	int val = myQueue.deleteTaskByID("103");	//testing deleteTaskByID() function with TaskID 103
	
	if (val == 1){
	
		cout<<"After removing task 103, Queue now contains:"<<endl<<endl;
		
		myQueue.displayQueue();	//display the map to confirm Task 103 has been deleted
		
	}
	
	else
		cout<<"No Task to be deleted found";
		
	cout<<endl;
	
	cout<<"Calling displayQueue"<<endl<<endl;
	
	myQueue.displayQueue();
	
	cout<<endl<<"Calling selectServers() function with an arguments of 101, server2, 3"<<endl<<endl;
		
	vector<string> selectedServers;
		
	selectedServers = myQueue.selectServers("101", "server2", 3);
	
	cout<<"servers selceted are "<<selectedServers.size()<<" and listed below"<<endl<<endl;
	
	for (int i=0; i<selectedServers.size(); i++){
	
		cout<<selectedServers[i]<<endl<<endl;
	}
		
	cout<<"Calling cleanUp() function with 50% threshold..."<<endl<<endl;
	
	myQueue.cleanUp(50);
	
	myQueue.displayQueue();
	
	return 0;
}
