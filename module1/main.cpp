#include <iostream>
#include <sstream>
#include <cassert>

void run(std::istream &input, std::ostream &output);
void test_run();
void test();

int main() {
    
    test();
    // run(std::cin, std::cout);
    
    return 0;
}

void run(std::istream &input, std::ostream &output) {
    
}

void test_run() {
    {
        std::stringstream input, output;
        input << "3" std::endl;
        input << "4 1000000 7" << std::endl;
        run(input, output);
        
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_run();
}