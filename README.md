# Custom HTTP Web Server in C

A high-performance, multi-threaded HTTP Web Server built from scratch in C using Berkeley Sockets and POSIX Threads (`pthreads`).

## Features
- **Socket Programming:** Built using native IPv4 TCP sockets (`sys/socket.h`).
- **Concurrency:** Multi-threaded architecture (`pthreads`) handling simultaneous client requests.
- **HTTP/1.1 Protocol:** Basic implementation serving static HTML pages.

## How to Build and Run

### Prerequisites
- GCC Compiler
- Linux/macOS or WSL (Windows Subsystem for Linux)

### Steps
1. **Compile the server:**
   ```bash
   make