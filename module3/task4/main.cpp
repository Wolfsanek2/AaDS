// Написать алгоритм для решения игры в “пятнашки”.

#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <set>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <algorithm>

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
        // int inv_count = get_inv_count();
        // int result = (get_inv_count() + empty_pos / 4);
        return (get_inv_count() + empty_pos / 4 + 1) % 2 == 0;
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
    bool operator==(const GameState &other) const {
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
    char get_move(const GameState &to) const {
        int difference = to.empty_pos - empty_pos;
        switch (difference) {
            case 1:
                return 'L';
            case -1:
                return 'R';
            case 4:
                return 'U';
            case -4:
                return 'D';
        }
        assert(false);
        return '0';
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

    friend class GameStateHasher;
    friend class PairComparator;
    friend int h(const GameState &state);
    friend std::string get_solution(const std::array<char, FieldSize> &field);
    friend std::ostream& operator << (std::ostream &stream, const GameState &state);
};

class GameStateHasher {
public:
    size_t operator()(const GameState &state) const {
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
    bool operator()(const Pair &p_1, const Pair &p_2) const {
        if (p_1.d < p_2.d) return true;
        else if (p_1.d > p_2.d) return false;
        else {
            for (int i = 0; i < FieldSize; i++) {
                if (p_1.game_state.field[i] != p_2.game_state.field[i]) {
                    return p_1.game_state.field[i] < p_2.game_state.field[i];
                }
            }
            return false;
        }
    }
};

std::string get_solution(const std::array<char, FieldSize> &field);
std::string get_solution_string(std::unordered_map<GameState, char, GameStateHasher> &visited);
std::ostream& operator << (std::ostream &stream, const GameState &state);
int h(const GameState &state);
void test_h();
void test_run();
void test();
void run(std::istream &input, std::ostream &output);

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    std::array<char, FieldSize> field;
    for (int i = 0; i < FieldSize; i++) {
        int value = 0;
        input >> value;
        field[i] = value;
    }
    std::string result = get_solution(field);
    if (result == "-1") {
        output << result << std::endl;
        return;
    }
    output << result.size() << std::endl;
    output << result << std::endl;
}

std::string get_solution(const std::array<char, FieldSize> &field) {
    GameState start_state(field);
    if (!start_state.is_solvable()) {
        return "-1";
    }
    std::unordered_map<GameState, int, GameStateHasher> d;
    std::unordered_map<GameState, char, GameStateHasher> visited;
    d[start_state] = 0;
    visited[start_state] = 'S';
    std::set<Pair, PairComparator> priority_queue;
    priority_queue.insert({start_state, d[start_state]});    
    while (!priority_queue.empty()) {
        Pair current_pair = *priority_queue.begin();
        GameState current_state = current_pair.game_state;
        // std::cout << priority_queue.size() << " " << d[current_state] << " " << h(current_state) << std::endl;
        // std::cout << current_state << std::endl;
        priority_queue.erase(current_pair);
        std::vector<GameState> next_states = current_state.get_next_states();
        for (GameState next_state: next_states) {
            if (!d.contains(next_state)) {
                visited[next_state] = current_state.get_move(next_state);
                d[next_state] = d[current_state] + 1;
                priority_queue.insert({next_state, h(next_state)});
            }
            else if (d[current_state] + 1 < d[next_state]) {
                priority_queue.erase({next_state, d[next_state]});
                d[next_state] = d[current_state] + 1;
                priority_queue.insert({next_state, h(next_state)});
                visited[next_state] = current_state.get_move(next_state);
            }
            if (next_state.is_complete()) {
                if (next_state.field != field) {
                    visited[next_state] = current_state.get_move(next_state);
                }
                return get_solution_string(visited);
            }
        }
    }
    return "-1";
}

std::string get_solution_string(std::unordered_map<GameState, char, GameStateHasher> &visited) {
    GameState state(FinishField);
    std::string result;
    while (visited[state] != 'S') {
        if (visited[state] == 'L') {
            state = state.move_right();
            result += 'L';
        } else if (visited[state] == 'R') {
            state = state.move_left();
            result += 'R';
        } else if (visited[state] == 'D') {
            state = state.move_up();
            result += 'D';
        } else if (visited[state] == 'U') {
            state = state.move_down();
            result += 'U';
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int h(const GameState &state) {
    int result = 0;
    for (int i = 0; i < FieldSize; i++) {
        if (i == state.empty_pos) continue;
        int value_1 = static_cast<int>(fabs(i - state.field[i] + 1)) % 4;
        int value_2 = static_cast<int>(fabs(i - state.field[i] + 1)) / 4;
        result += value_1 + value_2;
    }
    return result;
}

std::ostream& operator << (std::ostream &stream, const GameState &state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int value = state.field[4 * i + j];
            stream << value << " ";
        }
        stream << std::endl;
    }
    return stream;
}

void test_h() {
    {
        GameState state(FinishField);
        int value = h(state);
        assert(value == 0);
    }
    {
        std::array<char, FieldSize> field({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 15});
        GameState state(field);
        int value = h(state);
        assert(value == 1);
    }
    {
        std::array<char, FieldSize> field({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 12, 13, 14, 11, 15});
        GameState state(field);
        int value = h(state);
        assert(value == 2);
    }
    std::cout << "h() test OK" << std::endl;
}

void test_run() {
    int counter = 0;
    {
        std::stringstream input, output;
        input << "1 2 3 4\n";
        input << "5 6 7 8\n";
        input << "9 10 11 12\n";
        input << "13 15 14 0\n";
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "-1\n");
        counter++;
    }
    std::cout << "run test case " << counter << " OK" << std::endl;
    {
        std::stringstream input, output;
        input << "1 2 3 4\n";
        input << "5 6 7 8\n";
        input << "9 10 11 0\n";
        input << "13 14 15 12\n";
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "1\nU\n");
        counter++;
    }
    std::cout << "run test case " << counter << " OK" << std::endl;
    {
        std::stringstream input, output;
        input << "1 2 3 4\n";
        input << "5 6 7 8\n";
        input << "9 10 0 12\n";
        input << "13 14 11 15\n";
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "2\nUL\n");
        counter++;
    }
    std::cout << "run test case " << counter << " OK" << std::endl;
    {
        std::stringstream input, output;
        input << "1 0 2 3\n";
        input << "5 6 7 4\n";
        input << "9 10 11 8\n";
        input << "13 14 15 12\n";
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "5\nLLUUU\n");
        counter++;
    }
    std::cout << "run test case " << counter << " OK" << std::endl;
    {
        std::stringstream input, output;
        input << "14 2 4 8\n";
        input << "1 0 5 3\n";
        input << "9 7 6 10\n";
        input << "13 11 15 12\n";
        run(input, output);
        std::string output_string = output.str();
        counter++;
    }
    std::cout << "run test case " << counter << " OK" << std::endl;
    {
        std::stringstream input, output;
        input << "14 6 9 10\n";
        input << "12 0 11 4\n";
        input << "3 7 8 1\n";
        input << "15 13 5 2\n";
        run(input, output);
        std::string output_string = output.str();
        counter++;
    }
    std::cout << "run test case " << counter << " OK" << std::endl;
    std::cout << "run test OK" << std::endl;
}

void test() {
    // test_h();
    test_run();
}