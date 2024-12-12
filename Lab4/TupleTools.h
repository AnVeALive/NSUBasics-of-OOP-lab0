#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

//===========+ TuplePrinter +===========+//
template <typename T, unsigned I, unsigned S>
struct TuplePrinter {
    static void print(std::ostream& os, const T& tuple) {
        os << std::get<I>(tuple) << ", ";
        TuplePrinter<T, I + 1, S>::print(os, tuple);

        /*
        if constexpr(I < S) {
            TuplePrinter<T, I + 1, S>::print(os, tuple);
        }
        */
    }
};

template <typename T, unsigned I>
struct TuplePrinter<T, I, I> {
    static void print(std::ostream& os, const T& tuple) {
        os << std::get<I>(tuple);
    }
};

template <typename T, unsigned I>
struct TuplePrinter<T, I, 0> {
    static void print(std::ostream& os, const T& tuple) {}
};

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& tuple) {
    TuplePrinter<decltype(tuple), 0, sizeof...(Args) - 1>::print(os, tuple);
    return os;
};



//===========+ TypeHandler +===========+//
template <typename T>
struct TypeHandler {
    static void process(T& value, const std::string& string) {
        std::istringstream is(string);
        is >> value;
    }
};

template <>
struct TypeHandler<std::string> {
    static void process(std::string& value, const std::string& string) {
        value = string;
    }
};



//===========+ TuplePlaceholder +===========+//
template <typename T, typename V, unsigned I, unsigned S>
struct TuplePlaceholder {
    static void add(T& tuple, const V& vector) {
        using ValueType = typename std::tuple_element<I, T>::type;
        ValueType value;

        TypeHandler<ValueType>::process(value, vector[I]);
        std::get<I>(tuple) = value;
        TuplePlaceholder<T, V, I + 1, S>::add(tuple, vector);
    }
};

template <typename T, typename V, unsigned I>
struct TuplePlaceholder<T, V, I, I> {
    static void add(T& tuple, const V& vector) {
        using ValueType = typename std::tuple_element<I, T>::type;
        ValueType value;

        TypeHandler<ValueType>::process(value, vector[I]);
        std::get<I>(tuple) = value;
    }
};

template <typename T, typename V, unsigned I>
struct TuplePlaceholder<T, V, I, 0> {
    static void add(T& tuple, const V& vector) {}
};