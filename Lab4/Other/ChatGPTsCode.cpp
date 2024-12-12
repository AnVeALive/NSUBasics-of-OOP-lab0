#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <stdexcept>

// Subtask #1: Print tuple template
namespace detail {
    template <typename Tuple, std::size_t Index>
    struct TuplePrinter {
        static void print(std::ostream& os, const Tuple& t) {
            TuplePrinter<Tuple, Index - 1>::print(os, t);
            os << ", " << std::get<Index>(t);
        }
    };

    template <typename Tuple>
    struct TuplePrinter<Tuple, 0> {
        static void print(std::ostream& os, const Tuple& t) {
            os << std::get<0>(t);
        }
    };
}

template <typename... Args>
auto operator<<(std::ostream& os, const std::tuple<Args...>& t) -> std::ostream& {
    os << "(";
    detail::TuplePrinter<decltype(t), sizeof...(Args) - 1>::print(os, t);
    os << ")";
    return os;
}

// Subtask #2: Simple CSV parser
class CSVParserError : public std::runtime_error {
public:
    CSVParserError(const std::string& message, std::size_t line, std::size_t column)
        : std::runtime_error(message), line_(line), column_(column) {}

    std::size_t line() const { return line_; }
    std::size_t column() const { return column_; }

private:
    std::size_t line_;
    std::size_t column_;
};

template <typename... Args>
class CSVParser {
    using TupleType = std::tuple<Args...>;

public:
    CSVParser(std::istream& input, std::size_t skip_lines = 0, char column_delim = ',', char row_delim = '\n', char escape_char = '"')
        : input_(input), column_delim_(column_delim), row_delim_(row_delim), escape_char_(escape_char) {
        for (std::size_t i = 0; i < skip_lines; ++i) {
            std::string dummy;
            std::getline(input_, dummy, row_delim);
        }
    }

    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = TupleType;
        using difference_type = std::ptrdiff_t;
        using pointer = TupleType*;
        using reference = TupleType&;

        Iterator(std::istream* input, char column_delim, char row_delim, char escape_char)
            : input_(input), column_delim_(column_delim), row_delim_(row_delim), escape_char_(escape_char) {
            ++(*this); // Load the first tuple
        }

        Iterator() : input_(nullptr) {}

        TupleType operator*() const { return current_tuple_; }

        Iterator& operator++() {
            if (input_ && !input_->eof()) {
                std::string line;
                if (std::getline(*input_, line, row_delim_)) {
                    parseLine(line); } else {
                    input_ = nullptr; // End of input
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return input_ != other.input_;
        }

    private:
        void parseLine(const std::string& line) {
            std::istringstream line_stream(line);
            parseColumns(line_stream, current_tuple_, std::index_sequence_for<Args...>{});
        }

        template <typename T>
        T parseValue(std::istream& is) {
            std::string value;
            char c;
            if (is.peek() == escape_char_) {
                is.get(); // Discard escape char
                while (is.get(c)) {
                    if (c == escape_char_ && is.peek() == escape_char_) {
                        is.get(); // Escaped escape char
                        value += c;
                    } else if (c == escape_char_) {
                        break;
                    } else {
                        value += c;
                    }
                }
            } else {
                std::getline(is, value, column_delim_);
            }

            std::istringstream value_stream(value);
            T result;
            if (!(value_stream >> result)) {
                throw CSVParserError("Failed to parse value", current_line_, current_column_);
            }
            return result;
        }

        template <std::size_t... Indices>
        void parseColumns(std::istream& line_stream, TupleType& tuple, std::index_sequence<Indices...>) {
            (..., (std::get<Indices>(tuple) = parseValue<std::tuple_element_t<Indices, TupleType>>(line_stream)));
        }

        std::istream* input_;
        TupleType current_tuple_;
        std::size_t current_line_ = 0;
        std::size_t current_column_ = 0;
        char column_delim_;
        char row_delim_;
        char escape_char_;
    };

    Iterator begin() {
        return Iterator(&input_, column_delim_, row_delim_, escape_char_);
    }

    Iterator end() {
        return Iterator();
    }

private:
    std::istream& input_;
    char column_delim_;
    char row_delim_;
    char escape_char_;
};

// TuplePlaceholder

// TuplePrinter

int main() {
    std::ifstream file("test.csv");
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    try {
        CSVParser<int, std::string> parser(file, 0);
        for (auto row : parser) {
            std::cout << row << std::endl;
        }
    } catch (const CSVParserError& e) {
        std::cerr << "Error parsing CSV at line " << e.line() << ", column " << e.column() << ": " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
