#include "A.hpp"

int S::get_int() const { return 2; }

int main() {
    A a;
    S s;
    a.get_S(s);
}