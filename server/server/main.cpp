/**
<<<<<<< Updated upstream
 @file  main.cpp - Driver file which runs the server side of the application. 
 @authors Phuc T, Narissa T
 @date 3/13/22
 @version 3.0
=======
 * @file main.cpp
 * @authors Phuc T, Narissa T
 * @date 3/13/22
 * @version 2.0
>>>>>>> Stashed changes
 */

#include <cstdio>       // printf
#include <iostream>    
#include "RPCServer.h"

/** Start of RPCServer.cpp 
 * @param argc
 * @param argv
 * @return int 
 */
int main(int argc, char const* argv[])
{

	const int port = 10327; 

	cout << "main: Starting in main...\n";
	const char* serverIP = argv[1]; // Holds server's IP address. 
	// int port = atoi(argv[2]);       // holds port number. 
	bool statusOk = true;           // Holds server status, true when server is running. 

	RPCServer* serverObj = new RPCServer(serverIP, port);
	statusOk = serverObj->StartServer();

	while (statusOk) {
		statusOk = serverObj->ListenForClient();
	}

	delete serverObj;
	return 0;
}
