
#include <cstring>

const char* c_handler_css = R"_(

body { background : white }
.identifier { color : brown }
.quote,.datatype { color : forestgreen }
.keyword, .controlflow { font-weight : bold }

)_";

const int c_handler_css_len = strlen(c_handler_css);
