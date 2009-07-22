#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream.h>
#include "tex.h"
#include <math.h>
#include <gl\glext.h>


char tgaheader[12];									// TGA header
char tgaID[256];									// TGA header
TGA tga;



int exist(char filename[60],char ext[34])
{
	FILE *file;
	char fname[60];

	strcpy(fname,filename);
//	strcat(fname,".");
	//strcat(fname,ext);

	file = fopen(fname,"rt");
  
	if (file == NULL) {return 0;}  
    
	else {fclose(file); return 1; }
}


AUX_RGBImageRec *auxDIBImageLoad(const char *FileName)
{ 
	 return new AUX_RGBImageRec(FileName);
}


void AUX_RGBImageRec::convertBGRtoRGB()
{
	const DWORD BitmapLength = sizeX * sizeY * 3;
	byte Temp;  // not quick but it works  
	for(DWORD i=0; i< BitmapLength; i += 3) 
	{
	    Temp = data[i];
	    data[i] = data[i+2];
	    data[i+2] = Temp;
	    }
	}


AUX_RGBImageRec::AUX_RGBImageRec(const char *FileName): data(NULL), NoErrors(false)
{ 
 loadFile(FileName);
}


AUX_RGBImageRec::~AUX_RGBImageRec()
{
  if (data != NULL) delete data;
  data = NULL;
}


bool AUX_RGBImageRec::loadFile(const char* Filename)
{
	BITMAPINFO BMInfo;								// need the current OpenGL device contexts in order to make use of windows DIB utilities  
	const HDC gldc = wglGetCurrentDC();   			// a handle for the current OpenGL Device Contexts
					  								// assume there are errors until file is loaded successfully into memory  
	NoErrors = false;  								// release old data since this object could be used to load multiple Textures  
	if(data != NULL) delete data;					// windows needs this info to determine what header info we are looking for  
	BMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  // Get windows to determine color bit depth in the file for us  
	BMInfo.bmiHeader.biBitCount = 0;				// Get windows to open and load the BMP file and handle the messy decompression if the file is compressed  
													// assume perfect world and no errors in reading file, Ha Ha  
	HANDLE DIBHandle = LoadImage(0,Filename, IMAGE_BITMAP, 0, 0,LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);  // use windows to get header info of bitmap - assume no errors in header format 

	GetDIBits(gldc, (HBITMAP)DIBHandle, 0,0, NULL, &BMInfo, DIB_RGB_COLORS);
	sizeX = BMInfo.bmiHeader.biWidth;
	sizeY = BMInfo.bmiHeader.biHeight;				// change color depth to 24 bits (3 bytes (BGR) / pixel)  
	BMInfo.bmiHeader.biBitCount = 24;				// don't want the data compressed  
	BMInfo.bmiHeader.biCompression = BI_RGB;  
	const DWORD BitmapLength = sizeX * sizeY * 3;	// 3 bytes (BGR) per pixel (24bp)  
													// allocate enough memory to hold the pixel data in client memory  
	data = new byte[BitmapLength];					// Get windows to do the dirty work of converting the BMP into the format needed by OpenGL  
													// if file is already 24 bit color then this is a waste of time but makes for short code  
													// Get the actual Texel data from the BMP object  
	
	if (GetDIBits(gldc, (HBITMAP)DIBHandle, 0, sizeY, data, &BMInfo, DIB_RGB_COLORS)) 
	{
		NoErrors = true;
		convertBGRtoRGB();							// NOTE: BMP is in BGR format but OpenGL needs RGB unless you use GL_BGR_EXT
	}

	DeleteObject(DIBHandle);						// don't need the BMP Object anymore  
	return NoErrors;
}        




// Using auxDIBImageLoad's Own Error-Handler!
unsigned int Loadbmp(char filename[60],int filter)
{	
										// Load Bitmaps And Convert To Textures
	unsigned int texture;												// Status Indicator
	AUX_RGBImageRec *Image=NULL;									// Create Storage Space For The Texture
	char *alpha=NULL;



	// Load The Tile-Bitmap For Base-Texture
	if (Image=auxDIBImageLoad(filename)) {	
				
		if (filter==0) 
			{	
				glGenTextures(1, &texture); 
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data); 
			}

		if (filter==1) 
			{
				glGenTextures(1, &texture); 
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data); 
			}

		if (filter==2) 
			{
			
				glGenTextures(1, &texture); 
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT ,4);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data); 
			}



		if (filter==99) //only used by sky
			{
				glGenTextures(1, &texture); 
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data); 
			}

}
	
	
	
	if (Image) {													// If Texture Exists
		if (Image->data) delete Image->data;						// If Texture Image Exists
		delete Image;
		Image=NULL;
	}

	return texture;								// Return The Status
}


bool LoadUncompressedTGA(Texture *texture, char filename[60], FILE * fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's 
{	// TGA Loading code nehe.gamedev.net)

	
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{	
		
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}	

	texture->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga.header[4];										// Determine the bits per pixel
	tga.Width		= texture->width;										// Copy width into local structure						
	tga.Height		= texture->height;										// Copy height into local structure
	tga.Bpp			= texture->bpp;											// Copy BPP into local structure

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	// Make sure all information is valid
	{
		
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}


	if(tgaheader[0]!=0)
	{
		if(fread(&tgaID, tgaheader[0], 1, fTGA) == 0)					// Read TGA header
		{	
			
			if(fTGA != NULL)													// if file is still open
			{
				fclose(fTGA);													// Close it
			}
			return false;														// Return fail
		}
	}


	texture->type	= GL_RGB;											// Set Image type to GL_RGB

	if(texture->bpp == 32) texture->type	= GL_RGBA;




	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	if(texture->imageData == NULL)											// If no space was allocated
	{
		
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
		
		if(texture->imageData != NULL)										// If imagedata has data in it
		{
			free(texture->imageData);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2];

	}

	fclose(fTGA);															// Close file
	return true;															// Return success
}

bool LoadCompressedTGA(Texture *texture, char filename[60], FILE * fTGA)		// Load COMPRESSED TGAs
{ 

	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Attempt to read header
	{
		
		
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failed
	}

	texture->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga.header[4];										// Determine Bits Per Pixel
	tga.Width		= texture->width;										// Copy width to local structure
	tga.Height		= texture->height;										// Copy width to local structure
	tga.Bpp			= texture->bpp;											// Copy width to local structure

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	//Make sure all texture info is ok
	{
			
		
		if(fTGA != NULL)													// Check if file is open
		{
			fclose(fTGA);													// Ifit is, close it
		}
		return false;														// Return failed
	}

	if(tgaheader[0])
	{
		if(fread(&tgaID, tgaheader[0], 1, fTGA) == 0)					// Read TGA ID
		{										
				
			if(fTGA != NULL)													// if file is still open
			{
				fclose(fTGA);													// Close it
			}
			return false;														// Return failular
		}
	
	}


	texture->type	= GL_RGB;											// Set Image type to GL_RGB

	if(texture->bpp == 32) texture->type	= GL_RGBA;




	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	if(texture->imageData == NULL)											// If it wasnt allocated correctly..
	{
		
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	GLuint pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	GLuint currentpixel	= 0;												// Current pixel being read
	GLuint currentbyte	= 0;												// Current byte 
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

	while(currentpixel < pixelcount)
	{
		GLubyte chunkheader = 0;											// Storage for "chunk" header

		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)				// Read in the 1 byte header
		{
				
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(texture->imageData != NULL)									// If there is stored image data
			{
				free(texture->imageData);									// Delete image data
			}
			return false;													// Return failed
		}

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
				{
						
			
					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						free(colorbuffer);												// If so, delete it
					}

					if(texture->imageData != NULL)										// See if there is stored Image data
					{
						free(texture->imageData);										// If so, delete it too
					}

					return false;														// Return failed
				}
																						// write to memory
				texture->imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
					
					
					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(texture->imageData != NULL)										// If there is Image data
					{
						free(texture->imageData);										// delete it
					}

					return false;														// Return failed
				}
				
			}


		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
			{	
				
				
				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					free(colorbuffer);													// delete it
				}

				if(texture->imageData != NULL)											// If thereis image data
				{
					free(texture->imageData);											// delete it
				}

				return false;															// return failed
			}

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				texture->imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
					
				
					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(texture->imageData != NULL)										// If there is Image data
					{
						free(texture->imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
	}
	
	fclose(fTGA);																		// Close the file

	return true;																		// return success
}



unsigned int Loadtga(char filename[60],int filter)				// Loads A TGA File Into Memory
{    
	Texture texture;

	unsigned int id;
	
	FILE * fTGA;												// File pointer to texture file
	fTGA = fopen(filename, "rb");								// Open file for reading

	if(fTGA == NULL)											// If it didn't open....
	{
		
		return false;														// Exit function
	}

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)					// Attempt to read 12 byte header from file
	{
		
		if(fTGA != NULL)													// Check to seeiffile is still open
		{
			fclose(fTGA);													// If it is, close it
		}
		return false;														// Exit function
	}

/*	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		LoadUncompressedTGA(&texture, filename, fTGA);						// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		LoadCompressedTGA(&texture, filename, fTGA);							// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
		lg.out(CF_ERROR,"TGA file be type 2 or type 10 ");
		fclose(fTGA);
		return false;																// Exit function
	}*/

	if(tgaheader[2]==2)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		LoadUncompressedTGA(&texture, filename, fTGA);						// If so, jump to Uncompressed TGA loading code
	}
	else if(tgaheader[2]==10)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		LoadCompressedTGA(&texture, filename, fTGA);							// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
		fclose(fTGA);
		return false;																// Exit function
	}


	int compnents=3;

	if (texture.type==GL_RGBA)  compnents=4;

	// Build A Texture From The Data

		if (filter==0) 
			{	
				glGenTextures(1, &texture.texID); 
				glBindTexture(GL_TEXTURE_2D, texture.texID);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, texture.type, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData); 
			}

		if (filter==1) 
			{
				glGenTextures(1, &texture.texID); 
				glBindTexture(GL_TEXTURE_2D, texture.texID);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, texture.type, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData); 
			}

		if (filter==2) //trilinear with anisotropy
			{
				glGenTextures(1, &texture.texID); 
				glBindTexture(GL_TEXTURE_2D, texture.texID);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT ,4);			
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				gluBuild2DMipmaps(GL_TEXTURE_2D, compnents, texture.width, texture.height, texture.type, GL_UNSIGNED_BYTE, texture.imageData); 
			}


		if (filter==99) //only used by sky
			{
				glGenTextures(1, &texture.texID); 
				glBindTexture(GL_TEXTURE_2D, texture.texID);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
				gluBuild2DMipmaps(GL_TEXTURE_2D, compnents, texture.width, texture.height, texture.type, GL_UNSIGNED_BYTE, texture.imageData); 
			}




		if (filter==88) //hmap
			{	
				glGenTextures(1, &texture.texID); 
				glBindTexture(GL_TEXTURE_2D, texture.texID);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA_FLOAT32_ATI, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData); 
			}



		
		id=texture.texID;
	

		if(texture.imageData != NULL)										// If there is Image data
			{
				free(texture.imageData);										// delete it
			}

		
		
		return id;												// Texture Building Went Ok, Return True
}




unsigned int LoadGLTextures(char filename[60],int filter)
{	
										// Load Bitmaps And Convert To Textures
	unsigned int texture;
	int format=0;
	/*
	0 : bmp
	1 : tga
	*/
	
	char namebmp[60],nametga[60];

	strcpy(namebmp,filename);
	strcpy(nametga,filename);

	strcat(namebmp,".bmp");	
	strcat(nametga,".tga");


	if (exist(namebmp,"bmp")) {format=1;}
	if (exist(nametga,"tga")) {format=2;}
	if (format==0) {return 0;}


	
	//texture=Loadbmp(namebmp,filter);

	switch (format)
	{
	case 1:
			texture=Loadbmp(namebmp,filter);
			break;
	case 2:
			texture=Loadtga(nametga,filter);
			break;

	default:
			return 0;

	break;

	}


	return texture;								// Return The Status
}


