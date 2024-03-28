// 2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию. A[0..n-1] и
// B[0..m-1]. n >> m. Найдите их пересечение.

#include <iostream>
#include <sstream>
#include <cassert>

class Array {
public:
    Array() = default;
    Array(int *ptr, int length)
    : length(length) {
        assert(length >= 0);
        array = new int[length];
        for (int i{}; i < length; i++) {
            array[i] = ptr[i];
        }
    }
    ~Array() {
        delete[] array;
    }
    Array(const Array &other) 
    : capacity(other.capacity), length(other.length) {
        array = new int[capacity];
        for (int i{}; i < length; i++) array[i] = other.array[i];
    }
    Array& operator = (const Array &other) {
        Array *new_array = new Array(other.array, other.length);
        return *new_array;
    }
    void push(const int value) {
        if (length == capacity) reallocate();
        array[length++] = value;
    }
    int get_length() {
        return length;
    }
    int *get_array() {
        return array;
    }
private:
    void reallocate() {
        if (capacity) {
            capacity *= 2;
        } else {
            capacity = 1;
        }
        int *new_array = new int[capacity];
        for (int i{}; i < length; i++) new_array[i] = array[i];
        delete [] array;
        array = new_array;
    }
    int *array = nullptr;
    int capacity{};
    int length{};
};

void run(std::istream &input, std::ostream &output);
Array get_intersection(int *A, int length_A, int *B, int length_B);
int exponential_search(int *array, int length, int start, int key);
int binary_search(int *array, int start, int end, int key);
void test_run();
void test_Array();
void test_binary_search();
void test_exponential_search();
void test();

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int length_A{}, length_B{};
    input >> length_A >> length_B;
    int *A = new int[length_A];
    int *B = new int[length_B];

    for (int i{}; i < length_A; i++) {
        input >> A[i];
    }
    for (int i{}; i < length_B; i++) {
        input >> B[i];
    }

    Array C = get_intersection(A, length_A, B, length_B);

    for (int i{}; i < C.get_length(); i++) {
        output << C.get_array()[i] << " ";
    }
    output << std::endl;

    delete [] A;
    delete [] B;
}

Array get_intersection(int *A, int length_A, int *B, int length_B) {
    Array C;
    int current_index{};
    for (int i{}; i < length_B; i++) {
        int index = exponential_search(A, length_A, current_index, B[i]);
        if (index >= 0) {
            C.push(A[index]);
            current_index = index + 1;
            if (current_index == length_A) break;
        }
    }
    return C;
}

int exponential_search(int *array, int length, int start, int key) {
    if (start >= length) return -1;
    int left = start;
    int right = start ? std::min(start * 2, length - 1) : std::min(1, length - 1);
    while (right < length - 1 && array[right] < key) {
        left = right;
        right = std::min(right * 2, length - 1);
    }
    return binary_search(array, left, right, key);
}

int binary_search(int *array, int start, int end, int key) {
    if (start == end) {
        if (array[start] == key) {
            return start;
        } else {
            return -1;
        }
    }
    int middle = (start + end) / 2;
    if (array[middle] == key) {
        return middle;
    } else if (array[middle] < key) {
        return binary_search(array, middle + 1, end, key);
    } else {
        return binary_search(array, start, std::max(middle - 1, start), key);
    }
}

void test_run() {
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "3" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "1 3 5" << std::endl;
        run(input, output);
        assert(output.str() == "1 3 5 \n");
    }
    {
        std::stringstream input, output;
        input << "1" << std::endl;
        input << "1" << std::endl;
        input << "1" << std::endl;
        input << "2" << std::endl;
        run(input, output);
        assert(output.str() == "\n");
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "3" << std::endl;
        input << "1 3 5 7 9" << std::endl;
        input << "2 4 8" << std::endl;
        run(input, output);
        assert(output.str() == "\n");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "1 2 3" << std::endl;
        input << "1 2 3" << std::endl;
        run(input, output);
        assert(output.str() == "1 2 3 \n");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "0" << std::endl;
        input << "1 2 3" << std::endl;
        input << "" << std::endl;
        run(input, output);
        assert(output.str() == "\n");
    }
    {
        std::stringstream input, output;
        input << "0" << std::endl;
        input << "0" << std::endl;
        input << "" << std::endl;
        input << "" << std::endl;
        run(input, output);
        assert(output.str() == "\n");
    }
    {
        std::stringstream input, output;
        input << "10" << std::endl;
        input << "10" << std::endl;
        input << "1 2 3 4 5 6 7 8 9 10" << std::endl;
        input << "1 3 5 7 9 10 11 12 13 14" << std::endl;
        run(input, output);
        assert(output.str() == "1 3 5 7 9 10 \n");
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "7" << std::endl;
        input << "7 9 18 28 31" << std::endl;
        input << "8 18 24 29 33 35 38" << std::endl;
        run(input, output);
        assert(output.str() == "18 \n");
    }
    int N = 100;
    int n = 100;
    int m = 10;
    for (int i{}; i < N; i++) {
        std::stringstream input, output;
        int length_1 = rand() % n, length_2 = rand() % m;
        input << length_1 << std::endl;
        input << length_2 << std::endl;
        int value{};
        for (int j{}; j < length_1; j++) {
            value += rand() % 10 + 1;
            input << value << " ";
        }
        input << std::endl;
        value = 0;
        for (int j{}; j < length_2; j++) {
            value += rand() % 10 + 1;
            input << value << " ";
        }
        input << std::endl;
        std::cout << "run test number:" << i << ", input: " << std::endl;
        std::cout << input.str();
        run(input, output);
        std::cout << "output: " << std::endl << output.str() << std::endl;
    }
    std::cout << "run test OK" << std::endl;
}

void test_Array() {
    {
        int length = 10;
        int *ptr = new int[length];
        Array array(ptr, length);
        assert(ptr != array.get_array());
    }
    std::cout << "Array test OK" << std::endl;
}

void test_exponential_search() {
    {
        int array[] = {1, 2, 3, 4, 5};
        assert(exponential_search(array, 5, 0, 0) == -1);
        assert(exponential_search(array, 5, 0, 1) == 0);
        assert(exponential_search(array, 5, 0, 2) == 1);
        assert(exponential_search(array, 5, 0, 3) == 2);
        assert(exponential_search(array, 5, 0, 4) == 3);
        assert(exponential_search(array, 5, 0, 5) == 4);
        assert(exponential_search(array, 5, 4, 5) == 4);
        assert(exponential_search(array, 5, 5, 5) == -1);
        assert(exponential_search(array, 5, 0, 6) == -1);
    }
    {
        int array[] = {1};
        assert(exponential_search(array, 1, 0, 1) == 0);
        assert(exponential_search(array, 1, 0, 0) == -1);
    }
    {
        int array[] = {1, 3};
        assert(exponential_search(array, 2, 0, 0) == -1);
        assert(exponential_search(array, 2, 0, 1) == 0);
        assert(exponential_search(array, 2, 0, 2) == -1);
        assert(exponential_search(array, 2, 0, 3) == 1);
        assert(exponential_search(array, 2, 0, 4) == -1);
    }
    std::cout << "exponential search test OK" << std::endl;
}

void test_binary_search() {
    {
        int array[1] = {};
        assert(binary_search(array, 0, 0, -10) == -1);
        assert(binary_search(array, 0, 0, 0) == 0);
        assert(binary_search(array, 0, 0, 10) == -1);
    }
    {
        int array[] = {1};
        assert(binary_search(array, 0, 0, 0) == -1);
        assert(binary_search(array, 0, 0, 1) == 0);
        assert(binary_search(array, 0, 0, 2) == -1);
    }
    {
        int array[] = {1, 2, 3};
        assert(binary_search(array, 0, 2, 3) == 2);
    }
    std::cout << "binary search test OK" << std::endl;
}

void test() {
    test_Array();
    test_exponential_search();
    test_binary_search();
    test_run();
}