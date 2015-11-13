/** Socket Demo 4
* This iteration makes the communication two-way!
*/

#include "headers.h"

#define dprint(x) do { cout<<#x<<"="<<x<<endl; } while(0);
const char ACK = 0x6;

void sender(int connection) {
    while (true) {
        string toSend;
        getline(cin, toSend);
        toSend += '\0';
        send(connection, toSend.c_str(), toSend.size(), 0);
    }

    //No more ACK messages, since our users will be chatting two-way instead.
}

void reciever(int connection) {
    while (true) {
        char buf;
        string sbuf;
        while (true) {
            size_t bytesWritten = recv(connection, &buf, sizeof(buf), 0);
            if (buf == '\0') break;
            else sbuf += buf;
        }
        cout << sbuf << endl;
    }

    //No more ACK messages, since our users will be chatting two-way instead.
}

int main(int argc, char* argv[])
{
    bool isServer = argc <= 1;

    auto sock = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, 4);

    sockaddr_in addr;
    addr.sin_family = AF_INET;

    if (isServer) {
        addr.sin_port = htons(0);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        ::bind(sock, (sockaddr*)&addr, sizeof(addr));

        listen(sock, 0);

        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        getsockname(sock, (sockaddr*)&addr, &len);
        cout << "Server running on port " << ntohs(addr.sin_port) << endl;

        while (true) {
            auto clientConnection = accept(sock, 0, 0);
            /*
                We want two-way chat, so both the server and client have to
                send and recieve.
                Since we put them in two nice functions, we can just spin off
                a second thread!
            */
            thread(sender, clientConnection).detach();
            reciever(clientConnection);
        }
    }
    else {
        cout << "client mode" << endl;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        unsigned short portnum;
        cout << "connect to port: ";
        cin >> portnum;
        string junk; //throw away the newline so we don't send it later
        getline(cin, junk);
        addr.sin_port = htons(portnum);

        connect(sock, (sockaddr*)&addr, sizeof(addr));

        /*
           The client also gets a second thread.
        */
        thread(sender, sock).detach();
        reciever(sock);
    }
}

