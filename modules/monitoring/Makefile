MAIN: monitoring.o monitoring.so
monitoring.so: monitoring.o
	    ${CC} -shared -o monitoring.so main.o
monitoring.o: main.c
	    ${CC} -c -fPIC main.c
clean:
	    rm -f *.o ; rm -f *.so 
