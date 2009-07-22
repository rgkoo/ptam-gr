/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Plane_hpp_
#define _include_bleifrei_Plane_hpp_ 


// includes
#include "../math/Matrix_op.hpp"
#include "../math/Matrix.hpp"

namespace bleifrei {
	namespace math {

		template<class T = float>
		class Plane {
		public:
			typedef T value_type;
			typedef Matrix<3, 1, T> Normal;
			typedef Matrix<3, 1, T> Point;

			Plane(const Normal &n, value_type dist) : n(n), dist(dist) {}
			Plane(const Normal &n, const Point &p) : n(n), dist(-p * n) {}
			Plane(const Point &p1, const Point &p2, const Point &p3);

			Normal &normal(void)						{ return n;				}
			value_type distance(const Point &p) const	{ return p * n + dist;	}

			value_type &a(void)							{ return n[0];			}
			value_type &b(void)							{ return n[1];			}
			value_type &c(void)							{ return n[2];			}
			value_type &d(void)							{ return dist;			}

			void normalize(void);
		private:
			Normal n;
			value_type dist;
		};

		template<class T>
		inline Plane<T>::Plane(const Point &p1, const Point &p2, const Point &p3)
		{
			Matrix<3, 1, float> temp;
			cross(n, (p2 - p1), (p3 - p1));
			math::normalize(n);
			dist = -(p1 * n);
		}

		template<class T>
		inline void Plane<T>::normalize(void)
		{
			T factor = 1 / length(normal);

			normal *= factor;
			dist *= factor;
		}

	}
}


#endif //_include_bleifrei_Plane_hpp_
