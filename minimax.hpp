#pragma once
#include <iostream>
#include <concepts>
#include <vector>
#include <map>

template<class Value>
class BaseValue {
  public:
  	static const Value VALUE_MIN, VALUE_MAX, VALUE_DRAW;
	std::string to_string() const;
	std::weak_ordering operator<=>(const Value& v) const;	
};

template<class HValue>
class BaseHValue { // heuristic value
  public:
  	std::string to_string() const;
	std::weak_ordering operator<=>(const HValue& v) const;
};

template<class Move>
class BaseMove {
  public:
	static Move parse_move(std::string s); // single token
	std::string to_string() const;
};

template<template<class, class, class> class StateT, class Value, class HValue, class Move> requires (
	std::derived_from<Value, BaseValue<Value>>,
	std::derived_from<HValue, BaseHValue<HValue>>,
	std::derived_from<Move, BaseMove<Move>>
)
class BaseState {
  public:
	typedef StateT<Value, HValue, Move> State;
	bool player_to_move; // 1 if this is the maximizing player
	std::pair<bool, Value> get_terminal() const;
	HValue get_heuristic_value() const;
	std::vector<std::string> display() const;
	std::vector<std::pair<Move, State>> adj() const; // return all {move, state} pairs reachable in one turn
	std::strong_ordering operator<=>(const State& s) const; // comparator for maps
};

template<template<class, class, class> class StateT, class Value, class HValue, class Move> requires (
	std::derived_from<StateT<Value, HValue, Move>, BaseState<StateT, Value, HValue, Move>>
)
class Minimax {
  public:
	typedef StateT<Value, HValue, Move> State;
	std::map<State, Value> state_values;
	std::map<State, std::pair<Move, State>> best_move;
	std::map<State, bool> processed;

	Value minimax(State state, Value alpha, Value beta) {
		if (processed[state]) {
			return state_values[state];
		}
		processed[state] = true;

		std::pair<bool, Value> p_terminal = state.get_terminal();
		if (p_terminal.first) return state_values[state] = p_terminal.second;
		
		if (state.player_to_move) {
			Value value = Value::VALUE_MIN;
			for (std::pair<Move, State> p : state.adj()) {
				State ne = p.second;
			
				Value cval = minimax(ne, alpha, beta);
				if (cval > value) {
					value = cval;
					best_move[state] = p;
				}

				if (value >= beta) {
					processed[state] = false;
					break;
				}
				alpha = std::max(alpha, value);
			}
			return state_values[state] = value;
		} else {
			Value value = Value::VALUE_MAX;
			for (std::pair<Move, State> p : state.adj()) {
				State ne = p.second;
				
				Value cval = minimax(ne, alpha, beta);
				if (cval < value) {
					value = cval;
					best_move[state] = p;
				}

				if (value <= alpha) {
					processed[state] = false;
					break;
				}
				beta = std::min(beta, value);
			}
			return state_values[state] = value;
		}
	}
};