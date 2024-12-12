#include <fstream>
#include <string>
#include <tuple>

#include "CsvIterator.h"



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