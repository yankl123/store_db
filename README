
#project name:
    "store_db"
#defenition
    handl store debts 
#language
    c
####################################################
#architecture 
#  store.h :
#      Engaged in philosophy of store : data struct ,compair data and validate 
#      revealing "incondition" and "all_valid" functions to store.db (signature) 

        /\
        ||
#       store.c:
#       implementing all function to handl data

#         string_handl.c
# /\      string_handl.h 
  ||         /\
  ||_________||        
#       store_db.c:
#          "orgenise_db" function build the database reading from debts.csv  
#          "parse_query" function parse query string and returen "after_pars" struct        
        ||  
        \/ 

#       store_db.h:
#           revealing the database functions to main_local (signature)
        /\
        ||
#        \_main_local.c:
#           initial database using "orgenize_db" function
#           user interface
#           use "parse_query", "show_db" ,"show_select" ,"set_new" functions
#           use "show_db" , "show_select" functions
#           define the "print_buf" function 

#           you can compile the progrem as server and client 
        /\
        ||
#       main_server.c:
#           initial database using "orgenize_db" function
#           user network interface 
#           use "parse_query", "show_db" , "show_select" ,,"set_new" functions
#           define the "send_buf" function 

#        client.c :
#           the client aplication 
#       debts.CSV
#          the database file

#   how to use :
#       make main_local : compile local aplication option (without network)
#       make main_server : compile derver aplication option 
#       make run_local  : run local option
#       make run server : run server option 
#       make run client : run client progrem to simulate the progrem (after run derver..)
#       clean : clean all object file and run files 
#       see more options in makfile..