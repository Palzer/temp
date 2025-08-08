#include <iostream>
#include "Delegate.h"

// Example free function
void FreeFunction(int x) {
    std::cout << "FreeFunction called with: " << x << std::endl;
}

// Example class with member functions
class MyClass {
public:
    void MemberFunction(int x) {
        std::cout << "MyClass::MemberFunction called with: " << x << std::endl;
    }

    static void StaticMemberFunction(int x) {
        std::cout << "MyClass::StaticMemberFunction called with: " << x << std::endl;
    }
};

int main() {
    // 1. Delegate for a free function
    Delegate<void(int)> freeFuncDelegate;
    freeFuncDelegate.Bind(&FreeFunction);
    std::cout << "Invoking free function delegate:" << std::endl;
    freeFuncDelegate(10);

    // 2. Delegate for a static member function
    Delegate<void(int)> staticMemberFuncDelegate;
    staticMemberFuncDelegate.Bind(&MyClass::StaticMemberFunction);
    std::cout << "\nInvoking static member function delegate:" << std::endl;
    staticMemberFuncDelegate(20);

    // 3. Delegate for a non-static member function
    MyClass instance;
    Delegate<void(int)> memberFuncDelegate;
    memberFuncDelegate.Bind(&instance, &MyClass::MemberFunction);
    std::cout << "\nInvoking member function delegate:" << std::endl;
    memberFuncDelegate(30);

    // 4. Delegate for a lambda function
    int captured_var = 40;
    Delegate<void(int)> lambdaDelegate([=](int x) {
        std::cout << "Lambda function called with: " << x << " and captured value: " << captured_var << std::endl;
    });
    std::cout << "\nInvoking lambda delegate:" << std::endl;
    lambdaDelegate(40);

    // 5. Unbinding and checking state
    std::cout << "\nUnbinding member function delegate." << std::endl;
    memberFuncDelegate.Unbind();
    if (memberFuncDelegate) {
        std::cout << "Delegate is still bound." << std::endl;
    } else {
        std::cout << "Delegate is unbound." << std::endl;
    }

    try {
        std::cout << "Attempting to invoke unbound delegate:" << std::endl;
        memberFuncDelegate(50);
    } catch (const std::runtime_error& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // 6. Moving a delegate
    std::cout << "\nMoving a delegate." << std::endl;
    Delegate<void(int)> movedDelegate = std::move(freeFuncDelegate);
    if (freeFuncDelegate) {
        std::cout << "Original delegate is still bound after move." << std::endl;
    } else {
        std::cout << "Original delegate is unbound after move." << std::endl;
    }

    if (movedDelegate) {
        std::cout << "Moved delegate is bound." << std::endl;
        std::cout << "Invoking moved delegate:" << std::endl;
        movedDelegate(60);
    }

    return 0;
}
