#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <cassert>
#include "minimax.hpp"

class Value : public BaseValue<Value> {
  public:
    int a;
	static const Value VALUE_MIN, VALUE_MAX, VALUE_DRAW;
    
    Value() { this-> a = 0; }
	Value(int a) { this->a = a; }

	std::string to_string() const {
		return std::to_string(a);
	}
	std::weak_ordering operator<=>(const Value& v) const { return this->a <=> v.a; }
};

const Value Value::VALUE_MIN(INT_MIN);
const Value Value::VALUE_MAX(INT_MAX);
const Value Value::VALUE_DRAW(0);