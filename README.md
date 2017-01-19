arachnid - web server studies
========

![](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c6/Spider.svg/60px-Spider.svg.png) 
It's *NOT* a usable web server.  Nginx is.

Arachnid is an HTTP 1.1 web server written in C++ with a syntax highlighter in Flex.  It's for me to 
mess around with C++ features, threads, files, networking, the HTTP protocol, and whatever I'm exploring at the moment. 
Feel free to study it and make comments about it.

### Installing on Unix

1) Build
Given a modern Unix with a C++14 compiler and flex, just:

    git clone [this-repository]
    cd arachnid
    make

2) Configure
Create a configuration file in this style


    yoursite.com 7777 / CSourceHandler
	  {
		    documentRoot some-source-code-directory
	  }


    yoursite.com 7777 / FileHandler
	  {
		  documentRoot EXAMPLE-DOCUMENT-ROOT
	  }
    
3) Run the server with the configuration file as an argument.


