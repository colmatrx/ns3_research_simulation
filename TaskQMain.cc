#include <iostream>
#include <map>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include "IoT_Taskv1.5.h"

using namespace std;
using namespace std::chrono;

int feature[4];			//declaration of array of task features 
map <int, vector<Task>> collusion; 	//map to store taskID and corresponding task properties including features and servers
string logString;
set<int> serverIDset;		//to keep unique serverIDs
set<int> colludingServerSet;	//a set of coluding serverIDs
set<int> universalServerSet;	//to track all of the servers generated
int numOfColludingServers;	//to store computed number of colluding servers from its percentage (based on 20 servers)
int colludingCount;	//to determine if there is colluding servers for a TaskID to poison the task
int numOfCleanTasks; //to store the number of clean tasks before collusion
int simulationRounds;

clock_t start_time, end_time, start_time_poisonedTask, end_time_poisonedTask, start_time_verifyBenign, end_time_verifyBenign;	//to measure execution times
double cumulativeGreedyTime, cumulativePoisonedTime, cumulativeBenignTime;

int repetition, numberOfTasks, percentageOfColludingServers;

void logFile (string logData){	//logs data to file

	fstream collusionLog;	// filename
	collusionLog.open("collusionLog", ios::app);
	if (!collusionLog){
		cout<<"Log file not created";
	}
	else{
		collusionLog<<logData<<"\n";
	}
	collusionLog.close();
}

int main(int argc, char* argv[]){

	numberOfTasks = stoi(argv[1]);		//convert second command line argument to integer
	numOfColludingServers = stoi(argv[2]);	//convert third command line argument to integer	
	numOfCleanTasks = stoi(argv[3]);	//convert third command line argument to integer
	simulationRounds = stoi(argv[4]); 
	if ((argc < 5) or (argc > 5)){		//check for 3 command line arguments - no of Tasks, maximum number of colluding servers, number of clean tasks before collusion and simulation rounds
		
		cout<<endl<<"Command line arguments must be four; terminating program"<<endl<<endl;
		return -1;	
	}
	//CollusionPercentage = float(numOfColludingServers / 20) * 100; //cout<<"collusion percentage is "<<CollusionPercentage<<endl;
	srand(time(NULL));	
		
	for (int round = 0; round < simulationRounds; round++){
		colludingServerSet.clear(); universalServerSet.clear(); serverIDset.clear(); collusion.clear();
		
		//generate a random set of colluding servers here	
		while (colludingServerSet.size() < numOfColludingServers){	//track if the required number of colluding serverIDs have been generated
						
				int colludingServerId = (rand()%20) + 1;	//generate random number between 1 and 20	
				
				colludingServerSet.insert(colludingServerId);	//insert colludingServerId into set
		}
		
		//Block to generate and populate the Pre-approved Task Table	
		cout<<"############## Populating Pre-approved Task Table... ###################"<<endl;
		
		for (int i = 1; i <= numberOfTasks; i++){	//generate TaskID
			
			Task task(0,feature,0,"",0);		//construct a Task object assume initially non poisoned task and non-colluding server
				
			task.TaskID = i;		//initialize taskID
			
			for (int features = 0; features < 4; features++){	//this block generates 4 task features for the TaskID_i
					
				task.taskFeature[features] = (rand()%101); //generate random number between 0 and 100
			}
				
			int workingServers = (rand()%20) + 1;	//generate random number between 1 and 20 to use as working servers that have seen Task_i	
			
			while (serverIDset.size() < workingServers){	//this block generates random serverIDs between 1 and 20 for TaskID_i
						
				int id = (rand()%20) + 1;	//generate random number between 1 and 20
				
				serverIDset.insert(id);	//insert id into set
				universalServerSet.insert(id);	//add serverIDs to the overall set to find the actual % of collusion later
			}
			
			//set the poisonTaskFlag here before going to flag colluding server
			//iterate the serverIDset and if at least 2 ServerIDs are found in the colludingServerSet, then flag the taks as poisoned
			
			colludingCount = 0;	//reset the count
			for (auto itrID = colludingServerSet.begin(); itrID != colludingServerSet.end(); itrID++){	//traverse the serverIDset of 3 servers
			
				if (serverIDset.find(*itrID) != serverIDset.end()){	//finds if the serverID is a colluding server
				
					colludingCount++;	//counts the number of colluders in the 3 servers that computed the task
				}
			}
			
			if (colludingCount >= 2 and task.TaskID > numOfCleanTasks){	//modify the first n task.TaskID to reflect non-colluded tasks
			
				task.poisonedTaskFlag = 1;	//it is considered a posioned task if 2 colluding servers have results for it
			}
			
			for (auto it = serverIDset.begin(); it != serverIDset.end(); it++){
			
				task.server.serverID = "server"+to_string(*it);	//construct serverID string
				if (colludingServerSet.find(*it) != colludingServerSet.end()){	//finds if the serverID is a colluding server
					
					task.server.colludingServerFlag = 1;	//set server as colluder if found in colludingServerSet
				}
				
				collusion[task.TaskID].push_back(task);		//add the task object to the Task Pre-approved table
				task.server.colludingServerFlag = 0;	//reset the serverFlag to avoid carrying over the precious flag
			}
			
			serverIDset.clear();	//clear the set before next iteration
		}
		
		//block to print out the Task table content
		
		cout<<endl<<"************* Dispalying Colluding Servers *****************"<<endl;

		cout<<endl<<"Colluding Server IDs [";
		
		for (auto it = colludingServerSet.begin(); it != colludingServerSet.end(); it++){
		
			cout<<*it<<", ";	
		}
		
		cout<<"]"<<endl;	
		
		cout<<endl<<"************* Dispalying All Servers *****************"<<endl;

		cout<<endl<<"All Server IDs [";
		
		for (auto it = universalServerSet.begin(); it != universalServerSet.end(); it++){
		
			cout<<*it<<", ";
		}
		
		cout<<"]"<<endl;
		
		cout<<endl<<"************* Dispalying Actual Simulated % of Collusion *****************"<<endl;
		int actualCollusionCount = 0; float actualCollusionPercentage; 
		
		for (auto it = colludingServerSet.begin(); it != colludingServerSet.end(); it++){
		
			if (universalServerSet.find(*it) != universalServerSet.end()){
			
				actualCollusionCount++;
			}
		}
		
		actualCollusionPercentage = (float (actualCollusionCount) / universalServerSet.size()) * 100;	//compute actual collusion percentage based on the generated table data
		cout<<endl<<"Number of colluding server(s) is "<<actualCollusionCount<<endl;
		cout<<endl<<"Number of working server(s) is "<<universalServerSet.size()<<endl;
		cout<<endl<<"Actual simulated % of collusion is "<<fixed<<setprecision(2)<<actualCollusionPercentage<<"%"<<endl;
		
		cout<<endl<<"************* Dispalying Pre-approved Task Table *****************"<<endl;
		for (auto itr = collusion.begin(); itr != collusion.end(); itr++){
		
			cout<<"TaskID_"<<itr->first<<"\t PoisonedTaskFlag "<< itr->second[0].poisonedTaskFlag<<"\t";	//print out the TaskID and poisonedTaskFlag
			
			cout<<"Task Features ["<<itr->second[0].taskFeature[0]<<", "<<itr->second[0].taskFeature[1]<<", "<<itr->second[0].taskFeature[2]<<", "<<itr->second[0].taskFeature[3]<<"]"<<"\t"; //grab the task features from the first task in the vector of task objects
			cout<<"Servers [";
			for (int i = 0; i < itr->second.size(); i++){		//print out the serverIDs for each TaskID
		
				cout<<itr->second[i].server.serverID<<", ";	//print out serverID and servercollusionFlag //
			}
			cout<<"]"<<endl;			
		}
		
		//########################## selectTaskGreedy Algorithm Implementation ###############################
		map <int, set<string>> greedyTask;	//map of selected collusion verification tasks
		set<string> currentServerSet;	//to keep set of servers for each task iteration and to keep cumulative set of servers during task iteration
		set<string> tempServerSet, totalServerSet, universalServerIDString;
		
		for (auto it = universalServerSet.begin(); it != universalServerSet.end(); it++){	//convert all int serverIDs to serverID strings
		
			universalServerIDString.insert("server"+to_string(*it));	
		}	
		
		cout<<endl<<"Start of selectTaskGreedy Algorithm execution"<<endl;
			
		start_time = clock();	//store the time at beginning of execution
			
		for (auto itr = collusion.begin(); itr != collusion.end(); itr++){	//loop to traverse the Pre-approved Task table
				
			for (int it = 0; it < itr->second.size(); it++){		//loop to traverse the Task objects for serverID comparison
			
				currentServerSet.insert(itr->second[it].server.serverID);	//collect a set of each server that have seen Taski	
			}
			
			set_difference(universalServerIDString.begin(), universalServerIDString.end(), currentServerSet.begin(), currentServerSet.end(), inserter(tempServerSet, tempServerSet.begin()));	//inserts the difference between the all the server set and current server set into temporary server set.
			
			if (tempServerSet.size() == 0){
			
				currentServerSet.clear();		//clear the currentServerSet before the next Task iteration	
				continue;	//skip to the next Task if there are no applicable servers for the TaskID
			}
			
			totalServerSet.insert(tempServerSet.begin(), tempServerSet.end());	//collect cumulative set of servers
			
			if (totalServerSet.size() == universalServerIDString.size()){	//check if we have accounted for all servers during iterations
			
				greedyTask.insert(pair<int, set<string>>(itr->first, tempServerSet));	//keep the Task-setOfServers pair in the map
				break;	//break out of the loop once we find tasks to verify all servers
			}		
			greedyTask.insert(pair<int, set<string>>(itr->first, tempServerSet));	//keep the Task-setOfServers pair in the map
			currentServerSet.clear();	//clear the currentServerSet before the next Task iteration
			tempServerSet.clear();		//clear the currentServerSet before the next Task iteration							
		}
				
		end_time = clock();	//store the time at the end of Collusion Algorithm execution
		cout<<endl<<"End of selectTaskGreedy Algorithm execution"<<endl;	//end of algorithm execution
		double execution_time = (double(end_time - start_time) / double(CLOCKS_PER_SEC))*1000000;	//compute the execution time in microseconds
		cumulativeGreedyTime = cumulativeGreedyTime + execution_time;	//sum up the execution time
		cout<<endl<<"Execution time of selectTaskGreedy Algorithm is "<<execution_time<<" microseconds"<<endl;
		
		cout<<endl<<"*************** Greedy Task List ***********************"<<endl;		
		for (auto itr = greedyTask.begin(); itr != greedyTask.end(); itr++){
		
			cout<<endl<<"TaskID_"<<itr->first<<"\t [";
		
			for (auto it:itr->second){
			
				cout<<it<<", ";	
			}
			cout<<"]"<<endl;	
		}	
		
		cout<<endl<<"There are "<<greedyTask.size()<<" greedy task(s) found to verify "<<totalServerSet.size()<<" servers."<<endl;
		
		//######################### Remove Posioned Task Implementation ########################
		int taskCount = 0; set <int> taskIdentifier; int poisonCount = 0;
		cout<<endl<<"Start of removePoisionedTask Algorithm execution"<<endl;
		start_time_poisonedTask = clock();	//store the time at beginning of execution
		for (auto iteration = collusion.begin(); iteration != collusion.end() ; ){	//loop to traverse the Pre-approved Task table
			
			taskCount = taskCount+1;	//consider the first n tasks as non-poisoned
			if (taskCount <= numOfCleanTasks){
				continue;	//assume the first n tasks are clean, so skip them from removal
			}		
			
			if (iteration->second[0].poisonedTaskFlag == 1){	//check if the task is a poisoned task
					
					collusion.erase(iteration++);				
			}
			
			else{
			
				++iteration;
			}
		}
		
		end_time_poisonedTask = clock();	//store the time at the end of Collusion Algorithm execution
		cout<<endl<<"End of removePoisionedTask Algorithm execution"<<endl;	//end of algorithm execution
		double execution_time_poisonedTask = (double(end_time_poisonedTask - start_time_poisonedTask) / double(CLOCKS_PER_SEC))*1000000;	//compute the execution time in microseconds
		cumulativePoisonedTime = cumulativePoisonedTime + execution_time_poisonedTask;
		cout<<endl<<"Execution time of removePoisionedTask Algorithm is "<<execution_time_poisonedTask<<" microseconds"<<endl;
		
		cout<<endl<<"************* Dispalying Pre-approved Task Table After Removing Poisoned Tasks *****************"<<endl;
		for (auto itr = collusion.begin(); itr != collusion.end(); itr++){
		
			cout<<"TaskID_"<<itr->first<<"\t PoisonedTaskFlag "<< itr->second[0].poisonedTaskFlag<<"\t";	//print out the TaskID and poisonedTaskFlag
			
			cout<<"Task Features ["<<itr->second[0].taskFeature[0]<<", "<<itr->second[0].taskFeature[1]<<", "<<itr->second[0].taskFeature[2]<<", "<<itr->second[0].taskFeature[3]<<"]"<<"\t"; //grab the task features from the first task in the vector of task objects
			cout<<"Servers [";
			for (int i = 0; i < itr->second.size(); i++){		//print out the serverIDs for each TaskID
		
				cout<<itr->second[i].server.serverID<<", ";	//print out serverID and servercollusionFlag
			}
			cout<<"]"<<endl;			
		}
		
		//################## Verify Benign Server Implementation ###########################
		
		set <int> benignServerSet;	//to collect the benign servers
		bool flagcheck;	//to track if serverj has seen a TaskID before
		vector <pair<int, string>> similarityScoreVector; //to store the TaskID-similarityScore pairs
		set_difference(universalServerSet.begin(), universalServerSet.end(), colludingServerSet.begin(), colludingServerSet.end(), inserter(benignServerSet, benignServerSet.begin()));	//separate the benign from colluding servers and store in benignServerSet.
		
		//block to generate random current Task Features
		int currentTaskFeatures[4];	//array to keep current task features
		//srand(time(NULL));
		for (int feature = 0; feature < 4; feature++){	//this block generates 4 task features for the current task
				
			currentTaskFeatures[feature] = (rand()%101); //generate random number between 0 and 100
			
		}
		cout<<endl<<"Current Task Features ["<<currentTaskFeatures[0]<<", "<<currentTaskFeatures[1]<<", "<<currentTaskFeatures[2]<<", "<<currentTaskFeatures[3]<<"]"<<endl;
		
		cout<<endl<<"Start of verifyBenignServer Algorithm execution"<<endl;	//end of algorithm execution
		
		//************** Start of verifyBenignServer Algorithm *********************
		
		start_time_verifyBenign = clock();	//record start time of algorithm
		
		for (auto itr = benignServerSet.begin(); itr != benignServerSet.end(); itr++){	//traverse the benign server set
		
			float currentSimilarityScore = -1.0;	//reset currentSimilarityScore
			string currentTaskID;
			string currentServerID = "server"+to_string(*itr);	//construct serverID
		
			for (auto it = collusion.begin(); it != collusion.end(); it++){	//loop to traverse the Pre-approved Task table
			
				flagcheck = false;	//reset flag for the next iteration
			
				for (int itx = 0; itx < it->second.size(); itx++){		//loop to traverse the Task objects for serverID comparison
				
					if (currentServerID == it->second[itx].server.serverID){	//check if serverj has seen the TaskID before
					
						flagcheck = true;
						break;
					}
				}
				
				if (flagcheck == true){		//if serverj has seen the task before, skip and continue to the next TaskID in the Pre-approved Task Table
				
					continue;
				}
				
				else {		//if serverj has not seen the task before, calculate the similarity score
					float similarityScore = (abs(currentTaskFeatures[0]-it->second[0].taskFeature[0]) + abs(currentTaskFeatures[1]-it->second[0].taskFeature[1]) + abs(currentTaskFeatures[2]-it->second[0].taskFeature[2]) + abs(currentTaskFeatures[3]-it->second[0].taskFeature[3])) / 400.0; //we get 400 from F*M where F=4 and M=100
					if (similarityScore > currentSimilarityScore){
					
						currentTaskID = "TaskID_" + to_string(it->first);	//store the corresponding taskID with the highest similarity score
						currentSimilarityScore = similarityScore; //keep the higher similarity score	
					}
				}				
			}
			if (currentSimilarityScore > -1.0){
				similarityScoreVector.push_back(make_pair(*itr, currentTaskID));	//keep the serverID and taskID
			}
		}
		end_time_verifyBenign = clock();	//record end time of algorithm execution
		cout<<endl<<"End of verifyBenignServer Algorithm execution"<<endl;	//end of algorithm execution
		double execution_time_verifyBenign = (double(end_time_verifyBenign - start_time_verifyBenign) / double(CLOCKS_PER_SEC))*1000000;	//convert to microseconds
		cumulativeBenignTime = cumulativeBenignTime + execution_time_verifyBenign;
		cout<<endl<<"Execution time of Similarity Score is "<<execution_time_verifyBenign<<" microseconds"<<endl;
		
		cout<<endl<<"************* Dispalying Tasks with Highest Similarity Score for each Benign Server *****************"<<endl;
		for (auto itr = 0; itr < similarityScoreVector.size(); itr++){
		
			cout<<"server"<<similarityScoreVector[itr].first<< " -> "<< similarityScoreVector[itr].second<<endl;				
		}
	}
	float CollusionPercentage = (numOfColludingServers / 20.0) * 100;
	logString = "No_Of_Tasks["+to_string(numberOfTasks)+"]\t"+"simulationRounds["+to_string(simulationRounds)+"]\t"+"%_Collusion["+to_string(CollusionPercentage)+"]\t"+"avgSelectTaskGreedy["+to_string(cumulativeGreedyTime/simulationRounds)+"]\t"+"avgRemovePoisonedTask["+to_string(cumulativePoisonedTime/simulationRounds)+"]\t"+"avgVerifyBenignServer["+to_string(cumulativeBenignTime/simulationRounds)+"]";
		logFile(logString);
	
	return 0;
}
