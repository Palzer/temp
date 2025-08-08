# C++ Delegate System

This project provides a simple, header-only C++ delegate system. It allows you to create delegates that can be bound to free functions, static member functions, non-static member functions, and lambdas.

## Features

- **Header-only**: Just include `Delegate.h` in your project.
- **Type-safe**: The delegate is a template class that is specialized on the function signature.
- **Supports various callables**:
  - Free functions
  - Static member functions
  - Non-static member functions
  - Lambda functions
- **Move-only**: The delegate has move semantics, similar to `std::unique_ptr` and `std::function`.
- **Exception-safe**: Throws `std::runtime_error` if you try to invoke an unbound delegate.

## Usage

To use the delegate system, include the `Delegate.h` header file.

```cpp
#include "Delegate.h"
```

### Binding to a function

You can bind a delegate to a function using the `Bind` method or by using the constructor.

```cpp
// Free function
void MyFreeFunction(int);
Delegate<void(int)> d1(&MyFreeFunction);

// Member function
class MyClass {
public:
    void MyMemberFunction(int);
};
MyClass instance;
Delegate<void(int)> d2(&instance, &MyClass::MyMemberFunction);
```

### Invoking a delegate

You can invoke a delegate using the `Invoke` method or by using `operator()`.

```cpp
d1.Invoke(10);
d2(20);
```

### Checking if a delegate is bound

You can check if a delegate is bound to a function using `operator bool`.

```cpp
if (d1) {
    // Delegate is bound
}
```

## Compiling the Example

An example `main.cpp` is provided to demonstrate the usage of the delegate system. You can compile it using the provided `Makefile`.

```sh
make
```

This will create an executable named `delegate_example`. You can run it with:

```sh
./delegate_example
```
