CPPFLAGS = -g -std=c++11
CFLAGS = -g
LDCPPLIBS = -lstdc++

all: parse

parse: parse.c
	cc $(CFLAGS) -o $@ $@.c

fib: fib.c
	cc $(CFLAGS) -o $@ $@.c

aVeryBigSum: aVeryBigSum.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

bfs: bfs.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

insertionSort2: insertionSort2.c
	cc $(CFLAGS) -o $@ $@.c

insertionSort1: insertionSort1.c
	cc $(CFLAGS) -o $@ $@.c

is_colorful: is_colorful.c
	cc $(CFLAGS) -o $@ $@.c

two_stack_queue: two_stack_queue.c
	cc $(CFLAGS) -o $@ $@.c

has_cycle: has_cycle.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

insertNodeAtPosition: insertNodeAtPosition.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

isBalanced: isBalanced.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

designerPdfViewer: designerPdfViewer.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

icecreamParlor: icecreamParlor.c
	cc $(CFLAGS) -o $@ $@.c

checkMagazine: checkMagazine.c
	cc $(CFLAGS) -o $@ $@.c

rotLeft: rotLeft.cpp
	$(CC) $(CPPFLAGS) -o $@ $? $(LDCPPLIBS)

repeatedString: repeatedString.c
	cc $(CFLAGS) -o $@ $@.c
