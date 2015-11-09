CXXFLAGS := -Wall -Wextra -ansi -pedantic -std=c++14

DEPS := $(wildcard *.h)

PROG = Trees
OBJS = main.o read_write_lock.o dynamic_locker.o 

all: debug

release: CXXFLAGS += -O3
release: $(PROG)

debug: CXXFLAGS += -g
debug: $(PROG)

clean:
	rm -f *~ *.out *.o Trees

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

test%.o: test%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROG) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
