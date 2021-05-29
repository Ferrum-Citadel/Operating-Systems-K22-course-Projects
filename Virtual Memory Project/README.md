# Virtual Memory simulator

In this project we were asked to implement a program that simulates the way a virtual memory works.  
We are using an [inverted page table](https://www.geeksforgeeks.org/inverted-page-table-in-operating-system/) to save the virtual page number of the processes along with its control bits. (The processes are simulated by 2 trace files).  
Each trace file contains hexadecimal memory references followed by "R" for reading and "W" from writing.
As a page replacement algorithm we are using [LRU](<https://en.wikipedia.org/wiki/Cache_replacement_policies#Least_recently_used_(LRU)>).

## Compilation

Inside the project directory:

```bash
make
```

For clearing the object files and executable use:

```bash
make clean
```

## Usage

Inside the project directory run:

```bash
./memory_manager arg1 arg2 arg3

```

Where:  
-arg1: Is the number of memory frames used.  
-arg2: Is the number of lines read from each trace file for each iteration(Simulated process).  
-arg3: Is the total max number of references read from the trace files.  
-arg4: Is the page replacement algorithm. ONLY "LRU" IS ACCEPTED AS A VALUE!

## Example

In the following example the low memory frame number given (arg1=100) results in many page faults as it shows in the exaple output.

```bash
./memory_manager 100 50 2000 LRU

```

### Output

```bash
----------STATISTICS FOR PROCESS: 1
-Page faults: 122
-Reads from disk: 122
-Writes to disk: 69
-References examined: 1000
-Frames used at exit: 100

----------STATISTICS FOR PROCESS: 2:
-Page faults: 312
-Reads from disk: 312
-Writes to disk: 137
-References examined: 1000
-Frames used at exit: 100

----------STATISTICS IN GENERAL
-Page faults: 434
-Reads from disk: 434
-Writes to disk: 206
-References examined: 2000
-Frames used at exit: 200

[pitas@Ironcastle Virtual Memory Project]$ ./memory_manager 100 50 2000 LRU
----------STATISTICS FOR PROCESS: 1
-Page faults: 122
-Reads from disk: 122
-Writes to disk: 69
-References examined: 1000
-Frames used at exit: 100

----------STATISTICS FOR PROCESS: 2:
-Page faults: 312
-Reads from disk: 312
-Writes to disk: 137
-References examined: 1000
-Frames used at exit: 100

----------STATISTICS IN GENERAL
-Page faults: 434
-Reads from disk: 434
-Writes to disk: 206
-References examined: 2000
-Frames used at exit: 200

```
