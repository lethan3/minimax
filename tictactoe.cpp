#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include "minimax.hpp"

class Value {
  public:
    int a;
	Value(int a) { this->a = a; }
	static const Value VALUE_MIN(INT_MIN), VALUE_MAX(INT_MAX);
	bool operator<(const Value& v) const { return this->a < v.a; }
	bool operator==(const Value& v) const { return this->a == v.a; }
	bool operator<=(const Value& v) const { return *this < v || *this == v; }
};