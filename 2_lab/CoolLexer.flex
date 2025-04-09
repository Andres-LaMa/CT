/* Flex файл для лексического анализа языка программирования Cool */
%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()

%}
/* Регулярные выражения для чисел, строковых констант и комментариев */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
WHITESPACE  [\t\n\r ]
ID          {LETTER}({LETTER}|{DIGIT})*
NUM         {DIGIT}+
STRING      \"([^\\\"]|\\.)*\"
COMMENT     "--".*\n


%option warn nodefault batch noyywrap
%option yylineno
%option yyclass="CoolLexer"

%%

class             return TOKEN_KW_CLASS;
else              return TOKEN_KW_ELSE;
false             return TOKEN_KW_FALSE;
fi                return TOKEN_KW_FI;
if                return TOKEN_KW_IF;
in                return TOKEN_KW_IN;
inherits          return TOKEN_KW_INHERITS;
isvoid            return TOKEN_KW_ISVOID;
let               return TOKEN_KW_LET;
loop              return TOKEN_KW_LOOP;
pool              return TOKEN_KW_POOL;
then              return TOKEN_KW_THEN;
while             return TOKEN_KW_WHILE;
case              return TOKEN_KW_CASE;
esac              return TOKEN_KW_ESAC;
new               return TOKEN_KW_NEW;
of                return TOKEN_KW_OF;
not               return TOKEN_KW_NOT;
true              return TOKEN_KW_TRUE;

IO                return TOKEN_KW_NAME_INHERITS;
Object            return TOKEN_KW_TYPE_OBJ;

Int                 return TOKEN_KW_TYPE_INT;
String              return TOKEN_KW_TYPE_STRING;
Bool                return TOKEN_KW_TYPE_BOOL;


[*/+\-,^.;:()\[\]]   return yytext[0];

{ID}              return TOKEN_KW_IDENTIFIER;
{NUM}             return TOKEN_KW_INTEGER_CONST;
{STRING}          return TOKEN_KW_STR_CONST;
{COMMENT}         ; /* Игнорируем комментарии */
{WHITESPACE}+     ; /* Игнорируем пробельные символы */

.                 return yytext[0]; /* Все остальные символы возвращаем как есть */

%%
