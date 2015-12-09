CXXFLAGS := -Wall -Werror -Wextra -ansi -pedantic -std=c++14 -pthread

DEPS := $(wildcard *.h)

PROG = Trees
OBJS = main.o tree.o node.o inner_node.o leaf_node.o helper.o

all: release

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
