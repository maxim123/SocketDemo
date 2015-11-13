/** Socket Demo 1
* This is the first version of our socket library.
* It sends messages from one person to another.
* Half of a chat app.
*/


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <iostream>
using namespace std;

#include "headers.h"

#define dprint(x) do { cout<<#x<<"="<<x<<endl; } while(0);

/* 
    Given a connection file descriptor, this function will
    read lines from stdin and send whatever it reads over the network.
*/
void sender(int connection) {
    while (true) {
        string toSend;
        getline(cin, toSend);
        send(connection,	//socket file descriptor
            toSend.c_str(), //message to send
            toSend.size(),	//size of message
            0				//flags (none yet)
        );
    }
}

void reciever(int connection) {
    char buf[4096];
    while (true) {
        size_t bytesWritten = recv(
            connection,		//socket file descriptor
            buf,			//buffer to put data in
            sizeof(buf) - 1,//max amount of data to read
            0				//flags
        );

        buf[bytesWritten] = 0;
        cout << buf << endl;
    }
}

int main(int argc, char* argv[])
{
    unsigned short port = 3778;

    bool isServer = argc <= 1;

    auto sock = socket(AF_INET, SOCK_STREAM, 0); /* Open a TCP connection (SOCK_STREAM)
                                                    Over the internet (AF_INET)	
                                                 */

    int yes = 1;										 // These lines allow the port to be reused
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, 4); // otherwise, dying processes are annoying

    sockaddr_in addr; //structure will define how we connect
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); /*	have to switch byte ordering to big_endian
                                    htons = "host to network short"
                                 */

    if (isServer) {
        cout << "server mode" << endl;
        addr.sin_addr.s_addr = htonl(INADDR_ANY); /* IP Address goes here
                                                     However, server's listening, not connecting
                                                     Thus, we don't care about the IP
                                                  */

        //request a port from the os
        ::bind(
            sock,             // the socket we want to listen over
            (sockaddr*)&addr, // that address structure (contains our port number)
            sizeof(addr)      // the size of said structure, because C
        );

        //allow incoming connections on the socket
        listen(sock, 0); //at this point client can connect

        while (true) {
            //wait for a client to connect on the socket
            auto clientConnection = accept(sock, 0, 0);
            /*
                This returns once a client has connected
                It returns a new file descriptor for that specific connection
                (that way, you can have more than one connection open at once)
            */

            reciever(clientConnection);
        }
    }
    else {
        cout << "client mode" << endl;

        /* 
           Since this is the client, we DO care about what IP address we want to connect to
           the inet_addr function converts a hostname into a 32-bit binary IP address
        */
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        //open a connection to the server
        connect(
            sock,				//file descriptor (since we're connecting we only get one socket FD)
            (sockaddr*)&addr,	//the structure defining the IP and port we're connecting to
            sizeof(addr)		//the size of said structure
        );

        sender(sock);
    }
}

