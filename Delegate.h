#pragma once

#include <utility>
#include <memory>
#include <stdexcept>

template <typename T>
class Delegate;

template <typename R, typename... Args>
class Delegate<R(Args...)> {
public:
    Delegate() = default;

    // Constructor for free functions and lambdas
    template <typename F>
    Delegate(F&& f) {
        Bind(std::forward<F>(f));
    }

    // Constructor for member functions
    template <typename C>
    Delegate(C* instance, R (C::*method)(Args...)) {
        Bind(instance, method);
    }

    // Bind to a free function or a lambda
    template <typename F>
    void Bind(F&& f) {
        callable_ = std::make_unique<FreeFunctionCallable<F>>(std::forward<F>(f));
    }

    // Bind to a member function
    template <typename C>
    void Bind(C* instance, R (C::*method)(Args...)) {
        callable_ = std::make_unique<MemberFunctionCallable<C>>(instance, method);
    }

    void Unbind() {
        callable_.reset();
    }

    R Invoke(Args... args) {
        if (callable_) {
            return callable_->Invoke(std::forward<Args>(args)...);
        }
        throw std::runtime_error("Delegate is not bound to a function.");
    }

    R operator()(Args... args) {
        return Invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const {
        return callable_ != nullptr;
    }

    // Make the delegate move-only
    Delegate(Delegate&& other) noexcept = default;
    Delegate& operator=(Delegate&& other) noexcept = default;
    Delegate(const Delegate&) = delete;
    Delegate& operator=(const Delegate&) = delete;

private:
    class ICallable {
    public:
        virtual ~ICallable() = default;
        virtual R Invoke(Args...) = 0;
    };

    template <typename F>
    class FreeFunctionCallable : public ICallable {
    public:
        FreeFunctionCallable(F&& f) : f_(std::forward<F>(f)) {}
        R Invoke(Args... args) override {
            return f_(std::forward<Args>(args)...);
        }
    private:
        F f_;
    };

    template <typename C>
    class MemberFunctionCallable : public ICallable {
    public:
        using Method = R (C::*)(Args...);
        MemberFunctionCallable(C* instance, Method method) : instance_(instance), method_(method) {}
        R Invoke(Args... args) override {
            return (instance_->*method_)(std::forward<Args>(args)...);
        }
    private:
        C* instance_;
        Method method_;
    };

    std::unique_ptr<ICallable> callable_;
};
