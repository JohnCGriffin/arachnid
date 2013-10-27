
#include "process.hpp"

#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

namespace arachnid {

  Process::~Process()
  {
    close(wfd);
    close(rfd);
  }

  Process::Process(std::string program_path, const SSMap& environment)
  {
    pid_t child;
    int a[2],b[2];

    if(pipe(a) || pipe(b)){
      throw std::runtime_error("pipe failure in Process");
    }

    child = fork();

    if(child){

      close(a[0]);
      close(b[1]);

      this->rfd = b[0];
      this->wfd = a[1];

      // keep away the CGI zombies
      signal(SIGPIPE, SIG_IGN);
      signal(SIGCHLD, SIG_IGN);

    } else {

      // Thanks to execve, None of these allocations need to be released 
      char** env = new char* [ environment.size() + 1 ];
      char** pEnv = env;

      for(auto p : environment){
	*pEnv++ = strdup((p.first + "=" + p.second).c_str());
      }
      *pEnv = 0;

      char** argv = new char* [ 2 ];
      argv[0] = strdup(program_path.c_str());
      argv[1] = 0;

      // old stdin, stdout, stderr
      close(0);
      close(1);
      close(2);

      // new stdin, stdout
      dup2(a[0],0);
      dup2(b[1],1);

      close(a[1]);
      close(b[0]);

      execve(argv[0], argv, env);

      fprintf(stderr,"FAIL to execve\n");

      // if you are here it's because the exec failed, e.g. missing or invalid program
      exit(1);
    }
  }

}
