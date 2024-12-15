#ifndef ASSOCIATIVE_STACK_QUEUE_DEQUE
#define ASSOCIATIVE_STACK_QUEUE_DEQUE

#include <stack>
#include <stdexcept>

/* Structures */

struct min_function { // Use any associative function
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

    [[nodiscard]] T top() const {
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



template <typename T, typename F = min_function>
class deque_with_function {
public:
    deque_with_function() = default;

    void push_back(const T &value) {
        tail.push(value);
    }

    void push_front(const T &value) {
        head.push(value);
    }

    void pop_back() { // Amortised time is O(1). When proving it, use potential function
        if (tail.is_empty()) {
            if (head.is_empty()) {
                throw std::out_of_range("Deque is empty");
            }
            const size_t half = head.size() / 2;
            std::stack<T> temp;
            for (size_t i = 0; i < half; i++) {
                temp.push(head.top());
                head.pop();
            }
            while (!head.is_empty()) {
                tail.push(head.top());
                head.pop();
            }
            while (!temp.empty()) {
                head.push(temp.top());
                temp.pop();
            }
        }
        tail.pop();
    }

    void pop_front() {
        if (head.is_empty()) {
            if (tail.is_empty()) {
                throw std::out_of_range("Deque is empty");
            }
            const size_t half = tail.size() / 2;
            std::stack<T> temp;
            for (size_t i = 0; i < half; i++) {
                temp.push(tail.top());
                tail.pop();
            }
            while (!tail.is_empty()) {
                head.push(tail.top());
                tail.pop();
            }
            while (!temp.empty()) {
                tail.push(temp.top());
                temp.pop();
            }
        }
        head.pop();
    }

    void clear() {
        tail.clear();
        head.clear();
    }

    [[nodiscard]] T top() { // Not optimised too
        if (tail.is_empty()) {
            if (head.is_empty()) {
                throw std::out_of_range("Deque is empty");
            }
            const size_t half = head.size() / 2;
            std::stack<T> temp;
            for (size_t i = 0; i < half; i++) {
                temp.push(head.top());
                head.pop();
            }
            while (!head.is_empty()) {
                tail.push(head.top());
                head.pop();
            }
            while (!temp.empty()) {
                head.push(temp.top());
                temp.pop();
            }
        }
        return tail.top();
    }

    [[nodiscard]] T front() { // Not optimised too
        if (head.is_empty()) {
            if (tail.is_empty()) {
                throw std::out_of_range("Deque is empty");
            }
            const size_t half = tail.size() / 2;
            std::stack<T> temp;
            for (size_t i = 0; i < half; i++) {
                temp.push(tail.top());
                tail.pop();
            }
            while (!tail.is_empty()) {
                head.push(tail.top());
                tail.pop();
            }
            while (!temp.empty()) {
                tail.push(temp.top());
                temp.pop();
            }
        }
        return head.top();
    }

    [[nodiscard]] T get_function_value() const {
        if (head.is_empty() && tail.is_empty()) {
            throw std::out_of_range("Deque is empty");
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