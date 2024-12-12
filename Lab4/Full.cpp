#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#define TableTemplate int, std::string, float, int



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



//===========+ CsvParserError +===========+//
// class CsvParserError : public std::runtime_error {
// public:
//     CsvParserError(const std::string& message, const std::size_t row, const std::size_t col) :
//     std::runtime_error(message), _row(row), _col(col) {}

//     std::size_t getRow() const {
//         return _row;
//     }

//     std::size_t getCol() const {
//         return _col;
//     }

// private:
//     std::size_t _row;
//     std::size_t _col;
// };



//===========+ CsvIterator +===========+//
template <typename... Args>
class CsvIterator {
    using TupleType = std::tuple<Args...>;
public:
    CsvIterator(std::istream* input, const char row_delim, const char col_delim, const char escape_symbol) :
    _input(input), _row_delim(row_delim), _col_delim(col_delim), _escape_symbol(escape_symbol) {
        ++(*this);
    }

    CsvIterator() : _input(nullptr) {}

    TupleType operator*() const {
        return _cur_tuple;
    }

    CsvIterator<Args...>& operator++() {
        if (_input && !_input->eof()) {
            std::string line;
            if (std::getline(*_input, line, _row_delim)) {
                ++_cur_row;
                _cur_col = 0;
                parseLine(line);
            }
            else {
                _input = nullptr;
            }
        }
        return *this;
    }

    bool operator!=(const CsvIterator<Args...>& iterator) {
        return _input != iterator._input;
    }

private:
    void parseLine(const std::string& line) {
        std::vector<std::string> columns_per_row;
        std::string              col;

        bool escape_flag = false;
        std::size_t size_line = line.size();
        for (int i = 0; i < size_line; ++i) {
            if (line[i] == _col_delim && !escape_flag) {
                ++_cur_col;
                columns_per_row.push_back(col);
                col.clear();
            }
            else if (line[i] == _escape_symbol) {
                if (i != size_line - 1 && line[i + 1] == _escape_symbol) {
                    ++i;
                    col += _escape_symbol;
                }
                else if (!escape_flag) {
                    escape_flag = true;
                }
                else if (escape_flag) {
                    escape_flag = false;
                }
            }
            else {
                col += line[i];
            }
        }
        columns_per_row.push_back(col);

        if (columns_per_row.size() != sizeof...(Args)) {
            throw std::invalid_argument("Error in (" + std::to_string(_cur_row) + ", " + std::to_string(_cur_col) + "): Ivalid format csv table.");
        }

        TuplePlaceholder<TupleType, std::vector<std::string>, 0, sizeof...(Args) - 1>::add(_cur_tuple, columns_per_row);
    }

    std::istream* _input;
    char          _col_delim;
    char          _escape_symbol;
    char          _row_delim;
    std::size_t   _cur_row = 0;
    std::size_t   _cur_col = 0;
    TupleType     _cur_tuple;
};

//===========+ CsvParser +===========+//
template <typename... Args>
class CsvParser {
    using TupleType = std::tuple<Args...>;

public:
    CsvParser(std::istream& input, const std::size_t skip_lines = 0,
    const char row_delim = '\n', const char col_delim = ',', const char escape_symbol = '"') :
    _input(input), _row_delim(row_delim), _col_delim(col_delim), _escape_symbol(escape_symbol) {
        std::string dummy;
        for (int i = 0; i < skip_lines; i++) {
            std::getline(input, dummy, row_delim);
        }
    }

    friend class CsvIterator<Args...>;

    CsvIterator<Args...> begin() {
        return CsvIterator<Args...>(&_input, _row_delim, _col_delim, _escape_symbol);
    }

    CsvIterator<Args...> end() {
        return CsvIterator<Args...>();
    }

private:
    std::istream& _input;
    char          _row_delim;
    char          _col_delim;
    char          _escape_symbol;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments!" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "File is not opened!" << std::endl;
        return 1;
    }

    try {
        CsvParser<TableTemplate> parser(file, 0);
        for (auto rs : parser) {
            std::cout << '(' << rs << ')' << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Errow." << std::endl;
    }

    return 0;
}