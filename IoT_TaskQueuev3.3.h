#ifndef IOT_TASKQ_H
#define IOT_TASKQ_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip> //needed to use setw()
#include <sstream>
#include <algorithm>	//needed to use the find() fucntion of a vector
#include <functional>
#include<set>
#include "IoT_Taskv1.5.h"

using namespace std;

bool sortByFrequency(pair<string, int> &a, pair<string, int> &b);	//this is sort function called in selectServers() as a comparator for sorting the vector of serverID and frequency pairs

bool sortElectionResult_DescendingOrder(pair<string, int> &a, pair<string, int> &b);	//sort function to sort voted task results (result - frequency pairs) in descending order

class TaskQueue{ //start of TaskQueue class

public:
map <string, vector<Task>> ProbeQueue;	//declare a map of key-value pair where key is the taskID and value is a vector of server objects

map <string, vector<Task>> VerificationQueue;	// data structure to keep the results of the elections by servers on TaskIDs

map <string, int> ProbeServerMap; //to store the serverID - frequncy pairs used by selectProbeServers()

vector<string> blacklistedServers;	//to keep a history of known malicious servers

// Default constructor
TaskQueue ();

//this constructor initializes the ProbeServerMap data structure with as many servers as the integer arguments passed using the naming convention of server(i).

TaskQueue(int numberOfServers);	//server naming convention is server1, server2.....servern

//this constructor initializes the ProbeServerMap data structure with the names provided by the client. The names should be provided as a vector of string
TaskQueue(vector<string> nodeIDs);	//takes a vector of node IDs and initializes them

int checkBlacklist(string serverID);		//sort and checks the blacklist for a serverID....returns 1 if serverID is found and 0 if not found	

int addMaliciousServerToBlacklist(string serverID);	//adds a serverID to the blacklist; returns 1 if serverID was added or 0 otherwise

int removeMaliciousServerFromBlacklist(string serverID);	//removes a serverID from the blacklist; returns 1 if successful and 0 on failure

//you may also call initializeProbeServer() whenever client discovers a new server that is not part of the existing probing servers

int initializeProbeServer(string serverID);	//it returns 1 if initialization is successful, it returns 0 if serverID already exists and -1 if it is found in the blacklist

//this function should be called by client after it has got some servers from selectProbeServers() and has sent out a probe tasks to them, so as to get ProbeServerMap up to date

int updateProbeServerFrequency(vector<string> serverIDs);	//takes a vector of serverIDs; returns 1 if successful and -1 if it fails

//overloaded updateProbeServerFrequency() to accept string

int updateProbeServerFrequency(string serverID);	//it returns 1 if updating is successful, it returns 0 if initializeProbeServer() was called and -1 if it fails		

vector<string> selectProbeServers(string currentServerID, int numberOfSelectedServers);   //returns a vector of serverIDs if successful, or a vector of size of 0 if no server to return 

string displayProbeServer_Frequency();	//to display the probing servers map data structure

int add_Update_Task(Task task);	//returns 1 if successful and -1 if unsuccessful. It prevents duplicate taskIDs and duplicate serverIDs

int blacklistMaliciousWorkingServer(string serverID);	//removes serverID from ProbeServerMap and adds it to the blacklist; should be called when a server fails verification


vector<string> blacklistMaliciousVotingServers(int maliciousThreshold);	//recommended to be called on the verification queue after voting so malicious servers cannot be used for subsequent probing or working

int cleanUpVerificationQueue(float thresholdPercentage);	//should be called periodically to clean up the entire data structure

int cleanUpVerificationQueueByTaskID(string TaskID, float thresholdPercentage);	//function deletes an individual task based on the given thresholdPercentage

vector<string> getTaskServerIDsByID(string taskID);	//returns the vector of the serverIDs that have seen this TaskID before or NULL if taskID is not found

vector<Servers> getTaskServersByID(string taskID);	//returns the vector of the server objects that have seen this TaskID before or NULL if taskID is not found

vector<Task> getTaskObjectsByID(string taskID);	//returns the vector of all the tasks that are associated with this taskID

vector<Task> getTaskObjectsByIDonVerificationQueue(string taskID);	//returns the vector of all the tasks that are associated with this taskID

int updateServerResult(string taskID, string newServerResult);	//updates all the server results for the given taskID, returns 1 on success, and -1 on failure

int deleteFromProbeQueueByTaskID(string taskID);	//deletes a Task identified by its taskID from the ProbeQueue; called inside voteOnProbeQueue()

int deleteFromVerificationQueueByTaskID(string taskID);	//deletes a Task identified by its TaskID from the VerificationQueue; can be called inside cleanUp()

string displayProbeQueue();	//prints out the content of the ProbeQueue

string displayVerificationQueue();	//prints out the content of the VerificationQueue

vector<string> getBlacklistedServers(); // return the vector of names of blacklisted server

string displayBlacklist();	//function to display the blacklisted servers

int voteOnProbeQueue(int numberOfVoters, int winThreshold);	//numberOfVoters = number of probe servers; winThreshold = minimum fraction of voters that should win the election eg 5 voters but need 4 correct results to win

TaskToVerify selectTaskToVerify(string serverID);	//takes a serverID and returns a TaskToVerify object

//this function updates the VerificationQueue and should be called after a verified server returns its result. It returns 1 on successful update and -1 on failure
int updateVerificationQueue(string TaskID, string serverID, string serverResult, int serverFlag);	//takes - taskID, serverID, serverResult and serverFlag
		
};//end of TaskQueue class
#endif
