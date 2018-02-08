EXENAME = mp1
OBJS = mp1.o search.o solution.o test.o

CXX = clang++
CXXFLAGS = -std=c++1y -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -lpthread

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

mp1.o : mp1.cpp search.h solution.h
	$(CXX) $(CXXFLAGS) mp1.cpp

search.o : search.cpp search.h solution.h
	$(CXX) $(CXXFLAGS) search.cpp

solution.o : solution.cpp solution.h
	$(CXX) $(CXXFLAGS) solution.cpp

test.o : test.cpp test.h
	$(CXX) $(CXXFLAGS) test.cpp

clean :
	-rm -f *.o $(EXENAME)

tidy:
	-rm -f *.o
