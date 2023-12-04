#make file - use 'make clean' command to compile
 
clean:  #target name
		$(RM) iot
		g++ TaskQMain.cc IoT_Taskv1.5.cc -o iot
