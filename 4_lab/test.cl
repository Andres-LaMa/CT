class B {
    s : String <- "Hello";
    g (y:String) : String {
        y.concat(s)
    };
    f (x:Int) : Int {
        x+1
    };
};

class A inherits B {
    a : Int;
    b : B <- new B;
    f(x:Int) : Int {
        x+a
    };
};

class Main {
    main(): Object {
      let i : Int <- 1 in
        while i <= 5 loop {
          out_int(i);
          out_string("1");
          i <- i + 1;
        } pool
  };
};