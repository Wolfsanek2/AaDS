// 5_3. Закраска прямой 1.
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка
// (Li и Ri). Найти длину окрашенной части числовой прямой.

#include <iostream>
#include <sstream>
#include <cassert>

struct Line {
    Line() = default;
    Line(int L, int R)
    : L(L), R(R) {}
    void operator << (std::istream &input) {
        int val{};
        input >> val;
        this->L = val;
        input >> val;
        this->R = val;
    }
    int L{};
    int R{};
};

class Line_comparator_left {
public:
    Line_comparator_left() = default;
    bool operator () (const Line &l1, const Line &l2) {
        // if (l1.L == l2.L) return l1.R < l2.R;
        return l1.L < l2.L;
    }
};

class Line_comparator_right {
public:
    Line_comparator_right() = default;
    bool operator () (const Line &l1, const Line &l2) {
        // if (l1.R == l2.R) return l1.R < l2.R;
        return l1.R < l2.R;
    }
};

template <typename T>
class Array {
public:
    Array() = default;
    Array(T *ptr, int length)
    : length(length) {
        assert(length >= 0);
        array = new T[length];
        for (int i{}; i < length; i++, capacity++) {
            array[i] = ptr[i];
        }
    }
    ~Array() {
        delete[] array;
    }
    Array(const Array &other) 
    : capacity(other.capacity), length(other.length) {
        array = new T[capacity];
        for (int i{}; i < length; i++) array[i] = other.array[i];
    }
    void operator = (const Array &other) {
        Array *new_array = new Array(other.array, other.length);
        this->array = new_array->array;
        this->capacity = new_array->capacity;
        this->length = new_array->length;
    }
    T &operator [] (int index) {
        return array[index];
    }
    void push(const T value) {
        if (length == capacity) expand();
        array[length++] = value;
    }
    T pop() {
        assert(length);
        if (length < 0.25 * capacity) reduce();
        return array[length--];
    }
    int get_length() {
        return length;
    }
    T *get_array() {
        return array;
    }
private:
    void reallocate() {
        T *new_array = new T[capacity];
        for (int i{}; i < length; i++) new_array[i] = array[i];
        delete [] array;
        array = new_array;
    }
    void expand() {
        if (capacity) {
            capacity *= 2;
        } else {
            capacity = 1;
        }
        reallocate();
    }
    void reduce() {
        capacity /= 2;
        reallocate();
    }
    T *array = nullptr;
    int capacity{};
    int length{};
};

template <typename T, typename Comparator = std::less<T>>
void merge_sort(T *array, int length, Comparator cmp = Comparator());
template <typename T, typename Comparator = std::less<T>>
void merge(T *array_1, T *array_2, int length_1, int length_2, Comparator cmp = Comparator());
void run(std::istream &input, std::ostream &output);
int do_logic(Array<Line> &array);
void merge_lines(Array<Line> &array);
void test_Line();
void test_merge_sort();
void test_merge_lines();
void test_run();
void test();

int main() {

    test();
    // run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int number{};
    input >> number;
    Array<Line> array;
    for (int i{}; i < number; i++) {
        Line line;
        line << input;
        array.push(line);
    }
    int result = do_logic(array);
    output << result << std::endl;
}

int do_logic(Array<Line> &array_left) {
    Array<Line> array_right = array_left;
    merge_sort(array_left.get_array(), array_left.get_length(), Line_comparator_left());
    merge_sort(array_right.get_array(), array_right.get_length(), Line_comparator_right());
    int current_point {array_left[0].L};
    int next_point {current_point};
    int index_left{}, index_right{};
    int count{}, value{};
    while (current_point < array_right[array_right.get_length() - 1].R) {
        if (array_left[index_left].L < array_right[index_right].R) {
            next_point = array_left[index_left].L;
            if (count) {
                value += next_point - current_point;
                current_point = next_point;
            }
            count++;
            index_left++;
        } else {
            next_point = array_right[index_right].R;
            index_right++;
            if (count) {
                value += next_point - current_point;
                current_point = next_point;
            }
            count--;
        }
    }
    return value;
}

void merge_lines(Array<Line> &array) {
    Array<Line> new_array;
    for (int i{}; i < array.get_length(); i++) {
        if (i == array.get_length() - 1) {
            new_array.push(array[array.get_length() - 1]);
            break;
        }
        if (array[i].R >= array[i + 1].L) {
            if (array[i].L < array[i + 1].R) {
                Line new_line(array[i].L, array[i + 1].R);
                new_array.push(new_line);
            } else {
                Line new_line(array[i].L, array[i].R);
                new_array.push(new_line);
            }
            i++;
        } else {
            Line new_line(array[i].L, array[i].R);
            new_array.push(new_line);
        }
    }
    array = new_array;
}

template <typename T, typename Comparator>
void merge_sort(T *array, int length, Comparator cmp) {
    if (length == 1) return;
    int first_length = length / 2;
    merge_sort(array, first_length, cmp);
    merge_sort(array + first_length, length - first_length, cmp);
    merge(array, array + first_length, first_length, length - first_length, cmp);
}

template <typename T, typename Comparator>
void merge(T *array_1, T *array_2, int length_1, int length_2, Comparator cmp) {
    T *new_array = new T[length_1 + length_2];
    int index_1{}, index_2{}, index{};
    while (index_1 < length_1 && index_2 < length_2) {
        if (cmp(array_1[index_1], array_2[index_2])) {
            new_array[index++] = array_1[index_1++];
        } else {
            new_array[index++] = array_2[index_2++];
        }
    }
    while (index_1 < length_1) new_array[index++] = array_1[index_1++];
    while (index_2 < length_2) new_array[index++] = array_2[index_2++];
    for (int i{}; i < length_1 + length_2; i++) {
        if (i < length_1) array_1[i] = new_array[i];
        else array_2[i - length_1] = new_array[i];
    }
    delete[] new_array;
}

void test_Line() {
    {
        Line line;
        std::stringstream input;
        input << "5 10" << std::endl;
        line << input;
        assert(line.L == 5);
        assert(line.R == 10);
    }
    std::cout << "Line test OK" << std::endl;
}

void test_merge_sort() {
    {
        int array[] = {2, 1};
        merge_sort(array, 2);
        assert(array[0] == 1);
        assert(array[1] == 2);
    }
    {
        int array[] = {3, 2, 1};
        merge_sort(array, 3);
        assert(array[0] == 1);
        assert(array[1] == 2);
        assert(array[2] == 3);
    }
    {
        int array[] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
        merge_sort(array, 9);
        assert(array[0] == 1);
        assert(array[1] == 2);
        assert(array[2] == 3);
        assert(array[3] == 4);
        assert(array[4] == 5);
        assert(array[5] == 6);
        assert(array[6] == 7);
        assert(array[7] == 8);
        assert(array[8] == 9);
    }
    {
        int array[] = {1, 9, 3, 7};
        merge_sort<int, std::greater<int>>(array, 4);
        assert(array[0] == 9);
        assert(array[1] == 7);
        assert(array[2] == 3);
        assert(array[3] == 1);
    }
    {
        int array[] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
        Array<int> din_array(array, 9);
        merge_sort(din_array.get_array(), 9);
        assert(din_array[0] == 1);
        assert(din_array[1] == 2);
        assert(din_array[2] == 3);
        assert(din_array[3] == 4);
        assert(din_array[4] == 5);
        assert(din_array[5] == 6);
        assert(din_array[6] == 7);
        assert(din_array[7] == 8);
        assert(din_array[8] == 9);
    }
    std::cout << "merge sort test OK" << std::endl;
}

void test_merge_lines() {
    {
        Array<Line> array;
        Line line_1(1, 2);
        Line line_2(2, 3);
        Line line_3(4, 5);
        array.push(line_1);
        array.push(line_2);
        array.push(line_3);
        merge_lines(array);
        assert(array[0].L == 1);
        assert(array[0].R == 3);
        assert(array[1].L == 4);
        assert(array[1].R == 5);
    }
    std::cout << "merge_lines test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "3\n";
        input << "1 4\n";
        input << "7 8\n";
        input << "2 5\n";
        run(input, output);
        assert(output.str() == "5\n");
    }
    {
        std::stringstream input, output;
        input << "1\n";
        input << "1 1\n";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input, output;
        input << "2\n";
        input << "1 3\n";
        input << "1 3\n";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input, output;
        input << "2\n";
        input << "1 5\n";
        input << "2 4\n";
        run(input, output);
        assert(output.str() == "4\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_Line();
    test_merge_sort();
    test_merge_lines();
    test_run();
}