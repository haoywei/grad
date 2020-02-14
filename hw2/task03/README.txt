cat out.txt | ./time_management
perl -e 'print "\x70"x110; print "\x37\x13"' >out.txt
I used the perl to create the input and then use the file as input to execute the function.
I'm trying to overwrite the value of local_1a so even if don't pass the euid check, it can still pass the loacl_1a check.
Then I used gdb to find the address of local_1a