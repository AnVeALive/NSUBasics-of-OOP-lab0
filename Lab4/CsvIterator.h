#include <fstream>
#include <string>
#include <tuple>
#include <vector>



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