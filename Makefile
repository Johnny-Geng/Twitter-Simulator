CXX=g++
CXXFLAGS=-g -Wall -std=c++11

all: twitter

twitter: twitter.o datetime.o twiteng.o user.o util.o cmdhandler.o handler.o tweet.o 
	$(CXX) $(CXXFLAGS) twitter.o datetime.o twiteng.o user.o util.o cmdhandler.o handler.o tweet.o -o twitter

twitter.o: twitter.cpp twiteng.h cmdhandler.h util.h
	$(CXX) $(CXXFLAGS) -c twitter.cpp -o twitter.o

datetime.o: datetime.h datetime.cpp
	$(CXX) $(CXXFLAGS) -c datetime.cpp -o datetime.o

twiteng.o: twiteng.h twiteng.cpp user.h util.h datetime.h
	$(CXX) $(CXXFLAGS) -c twiteng.cpp -o twiteng.o

user.o: user.h user.cpp tweet.h
	$(CXX) $(CXXFLAGS) -c user.cpp -o user.o

util.o: util.h util.cpp tweet.h
	$(CXX) $(CXXFLAGS) -c util.cpp -o util.o

cmdhandler.o: cmdhandler.h cmdhandler.cpp util.h handler.h
	$(CXX) $(CXXFLAGS) -c cmdhandler.cpp -o cmdhandler.o

handler.o: handler.h handler.cpp twiteng.h
	$(CXX) $(CXXFLAGS) -c handler.cpp -o handler.o

tweet.o: tweet.h tweet.cpp datetime.h user.h
	$(CXX) $(CXXFLAGS) -c tweet.cpp -o tweet.o

clean:
	rm -f *.o twitter *.feed
