main_local: main_local.o store.o store_db.o string_handl.o
	gcc -g main_local.o store.o store_db.o string_handl.o -o main_local

main_local.o: main_local.c
	gcc -Wall -g -c main_local.c

store.o: store.c
	gcc -Wall -g -c store.c

store_db.o: store_db.c
	gcc -Wall -g -c store_db.c

string_handl.o : string_handl.c
	gcc -Wall -g -c string_handl.c	

clean:
	rm -f *.o main_local
run:
	./main_local debts.CSV

debug:
	gdb --args ./main_local debts.CSV