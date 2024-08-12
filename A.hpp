#include <iostream>
#include <string>

class S {
  public:
    S() {}
    int get_int() const;
};

class A {
  public:
    A() {}
    void get_S(S s) {
        std::cout << s.get_int() << "\n";
    } 
};