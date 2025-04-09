%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()

int comment_level = 0;
int count_str = 0;
int count_slash = 0;
%}

%option warn batch noyywrap c++
%option yylineno
%option yyclass="CoolLexer"

%x COMMENT
%x STRING

WHITESPACE				[ \f\r\t\v]
ONELINE_COMMENT			"--"(.*)
DARROW                  =>
ASSIGN                  <-
LE                      <=
STRING      			\"([^"\n]|\\[^\n]|\\\n)*\"
BADSTRING               \"([^"\n]|\\[^\n]|\\\n)*
INTEGER                 [0-9]+
TYPE_ID                 [A-Z][a-zA-Z0-9_]*
OBJECT_ID               [a-z][a-zA-Z0-9_]*
SYMBOLS                 [\(\)\{\}\=\<\~\@\/\*\-\+\:\;\.\,]

%%

"(*"                   { BEGIN(COMMENT); comment_level++; }
<COMMENT>"(*"          { comment_level++;}
<COMMENT>"*)"          {
                            if (comment_level > 0) {
                                comment_level--;
                                if (comment_level == 0) {
                                    BEGIN(INITIAL);
                                }
                            } else {
                                Error("Unmatched *)");
                                return -1;
                            }
                        }
<COMMENT>.              {}
<COMMENT>\n             {lineno++;}
<COMMENT><<EOF>>        {Error("EOF in comment"); return -1; }




{STRING}				{lineno++; return TOKEN_KW_STR_CONST;};
{BADSTRING}             Error("Unterminated string");
{WHITESPACE}			{}
{ONELINE_COMMENT}		;
class 					return TOKEN_KW_CLASS;
else					return TOKEN_KW_ELSE;
fi						return TOKEN_KW_FI;
if						return TOKEN_KW_IF;
in						return TOKEN_KW_IN;
inherits				return TOKEN_KW_INHERITS;
isvoid					return TOKEN_KW_ISVOID;
let						return TOKEN_KW_LET;
loop					return TOKEN_KW_LOOP;
pool 					return TOKEN_KW_POOL;
then					return TOKEN_KW_THEN;
while					return TOKEN_KW_WHILE;
case					return TOKEN_KW_CASE;
esac 					return TOKEN_KW_ESAC;
new 					return TOKEN_KW_NEW;
of						return TOKEN_KW_OF;
not						return TOKEN_KW_NOT;
self                    return TOKEN_KW_SELF;

{DARROW}				return TOKEN_DORROW;
{ASSIGN}				return TOKEN_ASSIGN;
{LE}					return TOKEN_LE;

false 					return TOKEN_KW_FALSE;
true					return TOKEN_KW_TRUE;
{INTEGER}				return TOKEN_KW_INTEGER;
{TYPE_ID}				return TOKEN_KW_TYPEID;
{OBJECT_ID}				return TOKEN_KW_OBJECTID;
{SYMBOLS}				return yytext[0];

\n                      {lineno++;}

%%
void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}