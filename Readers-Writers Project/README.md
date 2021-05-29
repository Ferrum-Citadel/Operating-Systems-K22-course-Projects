# Coordinator

Coordinator is a simple implementation of the readers-writers problem using POSIX semaphores and SYSTEM V shared memory.  
The program spawns processes each responsible for reading and writing on shared resources.

## Compiling

Inside the project directory:

```bash
make
```

For clearing object files and the executable use:

```bash
make clean
```

## Usage

Inside the project directory run:

```bash
./coordinator arg1 arg2 arg3

```

Where:  
-arg1: Is the number of writing or reading iterations for each process.  
-arg2: Is the number of entries used in the shared memory segment.  
-arg3: Is readers to writers ratio. E.g if given "2" the reading processes will be twice as many as the writing ones.

## Example

Using time is useful for seeing how different arg values impact the efficiency of the algorithm

```bash
time ./coordinator 100 100 2

```

### Output

```bash
-------------------------PROCESS:21427-------------------------
Writer iterations: 42
Reader iterations: 58
Average waiting time for writers: 0.857143 microseconds
Average waiting time for readers: 0.017241 microseconds
Overall average waiting time: 0.370000 microseconds

-------------------------PROCESS:21428-------------------------
Writer iterations: 43
Reader iterations: 57
Average waiting time for writers: 0.976744 microseconds
Average waiting time for readers: 0.228070 microseconds
Overall average waiting time: 0.550000 microseconds

-------------------------PROCESS:21430-------------------------
Writer iterations: 28
Reader iterations: 72
Average waiting time for writers: 0.678571 microseconds
Average waiting time for readers: 0.027778 microseconds
Overall average waiting time: 0.210000 microseconds

-------------------------PROCESS:21429-------------------------
Writer iterations: 32
Reader iterations: 68
Average waiting time for writers: 0.718750 microseconds
Average waiting time for readers: 0.029412 microseconds
Overall average waiting time: 0.250000 microseconds

-------------------------PROCESS:21431-------------------------
Writer iterations: 31
Reader iterations: 69
Average waiting time for writers: 1.000000 microseconds
Average waiting time for readers: 0.028986 microseconds
Overall average waiting time: 0.330000 microseconds

-/-/-/-/-/--/-/-/-/--SHARED MEMORY STATS--/-/-/-/-/-/-/-/-/-/-/
Total reads in entries: 324
Total writes in entries: 176
Given ratio: 2.000000
SUM: 500

real    0m11,554s
user    0m0,011s
sys     0m0,007s

```
