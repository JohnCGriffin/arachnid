
#ifndef _TAG_HPP_1365179367_
#define _TAG_HPP_1365179367_

#include <string>
#include <iostream>

namespace arachnid {

struct Tag {
    std::ostream& os;
    std::string name;
    Tag(std::ostream& os,std::string name);
    ~Tag();
};

}

/* http://stackoverflow.com/questions/1132751/how-can-i-generate-unique-values-in-the-c-preprocessor */

#define MERGE_(a,b)  a##b
#define LABEL_(a) MERGE_(unique_name_, a)
#define UNIQUE_NAME LABEL_(__LINE__)

#define TAG(OS,NAME) Tag UNIQUE_NAME (OS,NAME);

#endif


