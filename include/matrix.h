//
// Created by LiuKe on 2021/2/2.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "vec.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - matrix - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

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

    matrix<nrows, ncols> adjugate() const{
        matrix<nrows, ncols> res;
        for(int i = 0; i < nrows; ++i)
            for(int j = 0; j < ncols; ++j)
                res[i][j] = cofactor(i, j);
        return res;
    }

    matrix<nrows, ncols> invert_transpose() const{
        auto temp = adjugate();
        return temp / (rows[0] * temp[0]);
    }

    matrix<ncols, nrows> transpose() const{
        matrix<ncols, nrows> res;
        for(int i = 0; i < ncols; ++i)
            res[i] = this->col(i);
        return res;
    }

    matrix<ncols, nrows> invert() const{
        return this->invert_transpose().transpose();
    }
private:
    vec<nrows> rows[ncols] = {{}}
};
/*- - - - - - - - - - - - - - - - - - - - - - - - matrix utils - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*- - - - - - - - - - - - - - - - - - - - - - - - determinant - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

template<>
class dt {
public:
    static double det(matrix<n, n> &mat) const {
        double res = 0;
        for(int i = 0; i < n; ++i)
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
