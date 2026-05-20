# spl-tcp-graph-server

A multithreaded TCP server that finds shortest paths in an undirected graph using BFS, with a fixed-size ring buffer for caching recent results.

## Project Structure

```
server/
  server.cpp            # TCP server, spawns a thread per client connection
  graph.h / graph.cpp   # Undirected graph with BFS pathfinding
  ring.h                # Fixed-size circular buffer (cache)
  std_lib_facilities.h  # Utility header
client/
  client.cpp            # TCP client
```

## How It Works

The server loads a graph from a file and listens on a TCP port. When a client sends two node IDs, the server:
1. Checks the ring buffer cache for a previously computed path
2. If not cached, runs BFS to find the shortest path and caches the result
3. Returns the path as a space-separated list of node IDs

## Build

```bash
# Server
g++ -std=c++11 -pthread server/server.cpp server/graph.cpp -o server

# Client
g++ -std=c++11 client/client.cpp -o client
```

## Usage

```bash
# Start the server with a graph file and port
./server graph.txt 9034

# Query shortest path between node 1 and node 5
./client 127.0.0.1 9034 1 5
```

## Graph File Format

Each line defines an undirected edge between two nodes:

```
1 2
2 3
3 5
1 4
```
