/** Socket Demo 2
  * This iteration implements acknowledgement messages.
  * This way, the sender knows their messages have been recieved.
*/
#include "headers.h"

#define dprint(x) do { cout<<#x<<"="<<x<<endl; } while(0);
const char ACK = 0x6; //this is the Acknowledge byte in ASCII

void sender(int connection) {
    while (true) {
        string toSend;
        getline(cin, toSend);
        toSend += '\0'; //we need a definition of the "end of a message now"
        send(connection, toSend.c_str(), toSend.size(), 0);

        /* Acknowledgement
            The client is allowed to recieve from the same connection
            (sockets are bidirectional). Here, we wait for the 
            server to acknowledge that it got a message
        */
        char val;
        recv(connection, &val, sizeof(val), 0);
        if (val == ACK) {
            cout << "\tMessage received" << endl;
        }
    }
}

void reciever(int connection) {
    while (true) {
        char buf;
        string sbuf;
        while (true) {
            size_t bytesWritten = recv(connection, &buf, sizeof(buf), 0);
            /*
                Here's where we check if we've gotten the whole message.
                Simply keep recieving until we get a null byte.
            */
            if (buf == '\0') break;
            else sbuf += buf;
        }
        cout << sbuf << endl;
        /* Acknowlegement
           Here's where we send that ACK byte, over the same connection

        */
        send(connection, &ACK, 1, 0);
    }
}

int main(int argc, char* argv[])
{
    unsigned short port = 3778;

    bool isServer = argc <= 1;

    auto sock = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, 4);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (isServer) {
        cout << "server mode" << endl;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        ::bind(sock, (sockaddr*)&addr, sizeof(addr));

        listen(sock, 0);

        while (true) {
            auto clientConnection = accept(sock, 0, 0);
            reciever(clientConnection);
        }
    }
    else {
        cout << "client mode" << endl;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        connect(sock, (sockaddr*)&addr, sizeof(addr));
        sender(sock);
    }
}

