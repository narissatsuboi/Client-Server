/**
 @file RPCServer.cpp
 @authors Phuc T, Narissa T
 @date 3/13/22
 @version 3.0
 */

#include <unistd.h>
#include <iostream>     // For C++ IO.
#include <stdio.h>
#include <sys/socket.h> // For UNIX socket structs.
#include <stdlib.h>
#include <netinet/in.h> // For constants and structs for INTERNET domain addresses.
#include <string.h>
#include <vector>
#include <iterator>
#include <pthread.h>

#include "RPCServer.h"
#include "RPCImpl.h"

using namespace std;



void* myThreadFun(void* vargp)
{

	sleep(1);

	int socket = *(int *)vargp;
	printf("RPCServer>myThreadFun: Calling ProcessRPC on socket\n");

	// Seed new RPCIml object with socket. 
	RPCImpl *rpcImplObj = new RPCImpl(socket);
	// ProcessRPC on the socket. 
	rpcImplObj->ProcessRPC();   // This will go until client disconnects;
	printf("RPCServer>myThreadFun: Done with Thread");
	return NULL;
}


RPCServer::RPCServer(const char* serverIP, int port)
{
    m_rpcCount = 0;
    m_serverIP = (char*)serverIP;
    m_port = port;
    mg = new MealGenerator();  // init new MG object
};


RPCServer::~RPCServer() {};


bool RPCServer::StartServer()
{

	// New socket constants 
	const int OPTVAL = 1;               // setsockopt option value
	const int BACKLOG = 10;             // Number of allowed connection requests before error thrown
	const int NAMESPACE = AF_INET;      // Internet namespace IP4 
	const int STYLE = SOCK_STREAM;      // Communication style for TCP programming
	const int PROTOCOL = 0;             

	// setsockopt constants
	const int LEVEL = SOL_SOCKET;
	const int OPTNAMES = SO_REUSEADDR;  // Socket level options to reuse address and port

	// Create socket file descriptor, exit on error or display success msg
	if ((m_server_fd = socket(NAMESPACE, STYLE, PROTOCOL)) == 0)
    {
		perror("RPCServer>StartServer: Socket creation error\n");
		exit(EXIT_FAILURE);
    }
	else
		cout << "RPCServer>StartServer: Socket creation successful\n";


	// TODO: NT Adjust the options when we switch to multithreading
    // forcefully attaching socket to the port
    if (setsockopt(m_server_fd, LEVEL, OPTNAMES, &OPTVAL, sizeof(OPTVAL)))
    {
		perror("RPCServer>StartServer: Error setting up socket options\n");
		exit(EXIT_FAILURE);
    }

	// init server socket address 
	struct sockaddr_in m_address;                      // server socket address struct
	m_address.sin_family = NAMESPACE;                  // server socket's address format is INET for IP4
	m_address.sin_addr.s_addr = INADDR_ANY;            // Server socket is bound to LOCAL HOST
	m_address.sin_port = htons(m_port);                // Assign server's port and convert to network bytes

    // Bind socket to port
    if (bind(m_server_fd, (struct sockaddr*)&m_address, sizeof(m_address)) < 0)
    {
		perror("RPCServer>StartServer: Error binding socket to address\n");
		exit(EXIT_FAILURE);
    }
	else
		cout << "RPCServer>StartServer: Socket binding successful\n";

	// Enable connection requests on server file descriptor, exit on error
    if (listen(m_server_fd, BACKLOG) < 0)
    {
		perror("RPCServer>StartServe: Error listening\n");
		exit(EXIT_FAILURE);
    }
	else
		cout << "RPCServer>StartServer: Server file descriptor listening for connection...\n";

    return true; // server started successfully
}


bool RPCServer::ListenForClient()
{
	int addrlen = sizeof(m_address);
	vector<pthread_t> thread_ids;
	pthread_t thread_id;

	for (;;) //TODO Endless loop. Probably good to have some type of controlled shutdown
	{
		if ((m_socket = accept(m_server_fd, (struct sockaddr*)&m_address,
			(socklen_t*)&addrlen)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		// Launch Thread to Process RPC
		//TODO:Join these thread ids, don't know where though. 
		thread_ids.push_back(thread_id);
		int socket = m_socket;

		// Create new thread with socket.
		printf("RPCServer>ListenForClient: Launching thread");
		pthread_create(&thread_id, NULL, myThreadFun, (void*)&socket);
	}
	return true;
}




