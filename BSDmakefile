CXXFLAGS=-Wall -g3
LDFLAGS=-lpcap
LDLIBS=

DEFAULT:	ztop

.PHONY:	clean

.SUFFIXES:	.cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $(.IMPSRC)

ztop:	main.o counters.o protocol.o address.o filters.o cppcap.o display.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(.ALLSRC) -o $(.TARGET)

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
