
%{
#ifdef TEST
#define DBG(x) (*yyout) << "[" << x << "]";
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

CONTROLFLOW case|switch|if|else|while|for|do|throw|return|continue|break|catch|try
DATATYPES const|char|int|long|short|double|float|void|[a-z]+_t
STRUCTURES public|protected|private|using|namespace|class|struct|enum|typename|typedef
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

%%
<INITIAL>^#include[^\n"]*\" { DBG(1); (*yyout) << "<span class='preprocessor'>" << yytext; BEGIN(LOCALINCLUDE); }
<INITIAL>^#                 { DBG(1.5); (*yyout) << "<span class='preprocessor'>" << yytext; BEGIN(PREPROCESSOR); }
<PREPROCESSOR>\n            { DBG(1.6); (*yyout) << "</span>\n"; BEGIN(INITIAL); }
<LOCALINCLUDE>[^"]+         { DBG(2); (*yyout) << "<a href='" << yytext << "'>" << yytext << "</a>"; }
<LOCALINCLUDE>\"            { DBG(3); (*yyout) << "\"</span>"; BEGIN(INITIAL); }
<INITIAL>\/\/[^\n]*	    { DBG(4); (*yyout) << "<span class='comment'>" << yytext << "</span>"; }
<INITIAL>\/\*               { DBG(5); (*yyout) << "<span class='comment'>" << yytext; BEGIN(COMMENT); }
<COMMENT>\\.                { DBG(6); ECHO; }
<COMMENT>\*\/               { DBG(7); (*yyout) << "*/</span>"; BEGIN(INITIAL); }
<INITIAL>\"	            { DBG(8); (*yyout) << "<span class='quote'>" << yytext; BEGIN(QUOTE); }
<QUOTE>\"	            { DBG(9); (*yyout) << "\"</span>"; BEGIN(INITIAL); }
<INITIAL>{CONTROLFLOW}      { DBG(10); (*yyout) << "<span class='controlflow'>" << yytext << "</span>"; }
<INITIAL>{DATATYPES}        { DBG(11); (*yyout) << "<span class='datatype'>" << yytext << "</span>"; }
<INITIAL>{STRUCTURES}       { DBG(12); (*yyout) << "<span class='structure'>" << yytext << "</span>"; }
<INITIAL>{IDENTIFIER}       { DBG(13); (*yyout) << "<span class='identifier'>" << yytext << "</span>"; }
\\.                         { DBG(14); ECHO; }
\n                          { DBG(15); ECHO; }
.                           { DBG(16); if (*yytext == '&') (*yyout) << "&amp;"; 
                              else if (*yytext == '<') (*yyout) << "&lt;";
                              else if (*yytext == '>') (*yyout) << "&gt;";
                              else (*yyout) << yytext ; }
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
