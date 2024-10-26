#include "telescope.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <utility>

int calculate_max_width(const std::vector<int>& vec) {
    int max_width = 0;
    for (int num : vec) {
        int width = std::to_string(num).size();  // Get the number of digits (including sign if negative)
        if (width > max_width) {
            max_width = width;
        }
    }
    return max_width;
}

// Mocking the parse_matrix function to be tested
std::pair<size_t, size_t> parse_matrix(std::istream& in) {
    std::string line;
    size_t rows = 0;
    size_t columns = 0;

    while (std::getline(in, line)) { // Reads each row (line)
        if (line.empty()) continue;

        std::istringstream iss(line);
        int num;
        size_t current_columns = 0;

        while (iss >> num) {
            current_columns++;
        }

        if (rows == 0) {
            columns = current_columns;
        }
        else if (current_columns != columns) {
            throw std::invalid_argument("Inconsistent number of columns in matrix.");
        }

        rows++;
    }

    return { rows, columns };  // Return dimensions of the matrix
}

std::vector<int> parse_matrix(std::istream& in, const std::pair<size_t, size_t>& m_size) {
    std::vector<int> matrix;
    std::string line;
    size_t rows = m_size.first;
    size_t columns = m_size.second;
    size_t expected_size = rows * columns;

    if (rows == 0 || columns == 0) return matrix;  // Empty matrix case

    // Reset the input stream to read the matrix again
    in.clear();
    in.seekg(0, std::ios::beg);

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        int num;
        while (iss >> num) {
            matrix.push_back(num);
        }
    }

    if (matrix.size() != expected_size) {
        throw std::invalid_argument("Matrix dimensions do not match the size of the vector.");
    }

    return matrix;
}

void print_matrix(std::ostream& out, const std::pair<size_t, size_t>& m_size, const std::vector<int>& vec) {
    size_t rows = m_size.first;
    size_t columns = m_size.second;

    if (rows == 0 && columns == 0) {
        return;
    }

    // Check if the size of the vector matches the expected matrix size
    if (rows * columns != vec.size()) {
        throw std::invalid_argument("Matrix dimensions do not match the size of the vector.");
    }

    int max_width = calculate_max_width(vec);  // Calculate the longest number
    int column_width = max_width + 1;          // Column width includes padding (space left and right)

    // Print top border
    out << std::string(columns * (column_width + 2) + 1, '-') << "\n";

    // Print matrix content row by row
    for (size_t i = 0; i < rows; ++i) {
        out << "|";  // Start of each row with one space after '|'
        for (size_t j = 0; j < columns; ++j) {
            int index = i * columns + j;
            out << std::setw(column_width) << vec[index] << " |";  // Print each element, right-aligned, with space
        }
        out << "\n";
    }

    // Print bottom border
    out << std::string(columns * (column_width + 2) + 1, '-') << "\n";
}

// --------------------- STAGE 2 ---------------------
/*
 * Matici znaku (unsigned char) ze vstupniho proudu nactete do jednorozmerneho vektoru.
 * Ctete radne i "bile znaky", tj. mezery.
 * Nekonzistentni pocet prvku (neodpovida rozmerum matice] na vstupu by melo
 * vyvolat vyjimku std::invalid_argument.
 */

// Pomocná funkce pro přístup k prvku v jednorozměrném vektoru, který reprezentuje matici.
inline unsigned char& get(std::vector<unsigned char>& vec, size_t row, size_t col, const std::pair<size_t, size_t>& m_size) {
    return vec[row * m_size.second + col];
}

std::vector<unsigned char> parse_stream(std::istream& in, const std::pair<size_t, size_t>& m_size) {
    char ch;
    size_t rows = m_size.first;
    size_t cols = m_size.second;
    std::vector<unsigned char> matrix;
    size_t expected_size = rows * cols;

    if (rows == 0 && cols == 0) return matrix;

    while (in.get(ch)) {
        matrix.push_back(static_cast<unsigned char>(ch));
    }

    if (matrix.size() != expected_size) {
        throw std::invalid_argument("Number of characters does not match the matrix dimensions.");
    }

    return matrix;
}

void rotate_down(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    if (m_size.first == 0 || m_size.second == 0) return; // Pokud je matice prázdná, nic neděláme.

    size_t num_cols = m_size.second;
    std::vector<unsigned char> last_row(vec.end() - num_cols, vec.end());

    // Posuneme všechny řádky dolů
    for (size_t i = m_size.first - 1; i > 0; --i) {
        std::copy(vec.begin() + (i - 1) * num_cols, vec.begin() + i * num_cols, vec.begin() + i * num_cols);
    }

    // Poslední řádek přesuneme na začátek
    std::copy(last_row.begin(), last_row.end(), vec.begin());
}

void rotate_down(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, int step) {
    size_t num_rows = m_size.first;
    size_t num_cols = m_size.second;

    while (step < 0) {
        step += num_rows; // Convert negative steps to positive rotations up
    }

    step = step % num_rows;

    // No rotation needed if step is 0
    if (step == 0) return;

    // Split the vector into two parts:
    // - last step rows
    // - remaining rows
    std::vector<unsigned char> last_rows(vec.end() - step * num_cols, vec.end()); // Last `step` rows
    std::vector<unsigned char> remaining_rows(vec.begin(), vec.end() - step * num_cols); // Remaining rows

    // Now, we merge them together: last_rows first, then remaining_rows
    std::copy(last_rows.begin(), last_rows.end(), vec.begin()); // Move last `step` rows to the top
    std::copy(remaining_rows.begin(), remaining_rows.end(), vec.begin() + step * num_cols); // Shift remaining rows down
}

void rotate_right(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    if (m_size.first == 0 || m_size.second == 0) return; // Pokud je matice prázdná, nic neděláme.

    size_t num_rows = m_size.first;
    size_t num_cols = m_size.second;

    // Uložíme poslední sloupec.
    std::vector<unsigned char> last_col(num_rows);
    for (size_t i = 0; i < num_rows; ++i) {
        last_col[i] = get(vec, i, num_cols - 1, m_size);
    }

    // Posuneme všechny sloupce doprava.
    for (size_t col = num_cols - 1; col > 0; --col) {
        for (size_t row = 0; row < num_rows; ++row) {
            get(vec, row, col, m_size) = get(vec, row, col - 1, m_size);
        }
    }

    // Poslední sloupec se stane prvním.
    for (size_t i = 0; i < num_rows; ++i) {
        get(vec, i, 0, m_size) = last_col[i];
    }
}

void rotate_right(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, int step) {
    if (m_size.first == 0 || m_size.second == 0) return; // Pokud je matice prázdná, nic neděláme.

    size_t num_rows = m_size.first;
    size_t num_cols = m_size.second;

    // Normalizujeme krok, aby nebyl větší než počet sloupců.

    if (step < 0) {
        while (step < 0) {
            step += num_cols; // Pokud je krok záporný, převedeme ho na kladný krok směrem doprava.
        }
    }

    step = abs(step) % num_cols;

    // Pokud je step 0, neděláme žádnou změnu.
    if (step == 0) return;

    // Vytvoříme dočasnou matici, která bude obsahovat rotovaný výsledek.
    std::vector<unsigned char> temp(vec.size());

    // Provedeme rotaci o step sloupců doprava.
    for (size_t col = 0; col < num_cols; ++col) {
        size_t new_col = (col + step) % num_cols;
        for (size_t row = 0; row < num_rows; ++row) {
            get(temp, row, new_col, m_size) = get(vec, row, col, m_size);
        }
    }

    vec.swap(temp);
}

void swap_points(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, const Point& p1, const Point& p2) {
    // Ověříme, že body jsou uvnitř matice.
    if (p1.x >= m_size.first || p1.y >= m_size.second || p2.x >= m_size.first || p2.y >= m_size.second) {
        throw std::invalid_argument("Points are outside of matrix bounds.");
    }

    // Prohození hodnot na pozicích p1 a p2.
    std::swap(get(vec, p1.y, p1.x, m_size), get(vec, p2.y, p2.x, m_size));
}

void swap_points(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, const Point& p1, const Point& p2, const Point& delta) {
    if (p1.x + delta.x > m_size.second || p1.y + delta.y > m_size.first ||
        p2.x + delta.x > m_size.second || p2.y + delta.y > m_size.first) {
        throw std::invalid_argument("Rectangles are outside of matrix bounds.");
    }

    if ((p1.x < p2.x + delta.x && p1.x + delta.x > p2.x && p1.y < p2.y + delta.y && p1.y + delta.y > p2.y) ||
        (p2.x < p1.x + delta.x && p2.x + delta.x > p1.x && p2.y < p1.y + delta.y && p2.y + delta.y > p1.y)) {
        throw std::invalid_argument("Rectangles overlap.");
    }

    for (size_t i = 0; i < delta.y; ++i) {
        for (size_t j = 0; j < delta.x; ++j) {
            std::swap(get(vec, p1.y + i, p1.x + j, m_size), get(vec, p2.y + i, p2.x + j, m_size));
        }
    }
}

void decode_picture(const std::string& file, const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    // TODO
}



