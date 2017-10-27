#include <fcntl.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
using namespace std;

class Client
{
  public:
    Client();
    ~Client();
    bool config(const string &ip, const int &port); //set server ip and port
    bool c();                                       //connect to server
    bool sendAns(int i);                            //send your answer to server

  private:
    int sock;
    bool connected;
    sockaddr_in serv_addr;
    char str[256];
};

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
//You may ignore the implementation of Client and see the sample program below directly
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

Client::Client()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    connected = false;
}

bool Client::config(const string &ip, const int &port)
{
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
        return false;
    serv_addr.sin_port = htons(port);
    return true;
}

bool Client::c()
{
    if (connect(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << errno << " connection error.\n";
        return false;
    }
    connected = true;
    return true;
}

bool Client::sendAns(int i)
{
    if (!connected && !c())
        return false;
    if (i < 0 || i > 9)
    {
        cerr << "Invalid input! Please check your code." << endl;
        return false;
    }
    str[0] = i + '0';
    str[1] = '\0';
    if (send(sock, str, strlen(str) * sizeof(char), 0) < 0)
    {
        cerr << "Sending error" << endl;
        connected = false;
        return false;
    }
    return true;
}

Client::~Client()
{
    close(sock);
}

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
//Sample program here
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

Client client;

void exit_handler(int a)
{
    client.~Client();
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, exit_handler); // exit_handler(int) will be called when ctrl+c is pressed, because we have to make sure destructor of Client will always run when exiting, otherwise next socket connection trial may fail
    if (argc != 2)
    {
        cerr << "Please enter ip address.\n";
        exit(1);
    }
    if (client.config(argv[1], 4331) == false) // our server listens port 4331
    {
        cerr << "Invalid ip.\n";
        exit(1);
    }
    while (!client.c())
        sleep(1); // try to connect until success
    cout << "successfully connect\n";
    int answer;
    while (1)
    {
        cin >> answer;
        if (answer == -1)
            break;
        cout << client.sendAns(answer) << endl; //check the status
    }
    return 0;
}
