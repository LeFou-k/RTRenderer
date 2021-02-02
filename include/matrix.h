//
// Created by LiuKe on 2021/2/2.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "vec.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - - - matrix - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

template<int n>
class dt;

template<int nrows, int ncols>
class matrix {
public:
    matrix() = default;

    vec<ncols> &operator[](const int row_idx) {
        assert(row_idx >= 0 && row_idx < nrows);
        return rows[row_idx];
    }

    const vec<ncols> &operator[](const int row_idx) const {
        assert(row_idx >= 0 && row_idx < nrows);
        return rows[row_idx];
    }

    matrix<nrows, ncols> operator-() {
        matrix<nrows, ncols> res;
        for (int i = 0; i < nrows; ++i)
            for (int j = 0; j < ncols; ++j)
                res[i][j] = -rows[i][j];
        return res;
    }

    matrix<nrows, ncols> &operator+=(const matrix<nrows, ncols> &mat) {
        for (int i = 0; i < nrows; ++i)
            for (int j = 0; j < ncols; ++j)
                rows[i][j] += mat[i][j];
        return *this;
    }

    matrix<nrows, ncols> &operator-=(const matrix<nrows, ncols> &mat) {
        (*this) += -mat;
        return *this;
    }

    matrix<nrows, ncols> &operator*=(const double t) {
        for (int i = 0; i < nrows; ++i)
            for (int j = 0; j < ncols; ++j)
                rows[i][j] *= t;
        return *this;
    }

    matrix<nrows, ncols> &operator/=(const double t) {
        assert(t != 0);
        (*this) *= (1 / t);
        return *this;
    }

    const vec<nrows> &col(const int col_idx) const {
        assert(col_idx >= 0 && col_idx < ncols);
        vec<nrows> res;
        for (int i = 0; i < nrows; ++i)
            res[i] = rows[i][col_idx];
        return res;
    }

    void set_col(const vec<nrows> &row, const int col_idx) {
        assert(col_idx >= 0 && col_idx < ncols);
        for (int i = 0; i < nrows; i++)
            rows[i][col_idx] = row[i];
    }

    matrix<nrows - 1, ncols - 1> get_minor(const int row, const int col) const {
        matrix<nrows - 1, ncols - 1> res;
        for (int i = nrows - 1; i >= 0; --i)
            for (int j = ncols - 1; j >= 0; --j)
                res[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
        return res;
    }

    double cofactor(const int row, const int col) const {
        return get_minor(row, col).det() * ((row + col) % 2 ? 1 : -1);
    }

    double det() const {
        assert(ncols == nrows);
        return dt<ncols>::det(*this);
    }

    matrix<nrows, ncols> adjugate() const {
        matrix<nrows, ncols> res;
        for (int i = 0; i < nrows; ++i)
            for (int j = 0; j < ncols; ++j)
                res[i][j] = cofactor(i, j);
        return res;
    }

    matrix<nrows, ncols> invert_transpose() const {
        auto temp = adjugate();
        return temp / (rows[0] * temp[0]);
    }

    matrix<ncols, nrows> transpose() const {
        matrix<ncols, nrows> res;
        for (int i = 0; i < ncols; ++i)
            res[i] = this->col(i);
        return res;
    }

    matrix<ncols, nrows> invert() const {
        return this->invert_transpose().transpose();
    }

private:
    vec<nrows> rows[ncols] = {{}}
};

/*- - - - - - - - - - - - - - - - - - - - - - - - matrix utils - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
template<int nrows, int ncols>
vec<nrows> operator*(matrix<nrows, ncols> &mat, vec<ncols> &v) {
    vec<nrows> res;
    for (int i = 0; i < nrows; ++i)
        res[i] = mat[i] * v;
    return res;
}

template<int r, int c1, int c2>
matrix<r, c2> operator*(matrix<r, c1> &m1, matrix<c1, c2> &m2) {
    matrix<r, c2> res;
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c2; ++j)
            res[i][j] = m1[i] * m2.col(j);
    return res;
}

template<int nrows, int ncols>
matrix<nrows, ncols> operator*(matrix<nrows, ncols> &mat, double t) {
    for (int i = 0; i < nrows; ++i)
        for (int j = 0; j < nrows; ++j)
            mat[i][j] *= t;
    return mat;
}

template<int nrows, int ncols>
matrix<nrows, ncols> operator/(matrix<nrows, ncols> &mat, double t) {
    assert(t != 0);
    return mat * (1 / t);
}

template<int nrows, int ncols>
matrix<nrows, ncols> operator+(matrix<nrows, ncols> &m1, matrix<nrows, ncols> &m2) {
    matrix<nrows, ncols> res;
    for (int i = 0; i < nrows; ++i)
        for (int j = 0; j < ncols; ++j)
            res[i][j] = m1[i][j] + m2[i][j];
    return res;
}

template<int nrows, int ncols>
matrix<nrows, ncols> operator-(matrix<nrows, ncols> &m1, matrix<nrows, ncols> &m2) {
    return m1 + (-m2);
}

/*- - - - - - - - - - - - - - - - - - - - - - - - determinant - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
template<>
class dt {
public:
    static double det(matrix<n, n> &mat) const {
        double res = 0;
        for (int i = 0; i < n; ++i)
            res += mat[0][i] * mat.cofactor(0, i);
        return res;
    }
};

template<>
class dt<1> {
public:
    static double det(matrix<1><1> &mat) const {
        return mat[0][0];
    }
};

#endif //MATRIX_H
