main: main.o store.o store_db.o
	gcc -g main.o store.o store_db.o -o main

main.o: main.c
	gcc -Wall -g -c main.c

store.o: store.c
	gcc -Wall -g -c store.c

store_db.o: store_db.c
	gcc -Wall -g -c store_db.c

clean:
	rm -f *.o main

run:
	./main debts.CSV

debug:
	gdb --args ./main debts.CSV