MAIN: mssh.o mssh.so
mssh.so: mssh.o
	    ${CC} -shared -o mssh.so main.o
mssh.o: main.c
	    ${CC} -c -fPIC main.c
clean:
	    rm -f *.o ; rm -f *.so 
