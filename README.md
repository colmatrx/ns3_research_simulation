I added IoT_TaskV1.1.cc and IoT_taskQueuev1.3.cc

The Server class and Task Class are located in IoT_TaskV1.1.cc

The TaskQueue Class and functions are is located in IoT_taskQueuev1.3.cc

For testing purposes, the IoT_taskQueuev1.3.cc file includes IoT_TaskV1.1.cc at the beginning

IoT_taskQueuev1.3.cc contains the main function in order to test the implementation so far.

Code development is still ongoing and the repository will be updated as soon as possible.

In addition to functions addTask, getTaskServerIDsByID, getTaskServersByID and getTaskObjectsByID, functions updateTaskResult, deleteTaskByID and displayQueue have 

been added. Additional testing was done in the main() function. Now working on selectServer() function.

I have implemented selectServers() in IoT_taskQueuev1.5.cc

I have implemented cleanUp() function in IoT_taskQueuev1.7.cc. The cleanUp function accepts a float threshold range between 1 and 100, and delete TaskIDs that meet or exceed the passed threshold. It returns the number of TaskIDs deleted or 0 if no TaskID was deleted.

Updates to IoT_TaskV1.2.cc: clientID has been removed from the class attributes

Updates to IoT_taskQueuev1.8.cc: addTask() has been modified to add_Update_Task(). selectServers() has been updated to accept additional arguments (TaskID, currentServer ad numberOfSelectServers )and skip the current working server and any servers that have seen the TaskID before from its returned set.

A contructor has been added in IoT_TaskQueuev1.9.cc. Inside this constructor, server initializations are done with a special initialization TaskID 000. This initialization task is excluded from cleanUp() function to prevent it from being deleted. However, a malicious server will be removed from the initialization Task when removeMaliciousServer() function is implemented so that such server is no longer part of selectServer() process.

removeMaliciousServer() function has been implemented in IoT_TaskQueuev2.0.cc. It takes a float perccentage value between 1 and 100 and remove servers that meet or exceed the percentage of maliciousness.Threshold is calculated by how many times a server was malicious out of how many times it was used.

Makefile has also been updated. Now you can compile by first typing 'make clean' followed by 'make all'

I have updated IoT_Task to IoT_Taskv1.3.cc and IoT_taskQueue to IoT_TaskQueuev2.1.cc

I have added the voting function named conductELections() to IoT_TaskQueuev2.1.cc

I also updated the Makefile so now a single command of "make clean" will compile the code

I added IoT_Taskv1.4 and IoT_TaskQueuev3.1
Changes made in IoT_TaskQueuev3.1a -> I renamed updateProbeServers() as updateProbeServerFrequency(). And displayProbeQueue() now displays tasks with one server.

I have uploaded IoT_TaskQueuev3.2 with more functions. I have tested them fairly but will provide README details tomorrow.
