CXXFLAGS=-Wall -g3
LDFLAGS=-lpcap

.PHONY:	clean

ztop:	main.o counters.o protocol.o address.o filters.o cppcap.o
	$(CXX) $(LDFLAGS) -o $@ $(.ALLSRC)

main.o:	counters.h protocol.h address.h cppcap.h

counters.o:	counters.h address.h

protocol.o:	protocol.h counters.h

address.o:	address.h

filters.o:	filters.h address.h

cppcap.o:	cppcap.h

clean:
	rm -f *.o ztop ztop.core

strip:	ztop
	strip --strip-all ztop
