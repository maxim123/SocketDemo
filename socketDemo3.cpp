/** Socket Demo 2
* This iteration switches to automatic ports.
* The server picks an arbitrary unused port, and the client connects to that port.
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

        //Acknowledgement
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
            if (buf == '\0') break;
            else sbuf += buf;
        }
        cout << sbuf << endl;
        send(connection, &ACK, 1, 0);
    }
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
        /* Using automatic ports
           A port of zero requests that the OS pick its own port
        */
        addr.sin_port = htons(0);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        ::bind(sock, (sockaddr*)&addr, sizeof(addr));

        listen(sock, 0);

        /* Printing the Port
           Because we've switched to automatic ports, we have
           to ask the OS what port it decided to bind our server
           to. This is accomplished with getsockname(), which gets
           that addr struct OUT of the socket.
        */
        sockaddr_in addr; //that addr struct where we specify the port and IP
        socklen_t len = sizeof(addr); //because C
        getsockname(
            sock,             //the socket file descriptor
            (sockaddr*)&addr, //the structure we want our info put into
            &len              //the amount of data the OS put into said structure, because C
        );
        unsigned short port = ntohs(addr.sin_port); //have to convert from Big-endian back to little-endian. ntohs = "network to host short"
        cout << "Server running on port " << port << endl;

        while (true) {
            auto clientConnection = accept(sock, 0, 0);
            reciever(clientConnection);
        }
    }
    else {
        cout << "client mode" << endl;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        /*
            We will specify the server's port to the client on stdin
        */
        unsigned short portnum;
        cout << "connect to port: ";
        cin >> portnum;
        string junk; //throw away the newline so we don't send it later
        getline(cin, junk);
        addr.sin_port = htons(portnum);

        connect(sock, (sockaddr*)&addr, sizeof(addr));

        sender(sock);
    }
}

