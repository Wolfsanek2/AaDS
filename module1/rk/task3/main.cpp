#include <iostream>

struct Crate {
    Crate() = default;
    Crate(int x, int y, int z, int index)
    : x(x), y(y), z(z), index(index) {}
    int x{}, y{}, z{}, index{};
};

template <typename T, typename Comparator = std::less<T>>
void merge(T *array_1, T *array_2, int length_1, int length_2, Comparator cmp = Comparator()) {
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

template <typename T, typename Comparator = std::less<T>>
void merge_sort(T *array, int length, Comparator cmp = Comparator()) {
    if (length == 1) return;
    int first_length = length / 2;
    merge_sort(array, first_length, cmp);
    merge_sort(array + first_length, length - first_length, cmp);
    merge(array, array + first_length, first_length, length - first_length, cmp);
}

class Crate_comparator {
public:
    Crate_comparator() = default;
    bool operator () (const Crate &c1, const Crate &c2) {
        int array_1[3] = {c1.x, c1.y, c1.z};
        int array_2[3] = {c2.x, c2.y, c2.z};
        merge_sort(array_1, 3);
        merge_sort(array_2, 3);
        return (array_1[0] < array_2[0]) && (array_1[1] < array_2[1]) && (array_1[2], array_2[2]);
    }
};

int main() {
    int length{};
    std::cin >> length;
    Crate *crates = new Crate[length];
    for (int i{}; i < length; i++) {
        std::cin >> crates[i].x;
        std::cin >> crates[i].y;
        std::cin >> crates[i].z;
        crates[i].index = i;
    }

    merge_sort(crates, length, Crate_comparator());

    for (int i{}; i < length; i++) {
        std::cout << crates[i].index << " ";
    }
    std::cout << std::endl;

    delete[] crates;
    return 0;
}