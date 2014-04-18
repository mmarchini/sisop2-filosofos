all: semaphor 
	@echo "Everything compiled"

semaphor: semaphor.o
	@mkdir -p bin
	@gcc -pthread obj/semaphor.o -o bin/semaphor

semaphor.o: src/semaphor.c
	@mkdir -p obj
	@gcc -g -c src/semaphor.c -lphtread -L include -o obj/semaphor.o

clean:
	@rm -rf obj/* bin/*
