#include <iostream>
#include <vector>
#include <cmath>      // для std::floor
#include <stdexcept>  // для исключений (опционально)

namespace math {

using real = double;

class Matrix {
private:
    int rows_;
    int cols_;
    std::vector<real> data_;

public:
    // Конструкторы
    Matrix() : rows_(0), cols_(0), data_() {}

    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows * cols) {}

    // Доступ к элементам (с проверкой границ)
    real& operator()(int row, int col) {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            throw std::out_of_range("Matrix index out of range");
        }
        return data_[row * cols_ + col];
    }

    real operator()(int row, int col) const {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            throw std::out_of_range("Matrix index out of range");
        }
        return data_[row * cols_ + col];
    }

    // Печать (для обратной совместимости)
    void print() const {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                std::cout << (*this)(i, j) << ' ';
            }
            std::cout << '\n';
        }
    }

    // Операторы составного присваивания (методы)
    Matrix& operator+=(const Matrix& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            std::cerr << "Matrix::operator+=: dimensions mismatch\n";
            return *this;
        }
        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i] += other.data_[i];
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            std::cerr << "Matrix::operator-=: dimensions mismatch\n";
            return *this;
        }
        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i] -= other.data_[i];
        }
        return *this;
    }

    Matrix& operator*=(real scalar) {
        for (real& val : data_) {
            val *= scalar;
        }
        return *this;
    }

    // Дружественные бинарные операторы (объявлены здесь для удобства)
    friend Matrix operator+(const Matrix& A, const Matrix& B);
    friend Matrix operator-(const Matrix& A, const Matrix& B);
    friend Matrix operator*(const Matrix& A, const Matrix& B);

    // Дружественные операторы ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
    friend std::istream& operator>>(std::istream& is, Matrix& mat);
};

// Реализация бинарных операторов (сложение, вычитание, умножение матриц)
Matrix operator+(const Matrix& A, const Matrix& B) {
    if (A.rows_ != B.rows_ || A.cols_ != B.cols_) {
        std::cerr << "Matrix::operator+: dimension mismatch, returning empty matrix\n";
        return Matrix();
    }
    Matrix result(A.rows_, A.cols_);
    for (size_t i = 0; i < result.data_.size(); ++i) {
        result.data_[i] = A.data_[i] + B.data_[i];
    }
    return result;
}

Matrix operator-(const Matrix& A, const Matrix& B) {
    if (A.rows_ != B.rows_ || A.cols_ != B.cols_) {
        std::cerr << "Matrix::operator-: dimension mismatch, returning empty matrix\n";
        return Matrix();
    }
    Matrix result(A.rows_, A.cols_);
    for (size_t i = 0; i < result.data_.size(); ++i) {
        result.data_[i] = A.data_[i] - B.data_[i];
    }
    return result;
}

Matrix operator*(const Matrix& A, const Matrix& B) {
    if (A.cols_ != B.rows_) {
        std::cerr << "Matrix::operator*: matrices cannot be multiplied, returning empty matrix\n";
        return Matrix();
    }
    Matrix result(A.rows_, B.cols_);
    for (int i = 0; i < A.rows_; ++i) {
        for (int j = 0; j < B.cols_; ++j) {
            real sum = 0;
            for (int k = 0; k < A.cols_; ++k) {
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

// Операторы ввода/вывода
std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    for (int i = 0; i < mat.rows_; ++i) {
        for (int j = 0; j < mat.cols_; ++j) {
            os << mat(i, j) << ' ';
        }
        os << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& mat) {
    int rows, cols;
    is >> rows >> cols;                 // сначала читаем размеры
    if (!is) return is;

    mat.rows_ = rows;
    mat.cols_ = cols;
    mat.data_.resize(rows * cols);

    for (real& val : mat.data_) {
        is >> val;                       // затем все элементы подряд
    }
    return is;
}

} // namespace math

// ---------- Демонстрация в main ----------
int main() {
    using namespace math;

    // Исходная демонстрация (из условия)
    Matrix m(3, 3);
    m(0, 0) = 1.0;
    m(1, 1) = 1.0;

    Matrix m1(3, 3);
    m1(0, 0) = 5.0;
    m1(1, 1) = 5.0;

    std::cout << "Matrix m is:\n" << m;          // используем operator<<
    std::cout << "\nMatrix m1 is:\n" << m1;

    std::cout << "\nSum of matrices m and m1 is:\n";
    Matrix m2 = m + m1;
    std::cout << m2;

    std::cout << "\nSubtraction of matrices m and m1 is:\n";
    Matrix m3 = m - m1;
    std::cout << m3;

    std::cout << "\nMultiplication of matrices m and m1 is:\n";
    Matrix m4 = m * m1;
    std::cout << m4;

    // Демонстрация новых операторов
    std::cout << "\n--- New operators demo ---\n";

    // Составное присваивание +=
    Matrix A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    Matrix B(2, 2);
    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;

    std::cout << "A:\n" << A;
    std::cout << "B:\n" << B;

    A += B;
    std::cout << "After A += B:\n" << A;

    // Составное присваивание -=
    Matrix C(2, 2);
    C(0, 0) = 10; C(0, 1) = 20;
    C(1, 0) = 30; C(1, 1) = 40;

    C -= B;
    std::cout << "After C -= B:\n" << C;

    // Составное присваивание *= (на число)
    C *= 2.5;
    std::cout << "After C *= 2.5:\n" << C;

    // Оператор ввода >>
    std::cout << "\nEnter matrix dimensions and elements (e.g. 2 3  1 2 3 4 5 6):\n";
    Matrix D;
    std::cin >> D;
    std::cout << "You entered:\n" << D;

    return 0;
}