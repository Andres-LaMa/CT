-- Тестовая программа для лексера языка Cool

-- Содержит все лексические элементы Cool:

-- 1. Ключевые слова:
class -- CLASS
inherits -- INHERITS
if -- IF
then -- THEN
else -- ELSE
fi -- FI
while -- WHILE
loop -- LOOP
let -- LET
in -- IN
case -- CASE
of -- OF
esac -- ESAC
new -- NEW
isvoid -- ISVOID
not -- NOT

-- 2. Типы:
Int -- INT_CONST
Bool -- BOOL_CONST
String -- STRING_CONST
Object -- OBJECTID
IO -- OBJECTID

-- 3. Идентификаторы:
myVariable -- OBJECTID
MY_CONSTANT -- TYPEID
aVeryLongVariableName -- OBJECTID
AVeryLongClassName -- TYPEID

-- 4. Целые числа:
0 -- INT_CONST
123 -- INT_CONST
99999 -- INT_CONST
-42 -- INT_CONST

-- 5. Строки:
"Hello, world!" -- STRING_CONST
"" -- STRING_CONST
--"Unterminated string" -- STRING_CONST
--"String with null \0 character" -- STRING_CONST

-- 6. Булевы значения:
true -- BOOL_CONST
false -- BOOL_CONST

-- 7. Операторы:
+ -- PLUS
- -- MINUS
* -- MULT
/ -- DIV
= -- EQ
< -- LT
<= -- LE
@ -- AT
. -- DOT
~ -- TILDE
<- -- ASSIGN
=> -- DARROW

-- 8. Скобки и точки с запятой:
( -- LPAREN
) -- RPAREN
{ -- LBRACE
} -- RBRACE
: -- COLON
; -- SEMI

-- 9. Комментарии:
-- Однострочный комментарий

(*
Многострочный
комментарий.
Можно содержать все что угодно, даже:
  class (* Комментарии внутри комментариев не поддерживаются *) Object
*)

class Main {
  io : IO <- new IO;
  main() : Object {
    {
      io.out_string("Проверка всех лексем языка Cool.\n");
      io.out_int(1 + 2 * 3 / 4 - 5);
      io.out_string(if true then "истина" else "ложь" fi);
      io.out_string("\n");
    }
  };
};