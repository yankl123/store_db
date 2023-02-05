main_local : main_local.o store.o store_db.o string_handl.o
	gcc -g main_local.o store.o store_db.o string_handl.o -o main_local

main_server: main_server.o store.o store_db.o string_handl.o 
	gcc -g -pthread main_server.o store.o store_db.o string_handl.o -o main_server

client : client.o
	gcc -g client.o -o client

main_local.o: main_local.c
	gcc -Wall -g -c main_local.c

main_server.o :main_server.c 
	gcc -Wall -g -c main_server.c

client.o : client.c
	gcc -Wall -g -c client.c

store.o: store.c
	gcc -Wall -g -c store.c

store_db.o: store_db.c
	gcc -Wall -g -c store_db.c

string_handl.o : string_handl.c
	gcc -Wall -g -c string_handl.c	

clean:
	rm -f *.o main_local main_server client

run_local:
	./main_local debts.CSV

run_server:
	./main_server debts.CSV	12345

run_client:
	./client 127.0.0.1 12345

debug_local:

	gdb --args ./main_local debts.CSV

debug_server:
	gdb --args ./main_server debts.CSV 12345

debug_client :
	gdb --args ./client 127.0.0.1 12345