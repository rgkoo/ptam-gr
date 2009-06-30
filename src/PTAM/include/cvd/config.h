#ifndef CVD_CONFIG_H
 #define CVD_CONFIG_H
 #define CVD_ARCH_LITTLE_ENDIAN 1
 #define CVD_DISABLE_FENV_H
 #ifndef CVD_DISABLE_CPU_i686
     #define CVD_HAVE_CPU_i686 1
 #endif
 #ifndef CVD_DISABLE_FENV_H
     #define CVD_HAVE_FENV_H 1
 #endif
 #ifndef CVD_DISABLE_FFMPEG
     #define CVD_HAVE_FFMPEG 1
 #endif
 #ifndef CVD_DISABLE_LAPACK
     #define CVD_HAVE_LAPACK 1
 #endif
 #ifndef CVD_DISABLE_MEMALIGN
     #define CVD_HAVE_MEMALIGN 1
 #endif
 #ifndef CVD_DISABLE_PTHREAD
     #define CVD_HAVE_PTHREAD 1
 #endif
 #ifndef CVD_DISABLE_PTHREAD_YIELD
     #define CVD_HAVE_PTHREAD_YIELD 1
 #endif
 #ifndef CVD_DISABLE_TOON
     #define CVD_HAVE_TOON 1
 #endif
 #ifndef CVD_DISABLE_V4L1BUFFER
     #define CVD_HAVE_V4L1BUFFER 1
 #endif
 #ifndef CVD_DISABLE_V4L2BUFFER
     #define CVD_HAVE_V4L2BUFFER 1
 #endif
 #ifndef CVD_DISABLE_VIDEODISPLAY
     #define CVD_HAVE_VIDEODISPLAY 1
 #endif
 #ifndef CVD_DISABLE_JPEG
     #define CVD_IMAGE_HAVE_JPEG JPEG,
 #endif
 #ifndef CVD_DISABLE_PNG
     #define CVD_IMAGE_HAVE_PNG PNG,
 #endif
 #ifndef CVD_DISABLE_TIFF
     #define CVD_IMAGE_HAVE_TIFF TIFF,
 #endif
 #ifndef CVD_DISABLE_TIFF_ORIENTED
     #define CVD_INTERNAL_HAVE_TIFF_ORIENTED TIFF,
 #endif
 #define CVD_INTERNAL_JPEG_BUFFER_SIZE 1
 #define CVD_KERNEL_MAJOR 2
 #define CVD_KERNEL_MINOR 6
 #define CVD_KERNEL_RELEASE 9
 #define CVD_MAJOR_VERSION 0
 #define CVD_MINOR_VERSION 7
 #endif
