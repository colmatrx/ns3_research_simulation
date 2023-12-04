#include <iostream>
#include <vector>
#include <string>
#include <iomanip> //needed to use setw()
#include <sstream>
#include <algorithm>	//needed to use the find() fucntion of a vector
#include <map>
#include <functional>
#include<set>
#include "IoT_TaskQueuev3.4.h"
#include "IoT_Taskv1.5.h"

using namespace std;

bool sortByFrequency(pair<string, int> &a, pair<string, int> &b){	//this is sort function called in selectServers() as a comparator for sorting the vector of serverID and frequency pairs

	return (a.second < b.second);
}

bool sortElectionResult_DescendingOrder(pair<string, int> &a, pair<string, int> &b){	//sort function to sort voted task results (result - frequency pairs) in descending order

	return (a.second > b.second);	
}

// Default constructor
TaskQueue::TaskQueue (){

}

//this constructor initializes the ProbeServerMap data structure with as many servers as the integer arguments passed using the naming convention of server(i).

TaskQueue::TaskQueue(int numberOfServers){	//server naming convention is server1, server2.....servern

	cout<<endl<<"Server initialization started"<<endl<<endl;

	for (int i = 1; i <= numberOfServers; i++){
		
		initializeProbeServer("node"+to_string(i));	//call initializeProbeServer to initialize the serverID - frequency pair in a vector with frequency of 0		
	}	
	
	cout<<"Server initialization completed. server1 through server"<<numberOfServers<<" initialized."<<endl<<endl;
}

//this constructor initializes the ProbeServerMap data structure with the names provided by the client. The names should be provided as a vector of string
TaskQueue::TaskQueue(vector<string> nodeIDs){	//takes a vector of node IDs and initializes them

	// cout<<endl<<"node initialization started"<<endl<<endl;

	for (auto i = 0; i < nodeIDs.size(); i++){
		
		initializeProbeServer(nodeIDs[i]);	//call initializeProbeServer to initialize the nodes with their names and frequency pair in a vector with frequency of 0		
	}	
	
	// cout<<"node initialization completed. All nodes have been initialized."<<endl<<endl;
}

int TaskQueue::checkBlacklist(string serverID){		//sort and checks the blacklist for a serverID....returns 1 if serverID is found and 0 if not found
	
	auto it = find(blacklistedServers.begin(), blacklistedServers.end(), serverID);	//find the serverID in the blacklist
	
	if (it != blacklistedServers.end())	//if serverID is found
		return 1;
	else				//if serverID is not found
		return 0;
}

int TaskQueue::addMaliciousServerToBlacklist(string serverID){	//adds a serverID to the blacklist; returns 1 if serverID was added or 0 otherwise

	auto it = find(blacklistedServers.begin(), blacklistedServers.end(), serverID);	//find the serverID in the blacklist
	
	if (it != blacklistedServers.end()){	//check to see if serverID is in the vector already
		
		cout<<serverID<<" is already found in blacklist"<<endl;	
		return 0;	//
	}
	
	else{	
		blacklistedServers.push_back(serverID);	//add the serverID to the vector
		sort (blacklistedServers.begin(), blacklistedServers.end()); //keep a sorted blacklist in ascending order so checkBlacklist() will be faster
		return 1;
	}
}

int TaskQueue::removeMaliciousServerFromBlacklist(string serverID){	//removes a serverID from the blacklist; returns 1 if successful and 0 on failure

	auto it = find(blacklistedServers.begin(), blacklistedServers.end(), serverID);	//find the serverID in the blacklist
	
	if (it != blacklistedServers.end()){	//check to see if serverID is in the vector already
		
		blacklistedServers.erase(it);
		cout<<serverID<<" has been removed from the blacklist"<<endl;	
		return 1;	//
	}
	
	else{	
		cout<<serverID<<" not found in blacklist, nothing to erase"<<endl;
		return 0;
	}
}

//you may also call initializeProbeServer() whenever client discovers a new server that is not part of the existing probing servers

int TaskQueue::initializeProbeServer(string serverID){	//it returns 1 if initialization is successful, it returns 0 if serverID already exists and -1 if it is found in the blacklist

	if (checkBlacklist(serverID) == 0){	//check that the serverID is not blacklisted before attempting to initialize it; 0 means not in blacklist
	
		auto it = ProbeServerMap.find(serverID);  //check if serverID is in the map. 

		if (it == ProbeServerMap.end()){	//if serverID is not found

			ProbeServerMap.insert(pair<string, int>(serverID, 0));	//initialize a new serverID with a frequency of 0 in the ProbeServerMap
			return 1;	
		}
		
		else{	//if serverID is found, do nothing because it is already initialized
			
			cout<<it->first<<" exists in ProbeServerMap; cannot be initialized"<<endl;
			return 0;
		}
	}
	
	else{
	
		cout<<serverID<<" is blacklisted; cannot be initialized"<<endl;
		return -1;	
	}
}

//this function should be called by client after it has got some servers from selectProbeServers() and has sent out a probe tasks to them, so as to get ProbeServerMap up to date

int TaskQueue::updateProbeServerFrequency(vector<string> serverIDs){	//takes a vector of serverIDs; returns 1 if successful and -1 if it fails

	int updateTracker = 0; int initializationTracker = 0;

	for (auto it = serverIDs.begin(); it != serverIDs.end(); it++){
		
		const auto &serverIndex = ProbeServerMap.find(*it);	//finds the serverID in the map
	
		if (serverIndex != ProbeServerMap.end()){	//true if serverID is found in map
		
			serverIndex->second++; //increment the serverID frequency value
			updateTracker++;
		}
		
		else{
			if (initializeProbeServer(*it) == 1){ 	//else part means it's a new server so call initializeProbeServer() to initialize it and error check initializeProbeServer()
				cout<<*it<<" is a new server, it cannot be updated but has just been initialized"<<endl;
				initializationTracker++;
			}
		}
	}
	
	if ((updateTracker > 0) || (initializationTracker > 0))	//return 1 if either update or initialize succeeds
		return 1;
	else
		return -1;
}

//overloaded updateProbeServerFrequency() to accept string

int TaskQueue::updateProbeServerFrequency(string serverID){	//it returns 1 if updating is successful, it returns 0 if initializeProbeServer() was called and -1 if it fails
		
	const auto &serverIndex = ProbeServerMap.find(serverID);	//finds the serverID in the map
	
	if (serverIndex != ProbeServerMap.end()){	//true if serverID is found in map
		
		serverIndex->second++; //increment the serverID frequency value
		return 1;
	}
	
	else{
		if (initializeProbeServer(serverID) == 1){	 //else part means it's a new server so call initializeProbeServer() to initialize it.
			cout<<serverID<<" is a new server, cannot be updated but has just been initialized"<<endl;
			return 0;
		}
	}
	
	return -1;	
}

vector<string> TaskQueue::selectProbeServers(string currentServerID, int numberOfSelectedServers){    //returns a vector of serverIDs if successful, or a vector of size of 0 if no server to return 												   
	vector <pair<string, int>> serverID_Frequency_Vector; //for sorting the current map serverID-frequency pairs
	
	vector <string> servers_returned;		//vector to store the "numberOfSelectServers" returned per the argument
	
	serverID_Frequency_Vector.clear();
	servers_returned.clear();
	
	for (auto it = ProbeServerMap.begin(); it != ProbeServerMap.end(); it++){	//iterate the ProbeServerMap to copy its content to the sorting vector below, excluding the current serverID
	
		if (it->first == currentServerID)	//exclude the currentServerID from the sorting vector
			continue;
				
		serverID_Frequency_Vector.push_back(make_pair(it->first, it->second));	//store the serverID - frequency pair in a vector
	}
	
	//sort the serverID_Frequency_Vector by frequency (value) in ascending order and return a vector of the first "numberOfSelectServers" argument
	
	sort(serverID_Frequency_Vector.begin(), serverID_Frequency_Vector.end(), sortByFrequency);	//sort the serverID - frequency pairs by frequency
	
	if (serverID_Frequency_Vector.size() == 0){	//if there is nothing in the vector
	
		return servers_returned;	//size of servers_returned here will be zero; this is a way to error check selectProbeServers() when called
	}		
	
	if ((numberOfSelectedServers) <= (serverID_Frequency_Vector.size())){	//if true, it is safe to return the number of selected servers
		
		for (int i = 0; i < numberOfSelectedServers; i++){	//iterate the serverID_Frequency_Vector by the number passed as argument to the function
		
			servers_returned.push_back(serverID_Frequency_Vector[i].first);	//store the least used numberOfSelectServers argument in the vector servers_returned
		}
		
		return servers_returned;
	}
	
	if ((serverID_Frequency_Vector.size()) < (numberOfSelectedServers)){	//if true, it is safer to return the number of servers in the serverID_Frequency_Vector
		
		for (int i = 0; i < serverID_Frequency_Vector.size(); i++){	//iterate the serverID_Frequency_Vector by the number passed as argument to the function
		
			servers_returned.push_back(serverID_Frequency_Vector[i].first);	//store the least used numberOfSelectServers in the vector servers_returned
		}
		
		return servers_returned;
	}
	
	return servers_returned;
	
}//end of selectProbeServers() function	


string TaskQueue::displayProbeServer_Frequency(){	//to display the probing servers map data structure

	ostringstream outputStream;	//to store concatenated strings
	string outputString;			//to print concatenated string
	
	// outputStream.str("");
	// outputString.clear();

	outputStream<<"Server Frequencies for Probing (node#[freq]): ";

	for (auto it = ProbeServerMap.begin(); it != ProbeServerMap.end(); it++){	
	
		outputStream<<left<<it->first<<"["<<it->second<<"]   ";		
	}
	
	outputString = outputStream.str();
	return outputString;
}


int TaskQueue::add_Update_Task(Task task){	//returns 1 if successful and -1 if unsuccessful. It prevents duplicate taskIDs and duplicate serverIDs

	map <string, vector<Task>>::iterator it;
	
	if (ProbeQueue.size() == 0){	//if the map is empty....will be true for the first time the map is used
	
		ProbeQueue[task.TaskID].push_back(task);
		
		// cout<<"Map was found empty: First Task -> "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl<<endl;
		
		return 1;	
	}
		
	//this block updates the Task object's server.serverResult and serverFlag if TaskID and serverID are found ie if a server has seen this Task before
	
	it = ProbeQueue.find(task.TaskID);	//find the taskID in the map
	
	if (it != ProbeQueue.end()){ 		//true if the taskID is found in the map
	
		for (int i = 0; i < it->second.size(); i++){	//traverse the corresponding vector to search for the server ID
		
			if (it->second[i].server.serverID == task.server.serverID){	//if the serverID is found, update the corresponding task result and server value
			
				it->second[i].server.serverResult = task.server.serverResult;
				it->second[i].server.serverFlag = task.server.serverFlag;				
			
				// cout<<"Inside taskFinder if block: Task "<<task.TaskID<< " has been seen by " <<task.server.serverID<<" before, therefore we have updated its server result and server flag."<<endl<<endl;				
				return 1;
			}		
		}
		
		//if taskID exists but serverID is not found for the same TaskID, then add the task for the calling task serverID
		
		ProbeQueue[task.TaskID].push_back(task);
		
		// cout<<"Existing Task "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl<<endl;
		
		return 1;	
	}
	
	//if the TaskID is not found in the map, it means it is a new task and should be added to the map for the corresponding server
	
	else if (it == ProbeQueue.end()){
	
		ProbeQueue[task.TaskID].push_back(task);
			
		// cout<<"New Task -> "<<task.TaskID<<" has been added to the map for "<<task.server.serverID<<endl<<endl;
			
		return 1;	
	}
	
	else 
		return -1;	//return -1 if all conditions failed
}

int TaskQueue::blacklistMaliciousWorkingServer(string serverID){	//removes serverID from ProbeServerMap and adds it to the blacklist; should be called when a server fails verification

	auto it = ProbeServerMap.find(serverID);	
	
	if (it != ProbeServerMap.end()){	//if serverID is found in the map
	
		ProbeServerMap.erase(it);	//erase it from the map
		
		addMaliciousServerToBlacklist(serverID);	//add serverID to the blacklist
		
		return 1;		
	}
	
	else{
		cout<<serverID<<" not found in the ProbeServerMap; nothing to be blacklisted"<<endl;
		return -1;
	}
}


vector<string> TaskQueue::blacklistMaliciousVotingServers(int maliciousThreshold){	//recommended to be called on the werification queue after voting so malicious servers cannot be used for subsequent probing or working
								//it takes integer as argument and returns the number of blacklisted serverIDs
	set<string> allVerificationServerSet;
	vector<string> maliciousServers; // store the malicious servers found by this function
	
	int numberOfMaliciousFlags = 0, numberOfBlacklikstedServers = 0;		
	
	//make a set of all verification server IDs
		
	for (auto it = VerificationQueue.begin(); it != VerificationQueue.end(); it++){	//loop to traverse the VerificationQueue map
	
		for (int i = 0; i < it->second.size(); i++){		//loop to traverse the vector of the taskID key in the VerificationQueue
		
			allVerificationServerSet.insert(it->second[i].server.serverID);	//insert each serverID into the set. Set stores unique copies and sort them in ascending order by default
			}
	}
	
	//for every serverID in the set, check its serverFlags for all TaskIDs to calculate its numberOfMaliciousFlags
	
	for (auto itr = allVerificationServerSet.begin(); itr != allVerificationServerSet.end(); itr++){	//traverse the set of unique serverIDs
	
		for (auto it = VerificationQueue.begin(); it != VerificationQueue.end(); it++){	//loop to traverse the map
	
			for (auto vec_itr = it->second.begin(); vec_itr != it->second.end(); vec_itr++){		//loop to traverse the vector of the taskID key in the map
		
				if (*itr == vec_itr->server.serverID){	//check if current serverID is present in the vector of the current taskID
					if (vec_itr->server.serverFlag == -1){
						
						numberOfMaliciousFlags++;	//count the number of times the server has been malicious
					}
					break; //break out of the vector loop once the serverID has been found; there is no point completing the loop since only once instance of the server is possible per TaskID		
				}
			}
		}
		
		if (numberOfMaliciousFlags >= maliciousThreshold){	//chcek if the malicious threshold is met after going through the verification queue for each serverID
		
			numberOfBlacklikstedServers++;	//increment the number of blacklisted server
			
			auto iterator = ProbeServerMap.find(*itr);	//find and remove the serverID from the ProbeServer map and add it to the blacklist
	
			if (iterator != ProbeServerMap.end()){	//if serverID is found in the ProbeServer map
			
				ProbeServerMap.erase(iterator);	//erase serverID from the ProbeServer map
				
				addMaliciousServerToBlacklist(*itr);	//add serverID to the blacklist	
				maliciousServers.push_back(*itr);
			}
			// cout<<*itr<<" was malicious "<<numberOfMaliciousFlags<<" times and was blacklisted."<<endl;
		}
		
		numberOfMaliciousFlags = 0;	//reset this variable before going for the next serverID in the set		
	}
	// return numberOfBlacklikstedServers;
	return maliciousServers;
	
}	//end of blacklistMaliciousVotingServers()


int TaskQueue::cleanUpVerificationQueue(float thresholdPercentage){	//should be called periodically to clean up the entire data structure
	
	set <string> allVerificationServerIDSet;	//set to store unique VerificationQueue serverIDs
	vector<string> serverIDsPerTaskID;	//keep a vector of serverIDs for each TaskID; clear this vector after each TaskID iteration
	vector<string> deleteVector;			//to store the TaskIDs to be deleted because deleting a TaskID in the middle of map iteration causes segmentation fault
	int verificationServerIDSetCount;		//store the total number of servers in the data structure map, i.e size of the serverIDSet
	int nServer=0; int serverCount;		//nServer counts the number of server that have seen a TaskID
	float deleteThreshold;				//to compute the threshold for deleting the TaskID
	
	if ((thresholdPercentage < 1) or (thresholdPercentage > 100)){	//check for valid input range between 1 and 100
	
		cout<<"Error: Threshold out of range (1 to 100)."<<endl<<endl;
	
		return -1;
	}
	
	//make a set of all the servers in the system, that is in the ProbeServerMap where we keep all of our servers
	
	for (auto it = ProbeServerMap.begin(); it != ProbeServerMap.end(); it++){	//loop to traverse the map
		
			allVerificationServerIDSet.insert(it->first);	//insert each serverID into the set. Set stores unique copies and sort them in ascending order by default
	}
		
	verificationServerIDSetCount = allVerificationServerIDSet.size();	//find how many elements are in the set
	
	for (auto it = VerificationQueue.begin(); it != VerificationQueue.end(); it++){	//loop to traverse the map
	
		for (int i = 0; i < it->second.size(); i++){		//loop to traverse the vector of each taskID key in the map
					
				serverIDsPerTaskID.push_back(it->second[i].server.serverID);	//push each serverID into the vector for every TaskID found in taskMap.		
			}
			
			for (auto set_itr = allVerificationServerIDSet.begin(); set_itr != allVerificationServerIDSet.end(); set_itr++){	//iterate through the allVerificationServerIDSet
			
				serverCount = count(serverIDsPerTaskID.begin(), serverIDsPerTaskID.end(), *set_itr);	//counts the number of occurences of each set element that occur the vector. It is 																expected to be 1 if it occurs, or 0 if not because the serverID is only stored 																once for each TaskID in the taskMap
				
				if (serverCount > 0){
				
					nServer++;	//increment nServer if the element of serverIDset is found in the vector of serverIDsPerTask					
				}		
			}
			
			deleteThreshold =  (float(nServer) / float(verificationServerIDSetCount)) * 100;	//compute the percentage of servers that have seen a TaskID
			// cout<<"Task "<<it->first<<" threshold is "<<deleteThreshold<<"%"<<endl<<endl;
			if (deleteThreshold >= thresholdPercentage){			//check if threshold is equal or greater than the argument threshold
			
				deleteVector.push_back(it->first);	//store the TaskID for deletion if it meets the threshold	
				
				// cout<<"Delete threshold was met::Task "<<it->first<<" has been flagged for deletion"<<endl<<endl;
			}

			nServer = 0;	//reset nServer for the next TaskID
			serverIDsPerTaskID.clear();	//clear the vector before going through the next TaskID
	}
	
	//This block calls detele function on deleteFromVerificationQueueByTaskID() to delete the flagged TaskIDs from the Verification Queue
	
	//int deleteVectorSize = deleteVector.size();	//keep a copy of the size of the vector before performing delete operations on it
	
	if (deleteVector.size() == 0){
		
		return 0;
	}
	
	else{	
		for (int i = 0; i < deleteVector.size(); i++){
				
			deleteFromVerificationQueueByTaskID(deleteVector[i]); //call deleteTaskByID() on each TaskID in the deleteVector		
		}
	}
	
	return deleteVector.size();
}	//end of cleanUpVerificationQueue()


int TaskQueue::cleanUpVerificationQueueByTaskID(string TaskID, float thresholdPercentage){	//function deletes an individual task based on the given thresholdPercentage

	set <string> allVerificationServerIDSet;	//set to store unique VerificationQueue serverIDs
	vector<string> serverIDsPerTaskID;	//keep a vector of serverIDs for each TaskID; clear this vector after each TaskID iteration
	int verificationServerIDSetCount;		//store the total number of servers in the data structure map, i.e size of the serverIDSet
	int nServer=0; int serverCount;		//nServer counts the number of server that have seen a TaskID
	float deleteThreshold;				//to compute the threshold for deleting the TaskID
	
	auto itr = VerificationQueue.find(TaskID);
	
	if (itr != VerificationQueue.end()){	//if TaskID is found in VerificationQueue
	
		for (int i = 0; i < itr->second.size(); i++){		//loop to traverse the vector of each taskID key in the map
					
				serverIDsPerTaskID.push_back(itr->second[i].server.serverID);	//push each serverID into the vector for every TaskID found in taskMap.		
		}
		
		for (auto it = ProbeServerMap.begin(); it != ProbeServerMap.end(); it++){	//loop to traverse the map
		
			allVerificationServerIDSet.insert(it->first);	//insert each serverID into the set. Set stores unique copies and sort them in ascending order by default
		}
		
		verificationServerIDSetCount = allVerificationServerIDSet.size();	//find how many elements are in the set	
		
		for (auto set_itr = allVerificationServerIDSet.begin(); set_itr != allVerificationServerIDSet.end(); set_itr++){	//iterate through the allVerificationServerIDSet
			
			serverCount = count(serverIDsPerTaskID.begin(), serverIDsPerTaskID.end(), *set_itr);	//counts the number of occurences of each set element that occur the vector. It is 																expected to be 1 if it occurs, or 0 if not because the serverID is only stored 																once for each TaskID in the taskMap
			
			if (serverCount > 0){
			
				nServer++;	//increment nServer if the element of serverIDset is found in the vector of serverIDsPerTask					
			}		
		}
			
		deleteThreshold =  (float(nServer) / float(verificationServerIDSetCount)) * 100;	//compute the percentage of servers that have seen a TaskID
		cout<<"Task "<<itr->first<<" threshold is "<<deleteThreshold<<"%"<<endl<<endl;
		if (deleteThreshold >= thresholdPercentage){			//check if threshold is equal or greater than the argument threshold
			
			cout<<"Delete threshold was met::Task "<<itr->first<<" has been flagged for deletion"<<endl<<endl;
		}	
		
		deleteFromVerificationQueueByTaskID(itr->first); //delete Task from verification queue
		return 1;	
	}
	
	else{	//TaskID not found
		cout<<itr->first<<" not found in Verification Queue."<<endl;
		return -1;
	}
}


vector<string> TaskQueue::getTaskServerIDsByID(string taskID){	//returns the vector of the serverIDs that have seen this TaskID before or NULL if taskID is not found
	
	vector<string> taskServerIDs; 			//declare a vector of string to store the found serverIDs for the given TaskiD
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = ProbeQueue.find(taskID);			//check whether the taskID is in the map		
	
	if (it != ProbeQueue.end()){ 			//true if the taskID is found in the map
	
		for (Task serverIDFinder : it->second){	//traverse the vector to search for each object's serverID	
		
			taskServerIDs.push_back(serverIDFinder.server.serverID);	//store each serverID of the task object in a vector		
		}	
		
		return taskServerIDs;	
	}
	
	//if taskID is not found, return a vector of one NULL taskServerID
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector"<<endl<<endl;
	
	taskServerIDs.push_back("NULL");	//this will be checked by comparing the first vector object to "NULL" e.g if (vector<string> serverIDVector[0] == "NULL")
		
	return taskServerIDs;	//return null containing vector if object not found	
}


vector<Servers> TaskQueue::getTaskServersByID(string taskID){	//returns the vector of the server objects that have seen this TaskID before or NULL if taskID is not found
	
	vector<Servers> taskServers; 			//declare a vector of Servers to store the found server objects for the given TaskID
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = ProbeQueue.find(taskID);			//check whether the taskID is in the map		
	
	if (it != ProbeQueue.end()){ 			//true if the taskID is found in the map
	
		for (Task serverFinder : it->second){	//traverse the vector to search for each object's server
		
			taskServers.push_back(serverFinder.server);	//store server of each task object in taskServers vector		
		}	
		
		return taskServers;	
	}
	
	//if taskID is not found, return a vector of one NULL server
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector"<<endl<<endl;
	
	Servers serv; serv.serverID = "NULL"; serv.serverFlag = -1;	//construct a Server object of serverID NULL and serverFlag -1 in order to be able to return a Server object
	
	taskServers.push_back(serv);	//push the invalid Server object to the vecotr if the passed TaskID is not found in the map
		
	return taskServers;	//this should be checked when called by comparing the first vector object to "NULL" e.g if (vector<Servers> serverVector[0].serverID == "NULL")	
}


vector<Task> TaskQueue::getTaskObjectsByID(string taskID){	//returns the vector of all the tasks that are associated with this taskID
	
	vector<Task> taskObjects; 			//declare a vector of Task to store the found task objects for the given TaskiD
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = ProbeQueue.find(taskID);			//check whether the taskID is in the map		
	
	if (it != ProbeQueue.end()){ 			//true if the taskID is found in the map
	
		for (Task taskFinder : it->second){	//traverse the vector to search for each object	
		
			taskObjects.push_back(taskFinder);	//store each object in the taskObject vector		
		}	
		
		return taskObjects;	
	}
	
	//if taskID is not found, return a vector of one NULL taskObject
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector as taskID. in Probe Queue"<<endl<<endl;
	
	Task invalidTask("NULL", "NULL", "NULL", "NULL", "NULL", -1);
	
	taskObjects.push_back(invalidTask);	//this may be checked when called by comparing the first vector object taskID to "NULL" e.g if (vector<Task> taskObject[0].taskID == "NULL")
		
	return taskObjects;	//return null pointer if object not found	
}


vector<Task> TaskQueue::getTaskObjectsByIDonVerificationQueue(string taskID){	//returns the vector of all the tasks that are associated with this taskID
	
	vector<Task> taskObjects; 			//declare a vector of Task to store the found task objects for the given TaskiD
	
	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = VerificationQueue.find(taskID);			//check whether the taskID is in the map		
	
	if (it != VerificationQueue.end()){ 			//true if the taskID is found in the map
	
		for (Task taskFinder : it->second){	//traverse the vector to search for each object	
		
			taskObjects.push_back(taskFinder);	//store each object in the taskObject vector		
		}	
		
		return taskObjects;	
	}
	
	//if taskID is not found, return a vector of one NULL taskObject
	
	cout<<"Task "<<taskID<<" not found. NULL will be returned to the vector as taskID in verification Queue"<<endl<<endl;
	
	Task invalidTask("NULL", "NULL", "NULL", "NULL", "NULL", -1);
	
	taskObjects.push_back(invalidTask);	//this may be checked when called by comparing the first vector object taskID to "NULL" e.g if (vector<Task> taskObject[0].taskID == "NULL")
		
	return taskObjects;	//return null pointer if object not found	
}


int TaskQueue::updateServerResult(string taskID, string newServerResult){	//updates all the server results for the given taskID, returns 1 on success, and -1 on failure

	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map
	
	it = ProbeQueue.find(taskID);			//check whether the taskID is in the map		
	
	if (it != ProbeQueue.end()){ 			//true if the taskID is found in the map
	
		for (int i = 0; i < it->second.size(); i++){	//traverse the vector to search for each object's ServerResult	
		
			it->second[i].server.serverResult = newServerResult;	//store new ServerResult of the taskID in the object queue		
		}	
		
		cout<<"Inside updateServerResult(), task result has been updated. "<<endl<<endl;
		
		return 1;	
	}
	
	//if taskID is not found, return -1
	
	cout<<"Task "<<taskID<<" not found. -1 is returned."<<endl<<endl;
	
	return -1;	
}

int TaskQueue::deleteFromProbeQueueByTaskID(string taskID){	//deletes a Task identified by its taskID from the ProbeQueue; called inside voteOnProbeQueue()

	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map

	it = ProbeQueue.find(taskID);		//find the taskID in the map
	
	if (it != ProbeQueue.end()){ 			//if taskID is found in the map
	
		ProbeQueue.erase(it);			//erase taskID referenced by the iterator	
		
		// cout<<"Task "<<taskID<<" has been deleted from the ProbeQueue"<<endl<<endl;
		
		return 1;	
	}
	
	cout<<"Task "<<taskID<<" not found in ProbeQueue. Nothing to delete."<<endl<<endl;
	
	return -1;
}

int TaskQueue::deleteFromVerificationQueueByTaskID(string taskID){	//deletes a Task identified by its TaskID from the VerificationQueue; can be called inside cleanUp()

	map <string, vector<Task>>::iterator it;	//declare an iterator for traversing the map

	it = VerificationQueue.find(taskID);		//find the taskID in the map
	
	if (it != VerificationQueue.end()){ 			//if taskID is found in the map
	
		VerificationQueue.erase(it);			//erase taskID referenced by the iterator	
		
		// cout<<"Task "<<taskID<<" has been deleted from the VerificationQueue"<<endl<<endl;
		
		return 1;	
	}
	
	cout<<"Task "<<taskID<<" not found in VerificationQueue. Nothing to delete."<<endl<<endl;
	
	return -1;
}

string TaskQueue::displayProbeQueue(){	//prints out the content of the ProbeQueue
	
	ostringstream outputStream;	//to store concatenated strings
	// string outputString;			//to print concatenated string
	
	// outputStream.str("");
	// outputStream.clear();
	
	outputStream<<"Probe Queue: "<<endl;
	outputStream<<setw(10)<<left<<"TaskID"<<setw(20)<<left<<"Task_Timestamp"<<setw(20)<<left<<"Task_VotedResult"<<setw(10)<<left<<"serverID[serverResult, serverFlag]"<<endl;
	// outputString = outputStream.str();
	// cout<<outputString;
	
	for (auto it = ProbeQueue.begin(); it != ProbeQueue.end(); it++){	//use of auto keyword
		
		// outputStream.str("");	//needed to avoid outputStreams printing multiple times						 
	 	// outputString.clear();	//needed to avoid outputStreams printing multiple times	
		
		outputStream<<setw(10)<<left<<it->second[0].TaskID<<setw(20)<<left<<it->second[0].TaskTimestamp<<setw(20)<<left<<it->second[0].TaskVotedResult<<it->second[0].server.serverID<<"["<<it->second[0].server.serverResult<<","<<it->second[0].server.serverFlag<<"]";
		
		if (it->second.size() > 1){	//check to see if there is more objects in the vector to print after printing the first object above
		
			for (int i = 1; i < it->second.size(); i++){	//print other servers that have seen the current task
		
				outputStream<<"\t"<<it->second[i].server.serverID<<"["<<it->second[i].server.serverResult<<","<<it->second[i].server.serverFlag<<"]";
			}
			
			// outputString = outputStream.str();
			// cout<<outputString;
		}
		
		// else{
		// 	outputString = outputStream.str();
		// 	cout<<outputString;
		// }			
		
		// cout<<endl;
		outputStream<<endl;
	}
	
	// cout<<endl;
	return outputStream.str();
}

string TaskQueue::displayVerificationQueue(){	//prints out the content of the VerificationQueue
	
	ostringstream outputStream;	//to store concatenated strings
	// string outputString;			//to print concatenated string
	
	// outputStream.str("");
	// outputStream.clear();
	
	outputStream<<"Verification Queue: "<<endl;		
	outputStream<<setw(10)<<left<<"TaskID"<<setw(20)<<left<<"Task_Timestamp"<<setw(20)<<left<<"Task_VotedResult"<<setw(10)<<left<<"serverID[serverResult, serverFlag]"<<endl;
	// outputString = outputStream.str();
	// cout<<outputString;
	
	for (auto it = VerificationQueue.begin(); it != VerificationQueue.end(); it++){	//use of auto keyword
		
		// outputStream.str("");	//needed to avoid outputStreams printing multiple times						 
	 	// outputString.clear();	//needed to avoid outputStreams printing multiple times	
		
		outputStream<<setw(10)<<left<<it->second[0].TaskID<<setw(20)<<left<<it->second[0].TaskTimestamp<<setw(20)<<left<<it->second[0].TaskVotedResult<<it->second[0].server.serverID<<"["<<it->second[0].server.serverResult<<","<<it->second[0].server.serverFlag<<"]";
		
		if (it->second.size() > 1){	//check to see if there is more objects in the vector to print after printing the first object above
		
			for (int i = 1; i < it->second.size(); i++){	//print other servers that have seen the current task
		
				outputStream<<"\t"<<it->second[i].server.serverID<<"["<<it->second[i].server.serverResult<<","<<it->second[i].server.serverFlag<<"]";
			}
			
			// outputString = outputStream.str();
			// cout<<outputString;
			outputStream << endl;
		}
		
		// else{
		// 	outputString = outputStream.str();
		// 	cout<<outputString;
		// }
		
		// cout<<endl;
	}	
	
	// cout<<endl;
	return outputStream.str();
}


vector<string> TaskQueue::getBlacklistedServers(){  // return the vector of names of blacklisted server
	return blacklistedServers;
}

string TaskQueue::displayBlacklist(){	//function to display the blacklisted servers

	ostringstream outputStream;	//to store concatenated strings

	outputStream<<"Displaying Blacklist: "<<endl;
	
	if (blacklistedServers.size() == 1)	{
	
		outputStream<<"["<<blacklistedServers[0]<<"]"<<endl;
	}
	
	else if (blacklistedServers.size() >= 1){
		
		outputStream<<"[";
	
		for (auto it = blacklistedServers.begin(); it != blacklistedServers.end(); it++){	//use of auto keyword
			
		 	outputStream<<*it<<",\t";		 	
		 }
		outputStream<<"]"<<endl;	
	}
	
	else
		outputStream<<"Blacklist is empty."<<endl;

	return outputStream.str();
}


int TaskQueue::voteOnProbeQueue(int numberOfVoters, int winThreshold){	//numberOfVoters = number of probe servers; winThreshold = minimum fraction of voters that should win the election eg 5 voters but need 4 correct results to win

	int serverResultFrequency = 0;	//to track the number of times a serverResult appears per TaskID

	set<string> serverResultSet;		//set to keep the server Results per TaskID
	
	vector<string> serverResultVector;	//to accumulate all the serverResults per TaskID
	
	vector <pair<string, int>> serverResult_Frequency_Vector; //to store the serverResult - Frequency pairs ie no of times a serverResult occurs per TaskID
	
	vector<string> votedTaskToRemove;	//to collect the tasks that will be removed from the ProbeQueue after successfully voting on it
	
	bool invalidServerResultFlag;
	
	int computedThreshold; int voteCount = 0;

	for (auto it = ProbeQueue.begin(); it != ProbeQueue.end(); it++){	//loop to traverse the map
	
		invalidServerResultFlag = false; //reset for the next TaskID iteration
	
		if (it->second.size() < numberOfVoters){	//test if default number of voting servers of 3 is met; voting servers have to be 3 or more for election to hold
		
			cout<<"Skipping Task "<<it->first<<"; Voting inconclusive because electorate servers were less than " << numberOfVoters <<endl;
			continue;
		}
		
		// scan for the vector of tasks for at least 1 invalid server result; if any server is yet to return its result, we will not vote on such task yet
		for (auto vec_itr = it->second.begin(); vec_itr != it->second.end(); vec_itr++){	//loop to traverse the vector of each TaskID in the map
		
			if (vec_itr->server.serverResult == "wait"){	//if a server still has the default invalid server result
			
				invalidServerResultFlag = true;
				// cout<<"Skipping Task "<<it->first<<"; Voting inconclusive because "<<vec_itr->server.serverID<<" or more nodes have no server result"<<endl;
				break; //break out of the scanning if at least one server is yet to return its result
			}				
		}
		
		if (invalidServerResultFlag == true)	//if there is an invalid server result, continue to the next TaskID iteration of the ProbeQueue
			continue;
		
		// collect the set and vector of server Results if all server results are present
		else{		
			for (auto vec_itr = it->second.begin(); vec_itr != it->second.end(); vec_itr++){	//loop to traverse the vector of each TaskID in the map
					
				serverResultSet.insert(vec_itr->server.serverResult);	//add each server Result to the set; a set does not store duplicate copies
				serverResultVector.push_back(vec_itr->server.serverResult); //add each server Result to the vector; a vector stores multiple copies
			}
			
			
			// Now count the frequency of each server Result in the set against the vector of server Results
			for (auto taskSet_itr = serverResultSet.begin(); taskSet_itr != serverResultSet.end(); taskSet_itr++){	//traverse the set of unique server Results
		
				serverResultFrequency = count(serverResultVector.begin(), serverResultVector.end(), *taskSet_itr);	//count how many times each serverResult in the set occur in the overall 																	vector of serverResults			
				serverResult_Frequency_Vector.push_back(make_pair(*taskSet_itr, serverResultFrequency));   	//store the serverResult - Frequency pair in a separate serverResult_Frequency_Vector			
			}			
			
			sort(serverResult_Frequency_Vector.begin(), serverResult_Frequency_Vector.end(), sortElectionResult_DescendingOrder); //sort election result - frequency pairs by frequency in descending order 										           
			computedThreshold = serverResult_Frequency_Vector[0].second; //the first frequency after sorting will be the winning frequency
			
			if (computedThreshold >= winThreshold){		//if there is a winning result; ie result threshold is met or exceeded
			
				voteCount++;	//to count how many successful elections take place
			
				// cout<<"Election of Task "<<it->first<<" is successful; Voted Task Result is "<<serverResult_Frequency_Vector[0].first<<endl;
				
				votedTaskToRemove.push_back(it->first);	//items of this vecotr will be removed from the ProbeQueue after voting is concluded
				
				//traverse the vector to construct electorateServers objects to populate the VerificationQueue
				for (auto vec_itr = it->second.begin(); vec_itr != it->second.end(); vec_itr++){
				
					if (serverResult_Frequency_Vector[0].first == vec_itr->server.serverResult){	//if the winning result equals the server's Result
					
						Task votedTask(vec_itr->TaskID, vec_itr->TaskTimestamp, serverResult_Frequency_Vector[0].first, vec_itr->server.serverID, vec_itr->server.serverResult, 1);//construct a winning voted Task  object with the ProbeQueue parameters while appropriately changing the TaskVotedResult and serverFlag as 1
											
						VerificationQueue[it->first].push_back(votedTask);		//populate the VerificationQueue	
					}
					
					else{	//if the server did not have a winning Task Result
					
						Task votedTask(vec_itr->TaskID, vec_itr->TaskTimestamp, serverResult_Frequency_Vector[0].first, vec_itr->server.serverID, vec_itr->server.serverResult, -1);//construct a winning voted Task object with the ProbeQueue parameters while appropriately changing the TaskVotedResult and serverFlag as -1
						
						VerificationQueue[it->first].push_back(votedTask);		//populate the VerificationQueue
					}
				}			
			}
			
			serverResultSet.clear();	//clears the set for the next iteration of ProbeQueue
			serverResultVector.clear();	//clears the vector for the next iteration of ProbeQueue
			serverResult_Frequency_Vector.clear();	//clears the vector for the next iteration of ProbeQueue
			serverResultFrequency = 0; 
			computedThreshold = 0;	
		}				
	}
	
	//this is where we reduce the ProbeQueue by the number of voted tasks
	if ((votedTaskToRemove.size()) == 0){	//if no Task was voted on
		
		return voteCount;	//voteCount will be 0 in this case
	}
	
	else{	
		for (int i = 0; i < votedTaskToRemove.size(); i++){
				
			deleteFromProbeQueueByTaskID(votedTaskToRemove[i]); //call deleteFromProbeQueueByTaskID() on each TaskID in the votedTaskToRemove vector		
		}
	}	
	
	return voteCount;	//return 0 if no election is successful and value of voteCount if any elections are successful

}	//end of voteOnProbeQueue()


TaskToVerify TaskQueue::selectTaskToVerify(string serverID){	//takes a serverID and returns a TaskToVerify object

	bool flag;
	int itr;
	
	TaskToVerify taskObject("", "");
	
	for (auto it = VerificationQueue.begin(); it != VerificationQueue.end(); it++){	//loop for iterating the varification queue
	
		flag = false;	//initialize the flag for checking if the serverID is found for a particular TaskID iteration
	
		for (itr = 0; itr < it->second.size(); itr++){	//traverse the vector values of the TaskID to check for the serverID

			if (it->second[itr].server.serverID == serverID){	//if serverID is found, change the flag to true
				
				flag = true;
				break;		//stop traversing once the serverID if found in the vector
			}
		}
		
		if (flag == true)	//if the serverID was found, continue to the next TaskID iteration
			continue;
		
		else{	//initialize the TaskToVerify_Object attributes if the serverID has not seen the task before
			taskObject.TaskID = it->first;
			taskObject.TaskVotedResult = it->second[0].TaskVotedResult;
			return taskObject; //check the TaskID if it is = "" to validate the returned object
		}	
	}
	
	return taskObject;
}	//end of selectTaskToVerify()


//this function updates the VerificationQueue and should be called after a verified server returns its result. It returns 1 on successful update and -1 on failure
int TaskQueue::updateVerificationQueue(string TaskID, string serverID, string serverResult, int serverFlag){	//takes - taskID, serverID, serverResult and serverFlag

	auto it = VerificationQueue.find(TaskID);	//find the TaskID in the verification queue
	
	if (it != VerificationQueue.end()){	//true if server's taskID is found
	
		for(int i = 0; i < it->second.size(); i++){	//double check the serverID is not present in the vector before adding it
		
			if(it->second[i].server.serverID == serverID){	//true if server has seen this task before
			
				cout<<"error: in updateVerificationQueue; serverID " << serverID << " already saw this TaskID " << TaskID << " before; this is not a valid update action"<<endl;
				return -1;
			}
		}
		Task verifiedServer(TaskID, "", "", serverID, serverResult, serverFlag);	//construct a task for the verified server being updated
		VerificationQueue[it->first].push_back(verifiedServer);	//add the verified server object to the vector
		return 1;
	}
	
	else{
		cout<<"error: TaskID is new to the Verification Queue; this is not a valid update action"<<endl;
		return -1;
	}
}	//end of updateVerificationQueue()

map<string, int> TaskQueue::getMapOfVerificationTasksForAllServers(){	//this function returns a map that contains all probing servers and their corresponding number of verification tasks

map<string, int> verificationTaskMap;	//map to store serverID - numberOfVerificationTasks pairs

int numberOfVerificationTasks = 0;

for (auto it = ProbeServerMap.begin(); it != ProbeServerMap.end(); it++){	//use the serverIDs in the ProbeServerMap as the nodes for which to find their corresponding numberOfVerificationTasks

	for (auto ver_itr = VerificationQueue.begin(); ver_itr != VerificationQueue.end(); ver_itr++){	//iterate the VerificationQueue to search for applicable verification tasks
	
		for (int i = 0; i < ver_itr->second.size(); i++){	//iterate the TaskID vector to search whether or not the serverID in ProbeServerMap has seen the VerificationQueue TaskID before
			
			if (it->first == ver_itr->second[i].server.serverID)	//if the serverID has seen this task before, break out of the current TaskID iteration for loop
				break;
		}		
		numberOfVerificationTasks++;	//if the current serverID has not seen the current TaskID before, increment numberOfVerificationTasks
	}
	verificationTaskMap.insert(pair<string, int>(it->first, numberOfVerificationTasks));	//insert the serverID - numberOfVerificationTasks pair into the map
	
	numberOfVerificationTasks = 0;	//reset the numberOfVerificationTasks for the next serverID in ProbeServerMap	
}
return verificationTaskMap;
}
