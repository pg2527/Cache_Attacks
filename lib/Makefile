include ../Makefile.inc

P=lib/
all:
	${GCC} -c -fPIC mylib.c && echo -n "  OBJ $P" && echo "mylibc.o"
	${GCC} -shared mylib.o -o libxyz.so && echo -n "  LIB $P" && echo "libxyz.so"
	${GCC} mandl.c -o mandl.out -ldl ${ECHO}mandl.c
	${GCC} mandl1.c -o mandl1.out -ldl ${ECHO}mandl1.c
clean:
	${RM} *.so run
