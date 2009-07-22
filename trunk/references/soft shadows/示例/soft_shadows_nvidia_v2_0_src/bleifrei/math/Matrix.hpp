/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_math_matrix_hpp_
#define _include_bleifrei_math_matrix_hpp_ 


namespace bleifrei {
	namespace math {

		template<int m, int n, class T = float>
		class Matrix {
		public:
			typedef T value_type;

			// OBS
			Matrix(T x, T y)										{ (*this)[0] = x; (*this)[1] = y;					}
			Matrix(T x, T y, T z)									{ (*this)[0] = x; (*this)[1] = y; (*this)[2] = z;	}
			Matrix(T x, T y, T z, T w)								{ (*this)[0] = x; (*this)[1] = y; (*this)[2] = z; (*this)[3] = w; }
			// END OBS

			Matrix(void)											{}
			Matrix(value_type *elem)								{ set(elem);										}
			Matrix(const Matrix<m, n, T> &m)						{ copy(m);											}

			value_type get(int i, int j = 0) const;
			const value_type &operator [](int pos) const			{ return elem[pos];									}
			value_type &operator [](int pos)						{ return elem[pos];									} 
			value_type operator ()(int i, int j) const				{ return elem[i + m * j];							}
			value_type &operator ()(int i, int j)					{ return elem[i + m * j];							}
			Matrix<m, n, T> &operator =(const Matrix<m, n, T> &m)	{ copy(m); return *this;							}
			const Matrix<m, 1, T> &getColumn(int i) const			{ return *(const Matrix<m, 1, T> *)&elem[i * m];	}

			void set(const value_type *elem);
			void set(value_type value, int i, int j = 0);
			void copy(const Matrix<m, n, T> &m);

			void identity(void);

		private:
			typename value_type elem[m * n];
		};

		typedef Matrix<2, 2> Matrix2x2;
		typedef Matrix<3, 3> Matrix3x3;
		typedef Matrix<4, 4> Matrix4x4;

		typedef Matrix<2, 1> Vector2;
		typedef Matrix<3, 1> Vector3;
		typedef Matrix<4, 1> Vector4;

		class Matrix_out_of_bounds {
		public:
			int rows, columns, i, j;

			template <int m, int n, class T>
			Matrix_out_of_bounds(const Matrix<m, n, T> &, int i, int j)
			: rows(m), columns(n), i(i), j(j)
			{}
		};

		/** Set Matrix to identity.
		 * Sets the diagonal elements to 1 (one) and the rest to 0 (zero).
		 */
		template<int m, int n, class T>
		inline void Matrix<m, n, T>::identity(void)
		{
			for (int i = 0; i < m; ++i) {
				for (int j = 0; j < n; j++) {
					elem[i * n + j] = (j == i) ? value_type(1) : value_type(0);
				}
			}
		}

		/** Set elements.
		 * Set elements to the values pointed to by elem
		 */
		template<int m, int n, class T>
		inline void Matrix<m, n, T>::set(const T *elem)
		{
			for (int pos = 0; pos < m * n; ++pos) this->elem[pos] = elem[pos];
		}

		/** Copy matrix.
		 * Copy all elements from the source matrix
		 */
		template<int m, int n, class T>
		inline void Matrix<m, n, T>::copy(const Matrix<m, n, T> &mat)
		{
			for (int pos = 0; pos < m * n; ++pos) this->elem[pos] = mat[pos];
		}

		/** Set element.
		 * Set element at position (i,j) to value.
		 */
		template<int m, int n, class T>
		inline void Matrix<m, n, T>::set(T value, int i, int j)
		{
			if (i && i < m && j && j < n)
				(*this)(i, j) = value;
			else
				throw Matrix_out_of_bounds(*this, i, j);
		}

		/** Get element.
		 * Get element at position (i, j).
		 */
		template<int m, int n, class T>
		inline T Matrix<m, n, T>::get(int i, int j) const
		{
			if (i && i < m && j && j < n) return (*this)(i, j);

			throw Matrix_out_of_bounds(*this, i, j);
		}

	}
}


#endif //_include_bleifrei_math_matrix_hpp_
