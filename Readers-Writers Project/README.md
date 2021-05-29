# Coordinator

Coordinator is a simple implementation of the readers-writers problem using POSIX semaphores and SYSTEM V shared memory.  
The program spawns processes each responsible for reading and writing on shared resources.

## Compiling

```bash
make
```

In the project directory and:

```bash
make clean
```

To delete the object files and executables.

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
time ./coordinator 100 100 100

```
