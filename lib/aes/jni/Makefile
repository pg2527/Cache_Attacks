all: spy
clean:
	rm -f *.o spy
spy: spy.cpp ../../cacheutils.h
#	g++-4.8 -g -std=gnu++11 -O2 -o  $@ $< -I /usr/local/openssl/include/openssl -L /usr/lib -lcrypto
	g++-4.8 -g -std=gnu++11 -O2 -o  $@ $< -I /usr/local/openssl/include/openssl -L /usr/local/openssl/lib -lcrypto
#	g++-4.8 -std=gnu++11 -O2 -o $@ $< 
