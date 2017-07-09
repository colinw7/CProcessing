all:
	cd src; qmake; make
	cd test/Schizzo2; qmake; make

clean:
	cd src; qmake; make clean
	rm -f src/Makefile
	cd test/Schizzo2; qmake; make clean
	rm -f test/Schizzo2/Makefile
	rm -f lib/libCProcessing.a
	rm -f bin/CQNurikabe
	rm -f test/Schizzo2/CProcessingSchizzo2
