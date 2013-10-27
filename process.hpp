
#ifndef _PROCESS_HPP_1364926169_
#define _PROCESS_HPP_1364926169_

#include <map>
#include <string>

#include "utilities.hpp"

/*********************************************************

Process forks another process attaching its stdout to
the read file descriptor (rfd) and with its stdin read
from the writing file descriptor (wfd).

#include <cstdio>

int main()
{
        int c;
        SSMap env;
        env["DOG"] = "fido";
        env["COW"] = "milk";

        Process process("/tmp/cgitest", env);
        FILE *fp = fdopen(process.rfd,"r");

        while((c = fgetc(fp)) != EOF){
                putchar(c);
        }

        fclose(fp);
}

**********************************************************/

namespace arachnid {

  struct Process {

    // read and write file descriptors
    int rfd;
    int wfd;

    // e.g. Process("/usr/bin/whatnot", map of "SERVER" -> "fred.com", "REMOTE_ADDR" -> "72.33.13.43",..
    Process( std::string program_path,  const SSMap& env  );

    virtual ~Process();
  };

}


#endif

