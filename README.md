# CampusCompute
### Intelligent Job Scheduling and Resource Management

A Linux-based Operating System project developed in C to demonstrate process management, CPU scheduling, synchronization, inter-process communication, deadlock avoidance, and resource management through an interactive command-line application.

---

## Overview

CampusCompute is designed to demonstrate how an operating system manages processes and system resources. It brings together multiple Operating System concepts in a single modular application.

The application provides an interactive interface through which users can execute different operations and observe their behaviour.

## Key Features

- **CPU Scheduling:** FCFS, Round Robin, and Priority Scheduling with performance metrics.
- **Deadlock Avoidance:** Banker's Algorithm with safe-state detection and safe-sequence generation.
- **Process Management:** Worker process creation, execution, termination, and controlled failure handling.
- **Resource Management:** Process resource restrictions using POSIX `setrlimit()`.
- **Concurrent Logging:** Multithreaded producer-consumer logging using POSIX threads.
- **Inter-Process Communication:** Shared memory and named semaphores for process communication and synchronization.

## Technologies Used

**Programming Language:** C

**Operating System:** Linux / Ubuntu

**Compiler:** GCC

**Build System:** GNU Make

**System Interfaces:** POSIX APIs and Linux system calls

**Core Concepts:** Process Management, CPU Scheduling, Multithreading, Synchronization, IPC, Deadlock Avoidance, and Resource Management.

## Project Architecture

The application follows a modular design, with each module implementing a specific Operating System functionality.

- `main.c` – Main application entry point and menu-driven interface.
- `job.c` and `job.h` – Job creation, initialization, and management.
- `scheduler.c` and `scheduler.h` – CPU scheduling algorithms and performance metrics.
- `banker.c` and `banker.h` – Banker's Algorithm and safe-state detection.
- `worker.c` and `worker.h` – Worker process creation and failure handling.
- `resource_limits.c` and `resource_limits.h` – Process resource limit management.
- `logger.c` and `logger.h` – Concurrent activity logging and thread synchronization.
- `ipc.c` and `ipc.h` – Inter-process communication using shared memory and semaphores.

## System Requirements

**Hardware**
- A computer capable of running Linux or Ubuntu.
- At least 2 GB RAM recommended.
- Sufficient disk space for the project and generated files.

**Software**
- Ubuntu 24.04 or a compatible Linux distribution.
- GCC compiler.
- GNU Make.
- POSIX-compatible environment.

## Installation and Setup

Install the required development tools:

    sudo apt update
    sudo apt install build-essential gcc make

Verify the installation:

    gcc --version
    make --version

Clone the repository:

    git clone <repository-url>

Navigate to the project directory:

    cd CampusCompute

## Compilation

The project uses GNU Make to automate compilation and linking.

Run the following command from the project root directory:

    make

**Compilation Workflow**

C Source Files
      |
      v
GCC Compilation
      |
      v
Object Files (.o)
      |
      v
Linking using Makefile
      |
      v
campuscompute Executable

## Execution

After successful compilation, run:

    ./campuscompute

The application displays an interactive menu through which users can select the required functionality.

**Application Workflow**

Launch Application
      |
      v
Display Main Menu
      |
      v
Select Functionality
      |
      v
Execute Selected Module
      |
      v
Display Results
      |
      v
Return to Main Menu

## Available Operations

1. FCFS Scheduling
2. Round Robin Scheduling
3. Priority Scheduling
4. Compare Scheduling Algorithms
5. Banker's Algorithm
6. Worker Process – Normal Execution
7. Worker Process – Simulate Failure
8. Resource Limit Demonstration
9. Concurrent Activity Logger
10. IPC – Shared Memory and Semaphore
11. Exit

The scheduling comparison uses a fixed Round Robin time quantum of 2 time units.

## Cleaning Build Files

To remove generated object files and the executable:
    
    make clean

This does not delete the original source or header files.

## Testing and Verification

The following functionalities are intended to be verified during final execution:

- Correct execution of all CPU scheduling algorithms.
- Accurate calculation of waiting and turnaround times.
- Safe and unsafe states using the Banker's Algorithm.
- Normal and controlled worker process execution.
- Process resource limit enforcement.
- Concurrent logging and thread synchronization.
- Parent-child communication using shared memory and semaphores.
- Proper cleanup of processes and IPC resources.

## Learning Outcomes

This project provides practical experience with:

- Linux process management and system calls.
- CPU scheduling and performance evaluation.
- Multithreading and concurrent execution.
- Mutual exclusion and synchronization.
- Inter-process communication.
- Deadlock avoidance.
- Process failure handling.
- Operating System resource management.

## Conclusion

CampusCompute demonstrates essential Operating System mechanisms through a modular C application. By integrating process scheduling, synchronization, IPC, deadlock avoidance, and resource management, the project provides a practical understanding of how operating systems coordinate processes and allocate system resources.

---

