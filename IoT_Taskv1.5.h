#ifndef IOT_TASK_H
#define IOT_TASK_H

#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include <bits/stdc++.h>
#include<set>
#include<algorithm>
#include<utility>
#include <sstream>
#include<unistd.h>		//needed for sleep() function
#include <iterator>
using namespace std;

class Servers{
	
	public:
	string serverID;
	int colludingServerFlag;	//1 for Colluding server, 0 for Benign server
	//string serverResult;
	//int serverFlag;
};//end of Servers class


class Task{

	//class variables	
	public:
	int TaskID;
	int taskFeature[4];	//to hold four task features
	int poisonedTaskFlag;	//1 for poisoned task, 0 for non-poisoned task
	//string TaskTimestamp;
	//string TaskVotedResult;
	Servers server;	//an object of the class Servers above
	
	//class constructor declarations
	
	Task(int TaskID, string serverID);	//constructor with 2 parameters	
		
	Task(int TaskID, int taskFeature[4], int poisonedTaskFlag, string serverID, int colludinsServerFlag);	//constructor with all Task properties
	
		
};//end of class Task

#endif
