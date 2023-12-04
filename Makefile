#make file - use 'make clean' command to compile
 
clean:  #target name
		$(RM) iot
		g++ TaskQMain.cc IoT_TaskQueuev3.3.cc IoT_Taskv1.5.cc -o iot
