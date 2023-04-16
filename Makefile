CC = gcc
FLAGS = -Wall -g
LDFLAGS = 



all: cmp copy shell codecA codecB encode decode 

shell: stshell.o
	$(CC) $(FLAGS) stshell.o -o shell -lreadline

encode: encode.c
	$(CC) $(FLAGS) -o $@ $^ 

decode: decode.c
	$(CC) $(FLAGS) -o $@ $^  

codecA: codecA.c codecA.h
	$(CC) $(FLAGS) -shared -fPIC -o codecA codecA.c

codecB: codecB.c codecB.h
	$(CC) $(FLAGS) -shared -fPIC -o codecB codecB.c

copy: copy.o
	$(CC) $(FLAGS) copy.o -o copy

cmp: cmp.o
	$(CC) $(FLAGS) cmp.o -o cmp 

cmp.o: cmp.c
	$(CC) $(FLAGS) -c cmp.c

copy.o: copy.c
	$(CC) $(FLAGS) -c copy.c

stshell.o: stshell.c
	$(CC) $(FLAGS)  -c stshell.c



clean:
	rm -f *.o cmp copy shell new.txt codecA codecB libcodecA.so libcodecB.so encode decode *.txt
	




