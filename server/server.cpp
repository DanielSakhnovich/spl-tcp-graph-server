#include "ring.h"
#include "graph.h"
#include <thread>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>

using namespace std;
#define RING_SIZE 10


bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) {
        return false;
    }
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}


int contains(ring<std::string, RING_SIZE>& history, int v, int u) {
    for (int i = 0; i < RING_SIZE; i++) {
        if (startsWith(history[i], std::to_string(v) + " ") && endsWith(history[i], " " +std::to_string(u) + "\n")) {
            return i;
        }
    }
    return -1;
}


void handleRequest(int fd2, Graph& graph, ring<string, RING_SIZE>* myRing) {
    vector <int> input_numbers;
    char buffer[1024];
    ssize_t bytes_read = read(fd2, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0';
    std::stringstream ss(buffer);
    std::string substring;
    for (int i=0; i<2;i++) {
        std::getline(ss, substring, ' ');
        input_numbers.push_back(stoi(substring));
    }
    int containsIndex = contains(*myRing, input_numbers[0], input_numbers[1]);
    if(containsIndex >= 0){
        string cachedPath = (*myRing)[containsIndex];
        ssize_t bytes_written = write(fd2, cachedPath.c_str(), cachedPath.size());
    }
    else
    {
        vector<int> bfs_path = graph.BFS(input_numbers[0], input_numbers[1]);
        string step_as_str;
        string pathToCache;
        for (size_t i = 0; i < bfs_path.size(); ++i) {
            int step = bfs_path[i];
            if (i < bfs_path.size()-1){
                step_as_str = std::to_string(step)+ ' ';
                pathToCache += step_as_str;
                pathToCache += ' ';
            }
            else {
                step_as_str = std::to_string(step)+ '\n';
                pathToCache += step_as_str;
            }
            ssize_t bytes_written = write(fd2, step_as_str.c_str(), step_as_str.size());
        }
        if (bfs_path.size()==0){
            step_as_str = '\n';
            ssize_t bytes_written = write(fd2, step_as_str.c_str(), step_as_str.size());
        }else{
            myRing->push_back(pathToCache);
        }
        
    }
    close(fd2);
}

int main(int argc, char **argv) {
    Graph graph(argv[1]);
    ring<string, RING_SIZE>* myRing = new ring<string, RING_SIZE>();
    std::mutex ringMutex;    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }
    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(stoi(argv[2]));
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    if (listen(fd, 5) == -1) {
        perror("listen");
        close(fd);
        return 1;
    }
       
    for (;;) {
        int fd2 = accept(fd, nullptr, nullptr);
        if (fd2 == -1) {
            perror("accept");
            continue;
        }
        std::thread handleRequestThread([fd2, &graph, myRing, &ringMutex]() { 
            ringMutex.lock();
            handleRequest(fd2, graph, myRing); });
            ringMutex.unlock();
        handleRequestThread.detach();    
    }
    close(fd); 
    return 0;
}
