CXX=g++
CPPFLAGS=-O3

all: rmdopfq
	

rmdopfq: rmdopfq.cpp
	$(CXX) $(CPPFLAGS) $? -o $@

check: rmdopfq.cpp
	mkdir -p out
	$(CXX) -coverage -O0 $? -o check
	./check || true
	./check data/dup.fq out/out.fq
	diff data/ans.fq out/out.fq

coverage: check
	gcov rmdopfq.cpp

test: check
	

clean:
	rm -f rmdopfq check
	rm -f *.exe *.gcov *.gcno *.gcda
	rm -rf out

