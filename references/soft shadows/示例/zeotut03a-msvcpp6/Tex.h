
#include <windows.h>									// Standard windows header
#include <stdio.h>										// Standard I/O header 
//#include <gl\gl.h>	


class AUX_RGBImageRec {
   void convertBGRtoRGB();
 public:
   byte *data;
   DWORD sizeX;
   DWORD sizeY;
   bool NoErrors;
   AUX_RGBImageRec(): NoErrors(false), data(NULL) {};
   AUX_RGBImageRec(const char *FileName);
   ~AUX_RGBImageRec();
   bool loadFile(const char *FileName);
   friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);
};

												// TGA image data




	typedef struct
	{
		char Header[12];									// TGA File Header
	} TGAHeader;


	typedef	struct									
	{
		unsigned char	* imageData;									// Image Data (Up To 32 Bits)
		unsigned int	bpp;											// Image Color Depth In Bits Per Pixel
		unsigned int	width;											// Image Width
		unsigned int	height;											// Image Height
		unsigned int	texID;											// Texture ID Used To Select A Texture
		unsigned int	type;											// Image Type (GL_RGB, GL_RGBA)
	} Texture;


	typedef struct
	{
		unsigned char		header[6];								// First 6 Useful Bytes From The Header
		unsigned int		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
		unsigned int		imageSize;								// Used To Store The Image Size When Setting Aside Ram
		unsigned int		temp;									// Temporary Variable
		unsigned int		type;	
		unsigned int		Height;									//Height of Image
		unsigned int		Width;									//Width ofImage
		unsigned int		Bpp;									// Bits Per Pixel
	} TGA;






unsigned int LoadGLTextures(char filename[60],int filter);

