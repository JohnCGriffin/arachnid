
CXX=g++
CXXBASEFLAGS=--std=c++14
CXXWARNINGS=-Wall
CXXFLAGS=$(CXXBASEFLAGS) $(CXXWARNINGS)
LIBS=-lpthread

OBJS=basic_realm.o benchmark_handler.o c_decorator.o cgi_handler.o c_handler.o \
	configuration.o file_handler.o file_utils.o get_handler.o handler_factory.o handler.o \
	http_server.o http_structs.o http_utils.o main.o mime.o process.o \
	scgi_handler.o str_utils.o tag.o tcp_conn.o uri_match.o utilities.o

arachnid_server: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

c_decorator.o: c_decorator.l
	flex -t *.l > c_decorator.cpp && $(CXX) $(CXXBASECPPFLAGS) -c c_decorator.cpp

clean:
	rm -f $(OBJS) c_decorator.o c_decorator.cpp arachnid_server TAGS

etags:
	etags *.cpp *.hpp
