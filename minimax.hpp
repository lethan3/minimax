#include <iostream>
#include <concepts>
#include <vector>
#include <map>

class Base_Value {
  public:
	Base_Value();
	bool operator<=>(const Base_Value& v) const;
};

class Base_HValue { // heuristic value
  public:
	Base_HValue();
	bool operator<(const Base_HValue& v) const;
	bool operator==(const Base_HValue& v) const;
	bool operator<=(const Base_HValue& v) const { return *this < v || *this == v; }
};

class Base_Move {
  public:
	Base_Move();
};

template<class Value, class HValue, class Move> requires (
	std::derived_from<Value, Base_Value>,
	std::derived_from<HValue, Base_HValue>,
	std::derived_from<Move, Base_Move>
)
class Base_State {
  public:
	Base_State();
	bool turn; // player to move
	bool is_terminal() const;
	Value get_terminal_value() const;
	HValue get_heuristic_value() const;
	std::vector<Base_State> adj(); // return all states reachable in one turn
	bool operator<=>(const Base_State& s) const; // comparator for maps
};

template<template<class, class, class> class StateT, class Value, class HValue, class Move> requires (
	std::derived_from<StateT<Value, HValue, Move>, Base_State<Value, HValue, Move>>
)
class Minimax {
  public:
	typedef StateT<Value, HValue, Move> State;
	std::map<State, Value> state_values;
	std::map<State, State> best_move;
	std::map<State, bool> processed;

	Value minimax(State state, Value alpha, Value beta, bool maximizing_player) {
		if (processed[state]) {
			return state_values[state];
		}
		if (state.is_terminal()) {
			return state.get_terminal_value();
		}
		if (maximizing_player) {
			Value value = Value::VALUE_MIN;
			for (State ne : state.adj()) {
				value = std::max(value, minimax(ne, alpha, beta, false));
				if (beta <= value) {
					break;
				}
				alpha = std::max(alpha, value);
			}
			return value;
		} else {
			Value value = Value::VALUE_MAX;
			for (State ne : state.adj()) {
				value = std::max(value, minimax(ne, alpha, beta, false));
				if (value <= alpha) {
					break;
				}
				beta = std::min(beta, value);
			}
			return value;
		}
	}
};