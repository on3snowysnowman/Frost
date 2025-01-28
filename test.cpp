#include <iostream>

class MyClass {
public:
    void displayFunctionInfo(int n) {
        std::cout << "Function: " << __func__ << std::endl;
        std::cout << "Full Signature: " << __PRETTY_FUNCTION__ << std::endl;
    }
};

int main() {
    MyClass obj;
    obj.displayFunctionInfo(3);
    return 0;
}
