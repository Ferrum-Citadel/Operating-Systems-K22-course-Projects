
# set compiler options here
CFLAGS=-g -Wall 

# set the name of the executable file to compile here
program=coordinator

# list object files needed to compile the program
objects=Coordinator.o Functions.o

# this makes the program. It's the first rule so it will be executed if you give just 'make'
$(program) : $(objects)
	gcc $(objects) -o $(program) -lpthread -lrt -lm
	
# list .h dependencies for each .o (dependency on .c is automatically set by make!)
Coordinator.o : Coordinator.h
Functions.o : Coordinator.h


# this allows to quicly remove all generated files with 'make clean'
clean:
	rm -rf $(program) $(objects)

# run the program with 'make run'. Sometimes useful to avoid rewriting long commands.
run: $(program)
	./$(program) 1000 500 2
