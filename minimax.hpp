#pragma once
#include <iostream>
#include <concepts>
#include <vector>
#include <map>
#include <set>

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
	bool operator==(const Move& m) const;
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
	std::string to_string() const;
	std::vector<std::pair<Move, State>> adj() const; // return all {move, state} pairs reachable in one turn
	std::strong_ordering operator<=>(const State& s) const; // comparator for maps
	bool operator==(const State& s) const;
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
	std::set<State> cstack;

	Value minimax(const State& state, Value alpha, Value beta) {
		if (processed[state]) {
			return state_values[state];
		}
		processed[state] = true;

		if (cstack.count(state)) {
			// infinite loop, hopefully logic actually works
			return Value::VALUE_DRAW;
		}
		cstack.insert(state);

		std::pair<bool, Value> p_terminal = state.get_terminal();
		if (p_terminal.first) {
			state_values[state] = p_terminal.second;
		} else if (state.player_to_move) {
			Value value = Value::VALUE_MIN;
			bool pruned = false;

			std::vector<std::pair<Move, State>> nes = state.adj();
			std::sort(nes.begin(), nes.end(), [&](std::pair<Move, State> a, std::pair<Move, State> b) {return a.second.get_heuristic_value() > b.second.get_heuristic_value(); });

			for (std::pair<Move, State>& p : nes) {
				State ne = p.second;
			
				Value cval = minimax(ne, alpha, beta);
				if (cval > value) {
					value = cval;
					best_move[state] = p;
				}

				if (value >= beta) {
					// don't process remaining branches
					processed[state] = false;
					pruned = true;
					break;
				}
				alpha = std::max(alpha, value);
			}
			state_values[state] = value;
		} else {
			Value value = Value::VALUE_MAX;
			bool pruned = false;

			std::vector<std::pair<Move, State>> nes = state.adj();
			std::sort(nes.begin(), nes.end(), [&](std::pair<Move, State> a, std::pair<Move, State> b) {return a.second.get_heuristic_value() < b.second.get_heuristic_value(); });

			for (std::pair<Move, State>& p : nes) {
				State ne = p.second;

				Value cval = minimax(ne, alpha, beta);
				if (cval < value) {
					value = cval;
					best_move[state] = p;
				}

				if (value <= alpha) {
					// don't process remaining branches
					processed[state] = false;
					pruned = true;
					break;
				}
				beta = std::min(beta, value);
			}
			state_values[state] = value;
		}

		cstack.erase(state);
		return state_values[state];
	}

	void process(State state) {
		if (!processed[state]) minimax(state, Value::VALUE_MIN, Value::VALUE_MAX);
	}

	void display_state(int curr_ind, State state) {
		std::cout << "State " << curr_ind << "\n";
		if (!processed[state]) {
			process(state);
		}

		std::cout << state.to_string();

		std::cout << "Value: " << state_values[state].to_string() << "\n";
	}

	void run(State start) {
		std::vector<State> state_v{start};
		std::vector<int> par{-1};
		std::vector<std::vector<int>> adj{{}};

		int curr_ind = 0;
		while (true) {
			display_state(curr_ind, state_v[curr_ind]);

			std::cout << "> ";
			std::string cmd;
			std::cin >> cmd;
			std::cout << "\n";
			
			if (cmd == "optmove" || cmd == "move") {
				// command creates a move
				if (state_v[curr_ind].get_terminal().first) {
					std::cout << "Current state is terminal\n";
					continue;
				}
				Move move;
				State next_state;

				if (cmd == "optmove") {
					processed[state_v[curr_ind]] = false;
					process(state_v[curr_ind]);
					std::tie(move, next_state) = best_move[state_v[curr_ind]];
				} else if (cmd == "move") {
					std::string inp_move;
					std::cin >> inp_move;

					try {
						move = Move::parse_move(inp_move);
					} catch (std::exception e) {
						std::cout << "Invalid move syntax\n";
						continue;
					}

					bool found = false;
					for (std::pair<Move, State>& p : state_v[curr_ind].adj()) {
						if (p.first == move) {
							found = true;
							next_state = p.second;
							break;
						}
					}

					if (!found) {
						std::cout << "Move not valid from current state\n";
						continue;
					}
				}

				par.push_back(curr_ind);
				adj[curr_ind].push_back((int)state_v.size());
				adj.push_back({});

				curr_ind = (int)state_v.size();
				state_v.push_back(next_state);

				std::cout << move.to_string() << "\n";
			} else if (cmd == "nav") {
				int inp_ind;
				std::cin >> inp_ind;
				if (inp_ind >= (int)state_v.size()) {
					std::cout << "Not index of valid state\n";
					continue;
				}

				curr_ind = inp_ind;
			} else if (cmd == "list_optmoves") {
				std::cout << "Optimal moves:\n";
				process(state_v[curr_ind]);
				for (std::pair<Move, State>& p : state_v[curr_ind].adj()) {
					process(p.second);
					if (state_values[p.second] <= state_values[state_v[curr_ind]] 
						&& state_values[p.second] >= state_values[state_v[curr_ind]]) {
						std::cout << "* " << p.first.to_string() << "\n";
					}
				}
			} else if (cmd == "list_winmoves") {
				std::cout << "Winning moves:\n";
				process(state_v[curr_ind]);
				for (std::pair<Move, State>& p : state_v[curr_ind].adj()) {
					process(p.second);
					if (state_values[p.second] > Value::VALUE_DRAW && state_v[curr_ind].player_to_move ||
						state_values[p.second] < Value::VALUE_DRAW && !state_v[curr_ind].player_to_move) {
						std::cout << "* " << p.first.to_string() << "\n";
					}
				}
			} else if (cmd == "list_drawmoves") {
				std::cout << "Drawing moves:\n";
				process(state_v[curr_ind]);
				for (std::pair<Move, State>& p : state_v[curr_ind].adj()) {
					process(p.second);
					if (state_values[p.second] <= Value::VALUE_DRAW
						&& state_values[p.second] >= Value::VALUE_DRAW) {
						std::cout << "* " << p.first.to_string() << "\n";
					}
				}
			} else if (cmd == "list_losemoves") {
				std::cout << "Losing moves:\n";
				process(state_v[curr_ind]);
				for (std::pair<Move, State>& p : state_v[curr_ind].adj()) {
					process(p.second);
					if (state_values[p.second] > Value::VALUE_DRAW && !state_v[curr_ind].player_to_move ||
						state_values[p.second] < Value::VALUE_DRAW && state_v[curr_ind].player_to_move) {
						std::cout << "* " << p.first.to_string() << "\n";
					}
				}
			} else {
				std::cout << "\"" << cmd << "\" is not a command\n";
			}

			std::cout << "\n";
		}
	}
};