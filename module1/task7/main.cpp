// 7_2. LSD для long long.
// Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6. Отсортировать
// массив методом поразрядной сортировки LSD по байтам.

#include <iostream>
#include <sstream>
#include <cassert>

void counting_sort(unsigned long long *array, int length);
void LSD_sort(unsigned long long *array, int length);

void run(std::istream &input, std::ostream &output);
void test_counting_sort();
void test_LSD_sort();
void test_run();
void test();

int main() {
    
    // test();
    run(std::cin, std::cout);
    
    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int length{};
    input >> length;
    unsigned long long *array = new unsigned long long[length];
    for (int i{}; i < length; i++) {
        input >> array[i];
    }
    LSD_sort(array, length);
    for (int i{}; i < length; i++) {
        unsigned long long value = array[i];
        output << array[i] << " ";
    }
    output << std::endl;
}

void counting_sort(unsigned long long *array, int length, int digit_number) {
    int mask = 255;
    int byte_size = 256;
    int *count_buffer = new int[byte_size]();
    for (int i{}; i < length; i++) {
        int digit = (array[i] >> (digit_number * 8)) & mask;
        count_buffer[digit]++;
    }
    for (int i{1}; i < byte_size; i++) {
        count_buffer[i] += count_buffer[i - 1];
    }
    unsigned long long *tmp_buffer = new unsigned long long[length];
    for (int i {length - 1}; i >= 0; i--) {
        int digit = (array[i] >> (digit_number * 8)) & mask;
        tmp_buffer[--count_buffer[digit]] = array[i];
    }
    for (int i{}; i < length; i++) {
        array[i] = tmp_buffer[i];
    }
    delete[] count_buffer;
    delete[] tmp_buffer;
}

void LSD_sort(unsigned long long *array, int length) {
    for (int i{}; i < sizeof(unsigned long long); i++) {
        counting_sort(array, length, i);
    }
}

void test_counting_sort() {
    {
        unsigned long long array[] = {9, 1, 8, 2, 7, 0, 3, 6, 4, 5};
        counting_sort(array, 10, 0);
        for (int i{}; i < 10; i++) {
            assert(array[i] == i);
        }
    }
    std::cout << "counting_sort test OK" << std::endl;
}

void test_LSD_sort() {
    {
        unsigned long long array[] = {9, 1, 8, 2, 7, 0, 3, 6, 4, 5};
        LSD_sort(array, 10);
        for (int i{}; i < 10; i++) {
            assert(array[i] == i);
        }        
    }
    {
        unsigned long long array[] = {22, 31, 21};
        LSD_sort(array, 3);
        assert(array[0] == 21);
        assert(array[1] == 22);
        assert(array[2] == 31);
    }
    {
        unsigned long long array[] = {4, 1000000, 7};
        LSD_sort(array, 3);
        assert(array[0] == 4);
        assert(array[1] == 7);
        assert(array[2] == 1000000);
    }
    std::cout << "LSD sort test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "4 1000000 7" << std::endl;
        run(input, output);
        assert(output.str() == "4 7 1000000 \n");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_counting_sort();
    test_LSD_sort();
    test_run();
}