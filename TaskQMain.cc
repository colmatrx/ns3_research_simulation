#include <iostream>
#include "IoT_TaskQueuev3.3.h"
#include "IoT_Taskv1.5.h"

using namespace std;

int main(){
	
	vector<string> myNodes;
	myNodes.push_back("node1"); myNodes.push_back("node2"); myNodes.push_back("node3"); myNodes.push_back("node4"); myNodes.push_back("node10");
	TaskQueue myQueue(myNodes);		//create object of class TaskQueue
	myQueue.displayProbeServer_Frequency();
	myQueue.updateProbeServerFrequency(myNodes);
	myQueue.displayProbeServer_Frequency();
	myQueue.updateProbeServerFrequency("node5");
	myQueue.displayProbeServer_Frequency();
	myQueue.updateProbeServerFrequency("node6");
	myQueue.displayProbeServer_Frequency();
	myQueue.initializeProbeServer("node7");
	myQueue.updateProbeServerFrequency("node8");
	myQueue.updateProbeServerFrequency("node9");
	myQueue.displayProbeServer_Frequency();
	cout<<endl<<"Calling selectProbeServers() function with an arguments of server2, 3"<<endl<<endl;
		
	vector<string> selectedServers;
		
	selectedServers = myQueue.selectProbeServers("server2", 3);
	
	cout<<"servers selceted are "<<selectedServers.size()<<" and listed below"<<endl<<endl;
	
	for (int i=0; i<selectedServers.size(); i++){
	
		cout<<selectedServers[i]<<endl<<endl;
	}
	myQueue.displayProbeQueue();
	Task task1("101", "12:30", "", "node1", "230", 1);
	Task task2("101", "12:30", "", "node2", "230", 1);
	Task task3("101", "12:30", "", "node3", "231", 1);
	myQueue.add_Update_Task(task1);
	myQueue.add_Update_Task(task2);
	myQueue.add_Update_Task(task3);
	Task task4("102", "13:30", "", "node4", "130", 1);
	Task task5("102", "13:30", "", "node5", "130", 1);
	Task task6("102", "13:30", "", "node6", "930", 1);
	myQueue.add_Update_Task(task4);
	myQueue.add_Update_Task(task5);
	myQueue.add_Update_Task(task6);
	Task task7("103", "23:30", "", "node7", "", 1);
	Task task8("103", "23:30", "", "node8", "", 1);
	Task task9("103", "23:30", "", "node9", "", 1);
	myQueue.add_Update_Task(task7);
	myQueue.add_Update_Task(task8);
	myQueue.add_Update_Task(task9);
	Task task10("104", "20:30", "", "node7", "12", 1);
	Task task11("104", "20:30", "", "node8", "22", 1);
	Task task12("104", "20:30", "", "node9", "12", 1);
	Task task13("110", "20:30", "", "node10", "12", 1);
	myQueue.add_Update_Task(task10);
	myQueue.add_Update_Task(task11);
	myQueue.add_Update_Task(task12);
	myQueue.add_Update_Task(task13);
	cout<<"before voting"<<endl;
	myQueue.displayProbeQueue();
	cout<<"now voting"<<endl;
	myQueue.voteOnProbeQueue(3, 2);
	cout<<"after voting"<<endl;
	myQueue.displayVerificationQueue();
	myQueue.displayProbeQueue();
	cout<<"selecting Task to verify node1"<<endl;
	TaskToVerify verificationTask = myQueue.selectTaskToVerify("node1");
	cout<<"TaskID to verify node1 is "<<verificationTask.TaskID<<" and TaskVotedResult is "<<verificationTask.TaskVotedResult<<endl;
	cout<<"updating Verification Queue for node1 with Task 102"<<endl;
	myQueue.updateVerificationQueue("102","node1","201", -1);
	cout<<"after updating VerificationQueue"<<endl;
	myQueue.displayVerificationQueue();
	vector<string> mal = myQueue.blacklistMaliciousVotingServers(1);	 
	myQueue.displayBlacklist();
	string freq = myQueue.displayProbeServer_Frequency();	cout<<freq<<endl;
	myQueue.initializeProbeServer("node1");
	myQueue.updateProbeServerFrequency("node8");
	//myQueue.removeMaliciousServerFromBlacklist("node6");
	myQueue.displayBlacklist();
	myQueue.displayProbeServer_Frequency();
	int verify = myQueue.cleanUpVerificationQueue(40);
	myQueue.displayVerificationQueue();		
	verify = myQueue.cleanUpVerificationQueueByTaskID("104", 30);
	myQueue.displayVerificationQueue();
	return 0;
}
