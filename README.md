[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10771184&assignment_repo_type=AssignmentRepo)
# Assignment-4-Code

Tasks:
Part(1):
 we made changes to Binomial_queue.h
 - we add positions_table_ as private data member
 - add parent pointer
 - insert function
 - Find function
 - Remove Function: checks in x in position table, returns false if not there
 - Update pointer in Combine Trees

 Part(2): Changes in Quadratic Probing.h
 - add template perameter
 - add value_
 - modify insert function: turn it to bool
 - Find function that helps us find key assosiated with value 
 - ability to change value

 Search: 
    Searches for an item in our file using test_insert_and_search
    compiled by: g++ -g -std=c++14 -Wall -o test_insert_and_delete test_insert_and_delete.cc

    ran by: ./test_insert_and_delete 100_numbers.txt 100_delete_out.txt

Delete:
    Deletes from our file
    compiled by: g++ -g -std=c++14 -Wall -o test_insert_and_delete test_insert_and_delete.cc

    ran by: ./test_insert_and_delete 100_numbers.txt 100_delete_out.txt

Insert:
    We had to implement insertnomerge, modify our merge function and insert it. 
    compiled by: g++ -g -std=c++14 -Wall -o test_insert_and_delete test_insert_and_delete.cc
    ran by: ./test_new_insert 100_numbers.txt 100_test_search.txt

Everything worked fine