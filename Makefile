all: semaphor cond_variables 
	@echo "Everything compiled"

semaphor: semaphor.o
	@mkdir -p bin
	@gcc -pthread obj/semaphor.o -o bin/semaphor

semaphor.o: src/semaphor.c
	@mkdir -p obj
	@gcc -g -c src/semaphor.c -lphtread -L include -o obj/semaphor.o

cond_variables: cond_variables.o
	@mkdir -p bin
	@gcc -pthread obj/cond_variables.o -o bin/cond_variables

cond_variables.o: src/cond_variables.c
	@mkdir -p obj
	@gcc -g -c src/cond_variables.c -lphtread -L include -o obj/cond_variables.o


clean:
	@rm -rf obj/* bin/*
