/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Matrix_op_hpp_
#define _include_bleifrei_Matrix_op_hpp_ 


// includes
#include <cmath>

#include "Matrix.hpp"

namespace bleifrei {
	namespace math {

		/** Add two matrices.
		 * Sum two matrices by each element, placing the result in
		 * the destination Matrix and returning a reference to it.
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> &add(Matrix<m, n, T> &dst, const Matrix<m, n, T> &src1, const Matrix<m, n, T> &src2)
		{
			for (int pos = 0; pos < m * n; ++pos) {
				dst[pos] = src1[pos] + src2[pos];
			}

			return dst;
		}

		template<>
		inline Matrix<3, 1, float> &add(Matrix<3, 1, float> &dst, const Matrix<3, 1, float> &src1, const Matrix<3, 1, float> &src2)
		{
			dst[0] = src1[0] + src2[0];
			dst[1] = src1[1] + src2[1];
			dst[2] = src1[2] + src2[2];

			return dst;
		}

		template<>
		inline Matrix<4, 1, float> &add(Matrix<4, 1, float> &dst, const Matrix<4, 1, float> &src1, const Matrix<4, 1, float> &src2)
		{
			dst[0] = src1[0] + src2[0];
			dst[1] = src1[1] + src2[1];
			dst[2] = src1[2] + src2[2];
			dst[3] = src1[3] + src2[3];

			return dst;
		}

		/** Add two matrices.
		 * Sum two matrices by each element and return the result as a new Matrix.
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> operator +(const Matrix<m, n, T> &mat1, const Matrix<m, n, T> &mat2)
		{
			Matrix<m, n, T> temp;

			add(temp, mat1, mat2);

			return temp;
		}

		/** Add one matrix to another.
		 * Sum two matrices by each element and return the result in the first Matrix.
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> &operator +=(Matrix<m, n, T> &mat1, const Matrix<m, n, T> &mat2)
		{
			return add(mat1, mat1, mat2);
		}

		/** Subtract two matrices.
		 * Subtract two matrices by each element, placing the result in
		 * the destination Matrix and returning a reference to it.
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> &sub(Matrix<m, n, T> &dst, const Matrix<m, n, T> &src1, const Matrix<m, n, T> &src2)
		{
			for (int pos = 0; pos < m * n; ++pos) {
				dst[pos] = src1[pos] - src2[pos];
			}

			return dst;
		}
		
		template<>
		inline Matrix<3, 1, float> &sub(Matrix<3, 1, float> &dst, const Matrix<3, 1, float> &src1, const Matrix<3, 1, float> &src2)
		{
			dst[0] = src1[0] - src2[0];
			dst[1] = src1[1] - src2[1];
			dst[2] = src1[2] - src2[2];

			return dst;
		}

		template<>
		inline Matrix<4, 1, float> &sub(Matrix<4, 1, float> &dst, const Matrix<4, 1, float> &src1, const Matrix<4, 1, float> &src2)
		{
			dst[0] = src1[0] - src2[0];
			dst[1] = src1[1] - src2[1];
			dst[2] = src1[2] - src2[2];
			dst[3] = src1[3] - src2[3];

			return dst;
		}

		/** Subtract two matrices.
		 * Subtract two matrices by each element and return the result as a new Matrix.
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> operator -(const Matrix<m, n, T> &mat1, const Matrix<m, n, T> &mat2)
		{
			Matrix<m, n, T> temp;

			sub(temp, mat1, mat2);

			return temp;
		}

		/* TODO: REWIRTE! */
		template<int m, int n, class T>
		inline Matrix<m, n, T> operator -(const Matrix<m, n, T> &mat)
		{
			Matrix<m, n, T> temp;

			for (int pos = 0; pos < m * n; ++pos) {
				temp[pos] = -mat[pos];
			}

			return temp;
		}

		/** Subtract one matrix from another.
		 * Subtract two matrices by each element and return the result in the first Matrix.
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> &operator -=(Matrix<m, n, T> &mat1, const Matrix<m, n, T> &mat2)
		{
			return sub(mat1, mat1, mat2);
		}

		/** Negate matrix.
		 * Negate each element in a matrix and assigns the result to another
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> &neg(Matrix<m, n, T> &dst, const Matrix<m, n, T> &src)
		{
			for (int pos = 0; pos < m * n; ++pos) {
				dst[pos] = -src[pos];
			}

			return dst;
		}


		/** Multiply two matrices.
		 * Calculate the dot product between each line in the first matrix and each column
		 * in the second.
		 */
		template<int m, int n, int l, class T>
		inline Matrix<m, l, T> mul(Matrix<m, l, T> &dst, const Matrix<m, n, T> &src1, const Matrix<n, l, T> &src2)
		{
			for (int i = 0; i < m; ++i) {
				for (int j = 0; j < l; ++j) {
					dst(i, j) = 0;
					for (int w = 0; w < n; ++w) {
						dst(i, j) += src2(w, j) * src1(i, w);
					}
				}
			}
			return dst;
		}

		template<>
		inline Matrix<4, 1, float> mul(Matrix<4, 1, float> &dst, const Matrix<4, 4, float> &src1, const Matrix<4, 1, float> &src2)
		{
			dst[0] = src1(0, 0) * src2[0] + src1(0, 1) * src2[1] + src1(0, 2) * src2[2] + src1(0, 3) * src2[3];
			dst[1] = src1(1, 0) * src2[0] + src1(1, 1) * src2[1] + src1(1, 2) * src2[2] + src1(1, 3) * src2[3];
			dst[2] = src1(2, 0) * src2[0] + src1(2, 1) * src2[1] + src1(2, 2) * src2[2] + src1(2, 3) * src2[3];
			dst[3] = src1(3, 0) * src2[0] + src1(3, 1) * src2[1] + src1(3, 2) * src2[2] + src1(3, 3) * src2[3];

			return dst;
		}

		template<class T>
		inline Matrix<3, 1, T> mul(Matrix<3, 1, T> &dst, const Matrix<4, 4, T> &src1, const Matrix<3, 1, T> &src2)
		{
			dst[0] = src1(0, 0) * src2[0] + src1(0, 1) * src2[1] + src1(0, 2) * src2[2];
			dst[1] = src1(1, 0) * src2[0] + src1(1, 1) * src2[1] + src1(1, 2) * src2[2];
			dst[2] = src1(2, 0) * src2[0] + src1(2, 1) * src2[1] + src1(2, 2) * src2[2];

			return dst;
		}

		/** Multiply two matrices.
		 * Calculate the dot product between each line in the first matrix and each column
		 * in the second.
		 */
		template<int m, int n, int l, class T>
		inline Matrix<m, l, T> operator *(const Matrix<m, n, T> &m1, const Matrix<n, l, T> &m2)
		{
			Matrix<m, l, T> temp;

			mul(temp, m1, m2);

			return temp;
		}

		template<class T>
		inline Matrix<3, 1, T> operator *(const Matrix<4, 4, T> &m1, const Matrix<3, 1, T> &m2)
		{
			Matrix<3, 1, T> temp;

			mul(temp, m1, m2);

			return temp;
		}

		/** Scale a matrix by constant.
		 *
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> &scale(Matrix<m, n, T> &dst, const Matrix<m, n, T> &src, T factor)
		{
			for (int pos = 0; pos < m * n; ++pos) {
				dst[pos] = src[pos] * factor;
			}

			return dst;
		}

		/** Scale a matrix by constant.
		 *
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> operator *(const Matrix<m, n, T> &mat, T factor)
		{
			Matrix<m, n, T> temp;

			return scale(temp, mat, factor);
		}

		/** Scale a matrix by constant.
		 *
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> operator *(T factor, const Matrix<m, n, T> &mat)
		{
			Matrix<m, n, T> temp;

			return scale(temp, mat, factor);
		}

		/** Scale a matrix by constant.
		 *
		 */
		template<int m, int n, class T>
		inline Matrix<m, n, T> operator *=(Matrix<m, n, T> &mat, T factor)
		{
			return scale(mat, mat, factor);
		}

		/** Column vector dot product.
		 * Calculate the dot product between two one-dimensional matrices(vectors).
		 */
		template<int m, class T>
		inline T operator *(const Matrix<m, 1, T> &mat1, const Matrix<m, 1, T> mat2)
		{
			T result = 0;

			for (int i = 0; i < m; ++i) result += mat1[i] * mat2[i];

			return result;
		}

		/** Row vector dot product.
		 * Calculate the dot product between two one-dimensional matrices(vectors).
		 */
		template<int n, class T>
		inline T operator *(const Matrix<1, n, T> &mat1, const Matrix<1, n, T> mat2)
		{
			T result = 0;

			for (int i = 0; i < m; ++i) result += mat1[i] * mat2[i];

			return result;
		}

		/** Matrix transpose.
		 */
		template<int m, int n, class T>
		inline Matrix<n, m, T> &transpose(Matrix<n, m, T> &dst, const Matrix<m, n, T> &src)
		{
			int pos = 0;
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < m; ++j) {
					dst(i, j) = src[pos++];
				}
			}
			return dst;
		}

		template<class T>
		inline Matrix<3, 1, T> &cross(Matrix<3, 1, T> &dst, const Matrix<3, 1, T> &src1, const Matrix<3, 1, T> &src2)
		{
			dst[0] = src1[1] * src2[2] - src1[2] * src2[1];
			dst[1] = src1[2] * src2[0] - src1[0] * src2[2];
			dst[2] = src1[0] * src2[1] - src1[1] * src2[0];

			return dst;
		}

		template<class T>
		inline Matrix<3, 1, T> operator ^(const Matrix<3, 1, T> &mat1, const Matrix<3, 1, T> &mat2)
		{
			Matrix<3, 1, T> temp;
			return cross(temp, mat1, mat2);
		}

		template<int m, class T>
		Matrix<m, 1, T> &normalize(Matrix<m, 1, T> &dst, const Matrix<m, 1, T> &src)
		{
			dst = src * (1 / length(src));
			return dst;
		}

		template<int m, class T>
		Matrix<m, 1, T> &normalize(Matrix<m, 1, T> &src)
		{
			src *= (1 / length(src));
			return src;
		}

		template<int m, class T>
		T length(const Matrix<m, 1, T> &src)
		{
			return (T)std::sqrt(src * src);
		}

		template<int m, int n, class T>
		inline bool operator ==(const Matrix<m, n, T> &mat1, const Matrix<m, n, T> &mat2)
		{
			for (int pos = 0; pos < m * n; ++pos) {
				if (mat1[pos] != mat2[pos]) return false;
			}
			return true;
		}

		template<class T>
		inline const Matrix<4, 1, T> &assign(Matrix<4, 1, T> &dst, const Matrix<3, 1, T> &src)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];

			return dst;
		}

		template<class T>
		inline const Matrix<3, 1, T> &assign(Matrix<3, 1, T> &dst, const Matrix<4, 1, T> &src)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];

			return dst;
		}

	}
}


#endif //_include_bleifrei_Matrix_op_hpp_ 
