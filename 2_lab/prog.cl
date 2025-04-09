-- COMENT
(*sdsds (*ffff *)
DDS

2*)

class Main inherits IO {
    main(): Object {
        let x: Int <- 42 in {
            out_int(x); -- Проверка целочисленной константы и вывода числа
            out_string("Hello, World!\n"); -- Проверка строковой константы и вывода строки
            if true then { -- Проверка условной конструкции
                out_string("True branch executed.\n");
            } else {
                out_string("False \
                 branch \
                executed.\n");
                out_string("False \
                 branch \
                executed.\n);
            }fi;
            while false loop { -- Проверка цикла while
                out_string("Loop body executed.\n");
            } pool;
            case x of -- Проверка оператора case
                i: Int => out_string("x is an integer.\n"),
                s: String => out_string("x is a string.\n")
            esac;
            new Self; -- Проверка создания нового объекта
            isvoid self; -- Проверка операции isvoid
            not true; -- Проверка отрицания
            let y: Bool <- true in { -- Проверка булевой константы
                if y then {
                    out_string("y is true.\n");
                } else {
                    out_string("y is false.\n");
                };
            };
            
        }
    };
};