#include <iostream>
#include <map>
#include<ctime>
#include<cstdlib>
#include "IoT_Taskv1.5.h"

//#include "IoT_TaskQueuev3.4.h"

using namespace std;
using namespace std::chrono;

bool sortSimilarityScore_DescendingOrder(pair<string, float> &a, pair<string, float> &b){	//to sort similarityScore vector in descending order

	return (a.second > b.second);	
}

int feature[4];			//declaration of array of task features 

Task task(0,feature,"");		//construct a Task object

map <int, vector<Task>> algorithm1; 	//map to store taskID and corresponding task properties including features and servers

vector<string> serverIDs;	//to store serverIDs for printout

set<int> serverIDset;		//to keep unique serverIDs

clock_t start_time, end_time;		//to measure execution times

double averageExecutionTime, sumOfExecutionTime=0.0; // averageDurationTime, sumOfDurationTime=0.0;

int repetition, numberOfTasks;

int main(int argc, char* argv[]){

	if ((argc < 3) or (argc > 3)){		//check for 2 command line arguments
		
		cout<<endl<<"Command line arguments must be two; terminating program"<<endl<<endl;
		return -1;	
	}
	
	numberOfTasks = stoi(argv[1]);	//convert second command line argument to integer
	repetition = stoi(argv[2]);	//convert third command line argument to integer
	
	//Block to generate and populate the Pre-approved Task Table
	
	cout<<"############## Populating Pre-approved Task Table... ###################"<<endl;
	
	for (int i = 1; i <= numberOfTasks; i++){	//generate TaskID
			
		task.TaskID = i;		//initialize taskID
		
		for (int features = 0; features < 4; features++){	//this block generates 4 task features for the TaskIDi
		
			srand(time(NULL));	//seeding the randome number generator with elapsed time in seconds
		
			task.taskFeature[features] = (rand()%101); //generate random number between 0 and 100
			
			//cout<<task.taskFeature[features]<<endl;
			
			sleep(1);	//sleep a bit to allow for more random numbers to be generated
		}
		
		while (serverIDset.size() < 3){	//this block generates 3 serverIDs for TaskIDi
		
			srand(time(NULL));	//seeding the randome number generator with elapsed time in seconds
			
			int id = (rand()%20) + 1;	//generate random number between 1 and 20
			
			//cout<<"inside set"<<endl;	
			
			serverIDset.insert(id);	//insert id into set
			
			sleep(1);	//sleep 1 second to affect srand()
		}
		
		//cout<<"outside set"<<endl;
		for (auto it = serverIDset.begin(); it != serverIDset.end(); it++){
		
			task.server.serverID = "server"+to_string(*it);	//construct serverID string
			//cout<<task.server.serverID<<endl;
			
			algorithm1[task.TaskID].push_back(task);		//add the task object to the Task Pre-approved table
		}
		
		serverIDset.clear();	//clear the set before next iteration
	}
	
	//block to print out the Pre-approved Task table content
	cout<<endl<<"************* Dispalying Pre-approved Task Table *****************"<<endl;
	for (auto itr = algorithm1.begin(); itr != algorithm1.end(); itr++){
	
		cout<<"TaskID_"<<itr->first<<"\t";	//print out the TaskID
		
		cout<<"Task Features ["<<itr->second[0].taskFeature[0]<<", "<<itr->second[0].taskFeature[1]<<", "<<itr->second[0].taskFeature[2]<<", "<<itr->second[0].taskFeature[3]<<"]"<<"\t"; //grab 											the task features from the first task in the vector of task objects
		
		//collect the serverIDs in a vector
		
		serverIDs.clear();	//clear the vector for a fresh round
		for (int i = 0; i < itr->second.size(); i++){
	
			serverIDs.push_back(itr->second[i].server.serverID);			
		
		}	
		
		//print out the list of servers
		cout<<"Servers [";
		
		for (auto it = serverIDs.begin(); it != serverIDs.end(); it++){
		
			cout<<*it<<", ";	
		}
		cout<<"]"<<endl;		
	}
		
	for (int reps = 0; reps < repetition; reps++){		//for loop to control algorithm repetitions
	
		cout<<endl<<"######## Iteration "<<reps+1<<" #######################"<<endl;
		sleep(1);	
		vector <pair<string, float>> similarityScoreVector; //to store the TaskID-similarityScore pairs
		srand(time(NULL));				//seeding the random number generator with elapsed time in seconds			
		int id = (rand()%20) + 1;			//generate random number between 1 and 20	
		string currentServerID = "server"+to_string(id);	//construct a current serverID for serverj
		bool flagcheck;	//to track if serverj has seen a TaskID before
		cout<<endl<<"Current serverID is "<<currentServerID<<endl;	
		
		//block to generate random current Task Features
		int currentTaskFeatures[4];	//array to keep current task features
		
		for (int feature = 0; feature < 4; feature++){	//this block generates 4 task features for the current task
		
			srand(time(NULL));	//seeding the random number generator with elapsed time in seconds
		
			currentTaskFeatures[feature] = (rand()%101); //generate random number between 0 and 100
			
			//cout<<task.taskFeature[features]<<endl;
			
			sleep(1);	//sleep a bit to allow for more random numbers to be generated
		}
		
		cout<<endl<<"Current Task Features ["<<currentTaskFeatures[0]<<", "<<currentTaskFeatures[1]<<", "<<currentTaskFeatures[2]<<", "<<currentTaskFeatures[3]<<"]"<<endl;
		
		//Algorithm1 Implementation	
		cout<<endl<<"Start of Algorithm1 execution"<<endl;
		
		start_time = clock();	//store the time at beginning of execution
		//auto start_of_execution = high_resolution_clock::now();	//using a different method to measure execution time
		
		for (auto itr = algorithm1.begin(); itr != algorithm1.end(); itr++){	//loop to traverse the Pre-approved Task table
			flagcheck = false;	//reset flag for the next iteration
			//cout<<"itr is "<<itr->first<<endl;
		
			for (int it = 0; it < itr->second.size(); it++){		//loop to traverse the Task objects for serverID comparison
			
				if (currentServerID == itr->second[it].server.serverID){	//check if serverj has seen the TaskID before
				
					flagcheck = true;
					break;
				}
			}
			
			if (flagcheck == true){		//if serverj has seen the task before, skip and continue to the next TaskID in the Pre-approved Task Table
			
				continue;
			}
			
			else {		//if serverj has not seen the task before, calculate the similarity score
				float similarityScore = (abs(currentTaskFeatures[0]-itr->second[0].taskFeature[0]) + abs(currentTaskFeatures[1]-itr->second[0].taskFeature[1]) + abs(currentTaskFeatures[2]-itr->second[0].taskFeature[2]) + abs(currentTaskFeatures[3]-itr->second[0].taskFeature[3])) / 400.0; //we get 400 from F=4 and M=100
				similarityScoreVector.push_back(make_pair(to_string(itr->first), similarityScore));	//add the pair of Task ID and similarity score to the vector
				//cout<<endl<<"Similarity Score for TaskID_"<<itr->first<<" is "<<similarityScore<<endl;	//print out the score for testing purpose
			}				
		}
		
		if (similarityScoreVector.size() == 0){	//check if there are no qualifying TaskIDs for this execution round
			end_time = clock();	//store the time at the end of algorithm1 execution
			cout<<endl<<"End of Algorithm1 execution"<<endl;	//end of algorithm execution
			double execution_time = double(end_time - start_time) / double(CLOCKS_PER_SEC);	//compute the execution time in seconds
			sumOfExecutionTime = sumOfExecutionTime + execution_time;
			cout<<endl<<"No applicable Task in Table"<<endl;
			cout<<endl<<"Execution time of Algorithm1 is "<<execution_time*1000000<<" microseconds"<<endl;	
			continue;
		}
		//sort similarityScore vector by similarityScore and return the corresponding highest score's TaskID
		
		sort(similarityScoreVector.begin(), similarityScoreVector.end(), sortSimilarityScore_DescendingOrder);	//sort similarityScoreVector
		end_time = clock();	//store the time at the end of algorithm1 execution
		//auto end_of_execution = high_resolution_clock::now();	//using a different method to measure execution time
		cout<<endl<<"End of Algorithm1 execution"<<endl;	//end of algorithm execution
		
		cout<<endl<<"TaskID with highest similarity score is TaskID_"<<similarityScoreVector[0].first<<" and score is "<<similarityScoreVector[0].second<<endl;		
		
		//calculate the execution time
		double execution_time = double(end_time - start_time) / double(CLOCKS_PER_SEC);	//compute the execution time in seconds
	 	//auto duration_of_execution = duration_cast<microseconds>(end_of_execution - start_of_execution);	//compute the execution time
	 	sumOfExecutionTime = sumOfExecutionTime + execution_time;
	 	//sumOfDurationTime = sumOfDurationTime + double((end_of_execution - start_of_execution)); not working
	 	
		//cout<<endl<<"Execution time of Algorithm1 is "<<execution_time<<" seconds"<<endl;
		cout<<endl<<"Execution time of Algorithm1 is "<<execution_time*1000000<<" microseconds"<<endl;	
		//cout<<endl<<"Duration of execution of Algorithm1 is " << duration_of_execution.count() << " microseconds" << endl;
	}
	
	//compute average execution time
	averageExecutionTime = (sumOfExecutionTime / repetition) * 1000000;	//convert seconds to microseconds
	//averageDurationTime = sumOfDurationTime / repetition;	//unit is already cast as microseconds
	//cout<<endl<<"^^^^^^^ Average duration of execution of Algorithm1 is " <<averageDurationTime<< " microseconds ^^^^^^^^^" << endl;
	cout<<endl<<"^^^^^^^ Average execution time of Algorithm1 is "<<averageExecutionTime<<" microseconds ^^^^^^^^"<<endl;
	
	return 0;
}
