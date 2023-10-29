
# Project Name: store_db

## Description:
Handles store debts.

## Language:
C

## Architecture:

### store.h:
Engaged in the philosophy of store: data structure, data comparison, and validation. It reveals "incondition" and "all_valid" functions to store.db with their signatures.

### store.c:
Implements all functions to handle data.

### string_handl.c
### string_handl.h 
Handles string operations.

### store_db.c:
- `orgenise_db` function builds the database by reading from debts.csv, using "parse" and "set_new" functions.
- `parse_query` function parses the query string and returns a "Task" struct.

### store_db.h:
Reveals the database functions to main_local with their signatures.

### main_local.c:
Initializes the database (linked list) using the `orgenize_db` function, provides the user interface. Utilizes functions like `parse_query`, `show_db`, `show_select`, `set_new`, `show_db`, `show_select`. Defines the `print_buf` function.

### debts.CSV:
The database file.

## Flow:
1. [Main Program]
    - Initializes the database.
    - Takes user input.
2. [Query String] --> `parse_query` function
    - Returns ["Task" struct]
        - enum Q_TYPE [PRINT|SELECT|SET|ERROR|QUIT]
        - Union:
            - `select_props sp`
            - `client *new`
            - `char error_str[200]`
    - The program works according to what is returned in "after parse". If the program needs to return database data, it is passed as arguments to the target function and the buffer is passed as an argument to `show_db` or `show_select` function. If the buffer is full, the function executes the target function and continues.

## How to Use:
- `make main_local`: Compile the program.
- `make run`: Run the application.
- `make clean`: Clean all object files and run files.
- For more options, refer to the Makefile.
