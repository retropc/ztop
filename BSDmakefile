CXXFLAGS=-Wall -g3
LDFLAGS=-lpcap

.PHONY:	clean

ztop:	main.o counters.o protocol.o address.o filters.o cppcap.o display.o
	$(CXX) $(LDFLAGS) -o $@ $(.ALLSRC)

main.o:	address.h counters.h cppcap.h display.h protocol.h filters.h protocol.h address.h filters.h filters.zpp

address.o:	address.h

counters.o:	counters.h

counters.h:	address.h

cppcap.o:	cppcap.h

ccpcap.h:	address.h packetstruct.h

display.o:	display.h address.h

display.h:	counters.h

protocol.o:	protocol.h

filters.o:	filters.h

filters.h:	address.h

clean:
	rm -f *.o ztop ztop.core

strip:	ztop
	strip --strip-all ztop
