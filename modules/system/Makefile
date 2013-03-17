MAIN: system.o system.so
system.so: system.o
	    ${CC} -shared -o system.so main.o
system.o: main.c
	    ${CC} -c -fPIC main.c
clean:
	    rm -f *.o ; rm -f *.so 
