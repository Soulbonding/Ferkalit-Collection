#ifndef FERKALIT
#define FERKALIT

#include <stack>
#include <stdexcept>

/* Structures */

// Use any associative function
struct min_function {
    template <typename T>
    T operator() (const T &a, const T &b) {
        return (a <= b) ? a : b;
    }
};

template <typename T, typename F = min_function>
class stack_with_function {
public:
    stack_with_function() = default;

    void push(const T &value) {
        main_stack.push(value);
        if (function_stack.empty()) {
            function_stack.push(value);
        }
        else {
            F function;
            T calc = function(value, function_stack.top());
            function_stack.push(calc);
        }
    }

    void pop() {
        if (function_stack.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        main_stack.pop();
        function_stack.pop();
    }

    void clear() {
        main_stack = std::stack<T>();
        function_stack = std::stack<T>();
    }

    [[nodiscrd]] T top() const {
        if (function_stack.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return main_stack.top();
    }

    [[nodiscard]] T get_function_value() const {
        if (function_stack.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return function_stack.top();
    }

    [[nodiscard]] bool is_empty() const {
        return main_stack.empty();
    }

    [[nodiscard]] size_t size() const {
        return main_stack.size();
    }
private:
    std::stack<T> main_stack;
    std::stack<T> function_stack;
};



template <typename T, typename F = min_function>
class queue_with_function {
public:
    queue_with_function() = default;

    void push(const T &value) {
        tail.push(value);
    }

    void pop() {
        if (head.is_empty()) {
            if (tail.is_empty()) {
                throw std::out_of_range("Queue is empty");
            }
            while (!tail.is_empty()) {
                head.push(tail.top());
                tail.pop();
            }
        }
        head.pop();
    }

    void clear() {
        head.clear();
        tail.clear();
    }

    [[nodiscard]] T front() { // Not optimised yet, I don't know if it's actually O(1) amortised time
        if (head.is_empty()) {
            if (tail.is_empty()) {
                throw std::out_of_range("Queue is empty");
            }
            while (!tail.is_empty()) {
                head.push(tail.top());
                tail.pop();
            }
        }
        return head.top();
    }

    [[nodiscard]] T get_function_value() {
        if (head.is_empty() && tail.is_empty()) {
            throw std::out_of_range("Queue is empty");
        }
        if (head.is_empty()) {
            return tail.get_function_value();
        }
        if (tail.is_empty()) {
            return head.get_function_value();
        }
        F function;
        return function(head.get_function_value(), tail.get_function_value());
    }

    [[nodiscard]] bool is_empty() const {
        return head.is_empty() && tail.is_empty();
    }

    [[nodiscard]] size_t size() const {
        return head.size() + tail.size();
    }
private:
    stack_with_function<T, F> tail;
    stack_with_function<T, F> head;
};

#endif