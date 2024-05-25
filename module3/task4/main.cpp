// Написать алгоритм для решения игры в “пятнашки”.

#include <iostream>
#include <array>
#include <cassert>
#include <set>
#include <unordered_map>

const char FieldSize = 16;
const std::array<char, FieldSize> FinishField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class GameState {
public:
    GameState(const std::array<char, FieldSize> &field)
    : field(field) {
        empty_pos = -1;
        for (int i = 0; i < FieldSize; i++) {
            if (field[i] == 0) {
                empty_pos = i;
            }
        }
        assert(empty_pos != -1);
    }
    bool is_complete() const {
        return field == FinishField;
    }
    bool is_solvable() const {
        return (get_inv_count() + empty_pos / 4) % 2 == 0;
    }
    bool can_move_left() const {
        return empty_pos % 4 != 3;
    }
    bool can_move_right() const {
        return empty_pos % 4 != 0;
    }
    bool can_move_up() const {
        return empty_pos < 12;
    }
    bool can_move_down() const {
        return empty_pos > 3;
    }
    GameState move_left() const {
        assert(can_move_left());
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos + 1]);
        new_state.empty_pos++;
        return new_state;
    }
    GameState move_right() const {
        assert(can_move_right());
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos - 1]);
        new_state.empty_pos--;
        return new_state;
    }
    GameState move_up() const {
        assert(can_move_up());
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos + 4]);
        new_state.empty_pos += 4;
        return new_state;
    }
    GameState move_down() const {
        assert(can_move_down());
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos - 4]);
        new_state.empty_pos -= 4;
        return new_state;
    }
    bool operator==(const GameState &other) {
        return field == other.field;
    }
    std::vector<GameState> get_next_states() const {
        std::vector<GameState> next_states;
        if (can_move_left()) {
            next_states.push_back(move_left());
        }
        if (can_move_right()) {
            next_states.push_back(move_right());
        }
        if (can_move_down()) {
            next_states.push_back(move_down());
        }
        if (can_move_up()) {
            next_states.push_back(move_up());
        }
        return next_states;
    }
private:
    size_t get_inv_count() const {
        size_t inv_count = 0;
        for (int i = 0; i < FieldSize - 1; i++) {
            for (int j = i + 1; j < FieldSize; j++) {
                if (field[i] > field[j] && field[i] && field[j]) {
                    inv_count++;
                }
            }
        }
        return inv_count;
    }
    std::array<char, FieldSize> field;
    char empty_pos;

    friend GameStateHasher;
};

class GameStateHasher {
public:
    size_t operator()(const GameState &state) const {
        // size_t hash = 0;
        // std::memcpy(&hash, state.field.data(), sizeof(hash));
        // return hash;

        size_t hash = 0;
        for (int i = 1; i < FieldSize; i++) {
            hash += state.field[i] * i;
        }
        return hash;
    }
};

struct Pair {
    Pair(const GameState &state, const int d)
    : game_state(state), d(d) {}
    GameState game_state;
    int d;
};

class PairComparator {
public:
    bool operator()(const Pair &p_1, const Pair &p_2) {
        return p_1.d < p_2.d;
    }
};

std::string get_solution(const std::array<char, FieldSize> &field);
int h(const GameState &state);

int main() {
    return 0;
}

std::string get_solution(const std::array<char, FieldSize> &field) {
    GameState start_state(field);
    if (!start_state.is_solvable()) {
        return "-1";
    }
    std::unordered_map<GameState, int, GameStateHasher> d;
    std::unordered_map<GameState, GameState, GameStateHasher> pi;
    d[start_state] = 0;
    std::set<Pair, PairComparator> priority_queue;
    priority_queue.insert(Pair(start_state, d[start_state]));
    while (!priority_queue.empty()) {
        Pair current_pair = *priority_queue.begin();
        GameState current_state = current_pair.game_state;
        int current_d = current_pair.d;
        priority_queue.erase(current_pair);
        std::vector<GameState> next_states = current_state.get_next_states();
        for (GameState next_state: next_states) {
            if (!d.contains(next_state)) {
                d[next_state] = d[current_state] + 1;
                priority_queue.insert(Pair(next_state, d[next_state]));
            }
            else {

            }
        }
    }
}

int h(const GameState &state) {
    int result = 0;
    for (int i = 0; i < FieldSize; i++) {
        result += ();
    }
}