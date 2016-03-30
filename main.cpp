////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client/Server Rotor Encryption Simulation
//
// Jeremy J. Hartmann
// 2016
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "RotorEncryption.h"
#include "DiffieHellman.h"

using namespace std;

#define MCHARS 94

// SIGCHLD Handler
void handle_sigchld(int sig)
{
    // cout << "SENT SIGCHLD" << endl;
    int serrno = errno;
    while (waitpid((pid_t) -1, 0, WNOHANG) > 0) {
        // Do until all child processes are reaped.
    }
    errno = serrno;
}

// Error Function
void error(string errorMessage)
{
    perror(errorMessage.c_str());
    exit(1);
}

// Forward Declarations
void StartServer(int portNumber);
void InfiniteRun(int csfd);
void StartClient(string hostname, int portnumber);
void buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main
int main()
{
    cout << "Rotor Encryption With Diffie-Hellman" << endl;

    cout << "Choose Option:\n\tBuild Encryption Schemes: 0\n\tSet Up Client: 1\n\tSet Up Server: 2\n\n Answer: ";
    string choice;
    cin >> choice;
    int choiceVal = atoi(choice.c_str());

    if (choiceVal > 2 || choiceVal < 0)
    {
        cout << "Invalid Choice." << endl;
        return 1;
    }

    if (choiceVal == 0)
    {
        // Build the encryption scheme to be shared between the server and client.
        // Output is a flatfile in .txt format
        string scheme;
        string rotor;

        cout << "Number of encryption schemes: ";
        cin >> scheme;
        cout << "\nNumber of rotors per scheme: ";
        cin >> rotor;
        int schemeCount = atoi(scheme.c_str());
        int rotorCount = atoi(rotor.c_str());
        RotorEncryption::buildEncryptionSchemeFlatFile(rotorCount, schemeCount);

    }
    if (choiceVal == 1)
    {
        // Setup host and portnuymber
        string hostname;
        cout << "\nClient Start... \nHost: ";
        cin >> hostname;

        string portstr;
        cout << "\nPort: ";
        cin >> portstr;
        int port = atoi(portstr.c_str());

        // Set up the Client
        StartClient(hostname, port);

//        RotorEncryption device;
//        device.setSchemeId(0);
//        string message = "Hello this is a string and a test to see how much this will work. I am writing to you today to warn you about the never ending waterfall in the distance horizon. I know this may come to you as a skock but this needs to be addressed in oder to prevent a grave catatastraphy.";
//        string response = device.encrypt(message);
//        cout << response << endl;
//        string decrypted = device.decrypt(response);
//        cout << decrypted << endl;
    }
    if (choiceVal == 2)
    {
        int bufferLen = 1024;
        char buffer[bufferLen];

        string portstr;
        cout << "\nServer Start...\nPort: ";
        cin >> portstr;
        int port = atoi(portstr.c_str());

        // Set up the Server.
        StartServer(port);

//        DiffieHellman HostA;
//
//        int G = HostA.getmG();
//        int P = HostA.getmP();
//        int A = HostA.getmA();
//
//        DiffieHellman HostB(G, P);
//        int B = HostB.getmA();
//
//        // Gen Shared Key
//        HostA.gen(B);
//        HostB.gen(A);
//
//        int encrypt = HostA.encrypt(4);
//        int decrypt = HostB.decrypt(encrypt);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Server

void StartServer(int portNumber)
{
    // Config variables
    int serverSocketFileDesc = -1;
    int clientSocketFileDesc = -1;
    socklen_t clientLen;

    // Socket Structure to use when making connections
    struct sockaddr_in serverAddress, clientAddress;

    // Create TCP socket over IP.
    if ((serverSocketFileDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Error: serverSocketFileDesc failed to instantiate. Error Code: " << serverSocketFileDesc << endl;
        error("Error: socket() for server failed.");
    }

    // Set server address to zero
    bzero((char *) &serverAddress, sizeof(serverAddress));

    // Configure Server Address.
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber); // Convert to network byte order.
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket file desc to server address
    if (bind(serverSocketFileDesc, (sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
        error("Error: Failed to bind socket to server address.");
    }

    // Start listen on socket
    listen(serverSocketFileDesc, 5);
    clientLen = sizeof(clientAddress);

    // Set up sigchld handler
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) < 0)
    {
        error("ERROR: Failed to setup SIGCHLD handler.");
    }

    // Start server loop
    while (true)
    {
        // Set up connection
        if ((clientSocketFileDesc = accept(serverSocketFileDesc, (sockaddr *) &clientAddress, &clientLen)) < 0) {
            error("ERROR: Failed to set up connection with client");
        }

        pid_t pid;
        if ((pid = fork()) < 0)
        {
            error("Failed to create process");
        }
        else if (pid == 0)
        {
            // Process the client connection
            close(serverSocketFileDesc);
            InfiniteRun(clientSocketFileDesc);

            // end process
            exit(0);
        }
        else
        {
            // Close the connection in the parent.
            close(clientSocketFileDesc);
        }


    }

    close(serverSocketFileDesc);
}

void InfiniteRun(int csfd)
{
    cout << "ENTERED PROCESS FOR CLIENT" << endl;
    // Set up buffer and connection
    srand(time(0));
    int bufferLen = 1024;
    char buffer[bufferLen];
    bool exit = false;
    int byteCount;

    // Establish shared key and encryption Scheme
    DiffieHellman encryption;
    long int G = encryption.getmG();
    long int P = encryption.getmP();
    long int A = encryption.getmA();

    // PUBLIC KEY G
    bzero((char *) buffer, bufferLen);
    memcpy(&buffer, &G, sizeof(long int));

    // Send public key G to client B
    if ((byteCount = send(csfd, buffer, sizeof(long int), 0)) < 0) {
        error("ERROR: Failed to send to client");
    }

    cout << "Send public key G: " << G << endl;
    // Wait for client Confirmation
    bzero((char *) buffer, bufferLen);
    if ((byteCount = recv(csfd, buffer, bufferLen, 0)) < 0) {
        error("ERROR: Failed to read form buffer");
    }

    // PUBLIC KEY P
    bzero((char *) buffer, bufferLen);
    memcpy(&buffer, &P, sizeof(long int));

    // Send public key P to client B
    if ((byteCount = send(csfd, buffer, sizeof(long int), 0)) < 0) {
        error("ERROR: Failed to send to client");
    }

    cout << "Send public key P: " << P << endl;
    // Wait for client confirmation
    bzero((char *) buffer, bufferLen);
    if ((byteCount = recv(csfd, buffer, bufferLen, 0)) < 0) {
        error("ERROR: Failed to read form buffer");
    }

    // PUBLIC KEY A
    bzero((char *) buffer, bufferLen);
    memcpy(&buffer, &A, sizeof(long int));

    cout << "Send public key A: " << A << endl;
    // Send public key A to client B
    if ((byteCount = send(csfd, buffer, sizeof(long int), 0)) < 0) {
        error("ERROR: Failed to send to client");
    }

    // Wait for client to send public key B
    bzero((char *) buffer, bufferLen);
    if ((byteCount = recv(csfd, buffer, bufferLen, 0)) < 0) {
        error("ERROR: Failed to read form buffer");
    }

    long int B;
    memcpy(&B, &buffer, sizeof(long int));
    cout << "Receive public key B: " << B << endl;

    // Create Shared Key
    encryption.gen(B);

    // Create Rotor Encryption Simulator
    RotorEncryption rotorMachine;
    int schemeId = rand() % rotorMachine.getSchemeCount();
    rotorMachine.setSchemeId(schemeId);

    // Send schemeId to client
    int encryptId = encryption.encrypt(schemeId);

    // SEND SCHEMEID
    bzero((char *) buffer, bufferLen);
    memcpy(&buffer, &encryptId, sizeof(long int));

    cout << "Send encrypted scheme id: " << encryptId << endl;

    // Send encrypted scheme id
    if ((byteCount = send(csfd, buffer, 14, 0)) < 0) {
        error("ERROR: Failed to send to client");
    }

    // Wait for client to send confirmation
    bzero((char *) buffer, bufferLen);
    if ((byteCount = recv(csfd, buffer, bufferLen, 0)) < 0) {
        error("ERROR: Failed to read form buffer");
    }

    // Enter loop for communication
    while (!exit)
    {
        bzero((char *) buffer, bufferLen);
        if ((byteCount = recv(csfd, buffer, bufferLen, 0)) < 0) {
            error("ERROR: Failed to read form buffer");
        }

        cout << "Received bytes: " << byteCount << endl;
        cout << "Received message: " << buffer << endl;

        // Send message back to client.
        if ((byteCount = send(csfd, "RECEIVED MESSAGE", 14, 0)) < 0) {
            error("ERROR: Failed to send to client");
        }

        exit = (strncmp("exit", buffer, 4) == 0);
    }

    cout << "EXIT PROCESS FOR CLIENT" << endl;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client

void StartClient(string hostName, int portNumber)
{
    // Check to see if host is name or address
    cout << "Is host identified by name? (Y/n): ";
    string answer;
    cin >> answer;


    int clientSocketFileDesc = -1;
    sockaddr_in serverAddress;
    struct in_addr serverAddr;
    hostent *server;

    // Setup socket
    if ((clientSocketFileDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR: Failed to create socket.");
    }

    // get server by hostname
    if (answer == "Y")
    {
        // Use the hostname
        if ((server = gethostbyname(hostName.c_str())) == NULL) {
            error("ERROR: Failed to get server hostname");
        }
    }
    else
    {
        // Use IP address
        inet_pton(AF_INET, hostName.c_str(), &serverAddr);
        if ((server = gethostbyaddr(&serverAddr, sizeof(serverAddr), AF_INET)) == NULL) {
            error("ERROR: Failed to get server address");
        }
    }
    // Zero out serverAddress and add configuration
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy(server->h_addr, (char *) &serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);


    // Connect client socket to server
    if (connect(clientSocketFileDesc, (sockaddr *) &serverAddress, sizeof(serverAddress))) {
        error("Error: Failed to connect to server.");
    }

    // Ask user for message to send server
    int buff_size = 1024;
    char buffer[buff_size];
    int byteCount;


    // ESTABLISH CONNECTION

    // Receive Public Key G
    bzero(buffer, buff_size);
    if ((byteCount = recv(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
        error("ERROR: Could not read from sever.");
    }

    long int G;
    memcpy(&G, &buffer, sizeof(long int));
    cout << "Received public key G: " << G << endl;
    if ((byteCount = send(clientSocketFileDesc, "OK", buff_size, 0)) < 0) {
        error("ERROR: sending message to server.");
    }

    // Receive Public Key P
    bzero(buffer, buff_size);
    if ((byteCount = recv(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
        error("ERROR: Could not read from sever.");
    }

    long int P;
    memcpy(&P, &buffer, sizeof(long int));
    cout << "Received public key P: " << P << endl;
    if ((byteCount = send(clientSocketFileDesc, "OK", buff_size, 0)) < 0) {
        error("ERROR: sending message to server.");
    }

    // Setup Diffie-Hellman
    DiffieHellman encryption(G, P);

    // Receive Public Key B
    bzero(buffer, buff_size);
    if ((byteCount = recv(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
        error("ERROR: Could not read from sever.");
    }

    long int B;
    memcpy(&B, &buffer, sizeof(long int));
    cout << "Received public key B: " << B << endl;
    encryption.gen(B);

    // Send Public Key A
    bzero(buffer, buff_size);
    long int A = encryption.getmA();
    memcpy(&buffer, &A, sizeof(long int));
    cout << "Send Public Key A: " << A << endl;
    if ((byteCount = send(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
        error("ERROR: sending message to server.");
    }

    // Get Encrypted Scheme Id.
    bzero(buffer, buff_size);
    if ((byteCount = recv(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
        error("ERROR: Could not read from sever.");
    }

    // Confirm schemid recieved.
    if ((byteCount = send(clientSocketFileDesc, "OK", buff_size, 0)) < 0) {
        error("ERROR: sending message to server.");
    }

    int schemeId;
    memcpy(&schemeId, &buffer, sizeof(int));
    cout << "SCHEME ID: " << schemeId << endl;

    // Set up rotorMachine
    RotorEncryption rotorMachine;
    rotorMachine.setSchemeId(schemeId);


    bool exit = false;
    while (!exit){
        cout << "Enter Message to send to server: ";
        string tmp;
        cin.ignore();
        getline (cin,tmp);

        bzero((char *)buffer, buff_size);
        memcpy(&buffer, tmp.c_str(), sizeof(char) * tmp.length());

        if ((byteCount = send(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
            error("ERROR: sending message to server.");
        }

        // Receive response from sever
        bzero(buffer, buff_size);
        if ((byteCount = recv(clientSocketFileDesc, buffer, buff_size, 0)) < 0) {
            error("ERROR: Could not read from sever.");
        }

        cout << "Message from server: " << buffer << endl;
        exit = (tmp == "exit");
    }
    // Close connections
    close(clientSocketFileDesc);

}

