#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <cassert>
#include "minimax.hpp"

class HValue : public BaseHValue<HValue> {
  public:
    int a;
	
    HValue() { this-> a = 0; }
	HValue(int a) { this->a = a; }

	std::string to_string() const {
		return std::to_string(a);
	}
	std::weak_ordering operator<=>(const HValue& v) const { return this->a <=> v.a; }
};