
%{
#ifdef TEST
#define DBG(x) (yyout) << "[" << x << "]";
#else
#define DBG(x)
#endif
%}

%option default
%option c++
%option prefix="CPP"

%s QUOTE
%s COMMENT
%s LOCALINCLUDE
%s PREPROCESSOR

CONTROLFLOW auto|case|switch|if|else|while|for|do|throw|return|continue|break|catch|try
DATATYPES const|char|int|long|short|double|float|void|[a-z]+_t
STRUCTURES public|protected|private|using|namespace|class|struct|enum|typename|typedef
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

%%
<INITIAL>^#include[^\n"]*\" { DBG(1) ; (yyout) << "<span class='preprocessor'>" << yytext; BEGIN(LOCALINCLUDE); }
<INITIAL>^#                 { DBG(2) ; (yyout) << "<span class='preprocessor'>" << yytext; BEGIN(PREPROCESSOR); }
<PREPROCESSOR>\n            { DBG(3) ; (yyout) << "</span>\n"; BEGIN(INITIAL); }
<LOCALINCLUDE>[^"]+         { DBG(4) ; (yyout) << "<a href='" << yytext << "'>" << yytext << "</a>"; }
<LOCALINCLUDE>\"            { DBG(5) ; (yyout) << "\"</span>"; BEGIN(INITIAL); }
<INITIAL>\/\/[^\n]*	        { DBG(6) ; (yyout) << "<span class='comment'>" << yytext << "</span>"; }
<INITIAL>\/\*               { DBG(7) ; (yyout) << "<span class='comment'>" << yytext; BEGIN(COMMENT); }
<COMMENT>\\.                { DBG(8) ; (yyout) << yytext;; }
<COMMENT>\*\/               { DBG(9) ; (yyout) << "*/</span>"; BEGIN(INITIAL); }
<INITIAL>\"	                { DBG(10); (yyout) << "<span class='quote'>" << yytext; BEGIN(QUOTE); }
<QUOTE>\"	                { DBG(11); (yyout) << "\"</span>"; BEGIN(INITIAL); }
<INITIAL>{CONTROLFLOW}      { DBG(12); (yyout) << "<span class='controlflow'>" << yytext << "</span>"; }
<INITIAL>{DATATYPES}        { DBG(13); (yyout) << "<span class='datatype'>" << yytext << "</span>"; }
<INITIAL>{STRUCTURES}       { DBG(14); (yyout) << "<span class='keyword'>" << yytext << "</span>"; }
<INITIAL>{IDENTIFIER}       { DBG(15); (yyout) << "<span class='identifier'>" << yytext << "</span>"; }
\\.                         { DBG(16); (yyout) << yytext;; }
\n                          { DBG(17); (yyout) << yytext;; }
.                           { DBG(18); if (*yytext == '&') (yyout) << "&amp;"; 
                              else if (*yytext == '<') (yyout) << "&lt;";
                              else if (*yytext == '>') (yyout) << "&gt;";
                              else (yyout) << yytext ; }
%%

#include <iostream>
#include <string>

int CPPFlexLexer::yywrap() { return 1; }

void decorate_cpp_code (std::istream& is, std::ostream& os)
{
	CPPFlexLexer lexer(&is,&os);
	lexer.yylex();
}


#ifdef TEST

#include <iostream>
int main()
{
	decorate_cpp_code (std::cin, std::cout);
}

#endif
