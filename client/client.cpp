#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include <string> 
#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;


int main(int arcg , char **argv){
    int fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in adder = {0};
    adder.sin_family = AF_INET;
    adder.sin_addr.s_addr = inet_addr(argv[1]);
    adder.sin_port = htons(stoi(argv[2]));
    connect(fd, (sockaddr*)&adder,sizeof(adder));
    std::stringstream ss;
    for (int i = 3; i < arcg; ++i) {
        ss << argv[i] << " ";
    }
    std::string message = ss.str();

    ssize_t bytes_written = write(fd, message.c_str(), message.size());

    const char* end_marker = "\n"; 
    bytes_written = write(fd, end_marker, 1);

    char buffer[1024];
    ssize_t bytes_received;
    while ((bytes_received = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << buffer;

        if (memchr(buffer, '\n', bytes_received) != nullptr) {
            break; // Stop reading further data
        }

    }
    close(fd);
}