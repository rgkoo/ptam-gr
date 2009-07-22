
#ifndef _include_bleifrei_misc_hpp_
#define _include_bleifrei_misc_hpp_ 


inline float random_float(float min, float max)
{
	return (max - min) * (rand() & 0x7FFF) / 0x8000 + min;
}


#endif //_include_bleifrei_misc_hpp_
