
// includes
#include <fstream>

#include "bleifrei/img/TGAImage.hpp"
#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/tex/Texture.hpp"

using namespace bleifrei;
using namespace std;

template< typename T >
void swap( T* a, T* b )
{
	T Temp;
	Temp = *a;
	*a = *b;
	*b = Temp;
}

//-----------------------------------------------------------------------------
// Name: Create2xColorLookupTexture()
// Desc: Creates a 1D texture that is used to convert a 2x coverage value to
//       a split color value.
//-----------------------------------------------------------------------------
tex::Texture1 create2xColorLookupTexture(void)
{
    const char* szSplitFilename = "media\\temptextures\\2xColorLookup.tga";
    const unsigned int SPLIT_TEXTURE_SIZE  = 2048;

/*	try {
		img::TGAImage lookup(std::ifstream(szSplitFilename, std::ios::binary));

		return tex::Texture1(lookup);
	}
	catch (...) {*/
		img::Image lookup(SPLIT_TEXTURE_SIZE, 1, 32);	// TODO: try/catch out_of_mem...

        unsigned int PixelSize = 4;

		platform::byte *pBits = lookup.getData();
		
	    for(unsigned int i = 0; i < SPLIT_TEXTURE_SIZE; i++)
	    {
            float fCoverage = ((float(i) / SPLIT_TEXTURE_SIZE) + 1.0f / (2.0f * SPLIT_TEXTURE_SIZE)) / 2.0f;

            float fCoverageScaled = fCoverage * 4095.0f;

			platform::dword dwCoverage;
            if(fCoverageScaled - floorf(fCoverageScaled) > 0.5f)
				dwCoverage = platform::dword(fCoverageScaled + 1.0f);
			else
				dwCoverage = platform::dword(fCoverageScaled);

            platform::byte bSplitR = platform::byte(dwCoverage >> 9);
            platform::byte bSplitG = platform::byte((dwCoverage >> 6) & 0x07);
            platform::byte bSplitB = platform::byte((dwCoverage >> 3) & 0x07);
            platform::byte bSplitA = platform::byte(dwCoverage & 0x07);

            platform::dword dwPixel = i * PixelSize;
/*			pBits[dwPixel + 0] = bSplitB;
			pBits[dwPixel + 1] = bSplitG;
			pBits[dwPixel + 2] = bSplitR;
			pBits[dwPixel + 3] = bSplitA;*/
			pBits[dwPixel + 0] = bSplitR;
			pBits[dwPixel + 1] = bSplitG;
			pBits[dwPixel + 2] = bSplitB;
			pBits[dwPixel + 3] = bSplitA;
	    }

		((img::TGAImage *)&lookup)->save(std::ofstream(szSplitFilename, std::ios::binary));

		return tex::Texture1(lookup);
//	}
}

//-----------------------------------------------------------------------------
// Name: CreateSpericalAreaTexture()
// Desc: Creates a 2D texture that is looks up the area of a "pie slice" defined
//       by two angles on a unit sphere
//-----------------------------------------------------------------------------
tex::Texture2 createSphericalAreaTexture(void)
{
    const char* szFilename = "media\\temptextures\\SphericalAreaTexture.tga";
    const unsigned int TEXTURE_SIZE  = 1024;

/*	try {
		img::TGAImage lookup(std::ifstream(szFilename, std::ios::binary));

		return tex::Texture2(lookup);
	}
	catch (...) {*/
		unsigned int PixelSize = 2;

		img::Image lookup(TEXTURE_SIZE, TEXTURE_SIZE, 16);

		platform::byte *pBits = (platform::byte*)lookup.getData();
		
	    for(unsigned int i = 0; i < TEXTURE_SIZE; i++)
	    {
            for(unsigned int j = 0; j < TEXTURE_SIZE; j++)
            {
                float fTheta1 = float(i) / TEXTURE_SIZE + 1.0f / TEXTURE_SIZE;
                float fTheta2 = float(j) / TEXTURE_SIZE + 1.0f / TEXTURE_SIZE;

                float fDelta = fabsf(fTheta1 - fTheta2);
                if(fDelta > 0.5f)
                    fDelta = 1.0f - fDelta;
                
                fDelta *= 2;

                float fDeltaScaled = fDelta * 65535.0f;
				platform::word wDeltaScaled;
                
                if(fDeltaScaled - floorf(fDeltaScaled) > 0.5f)
				    wDeltaScaled = platform::word( fDeltaScaled + 1.0f);
			    else
				    wDeltaScaled = platform::word( fDeltaScaled );

				//wDeltaScaled = (platform::dword(fDeltaScaled) << 24) + (platform::word(fDeltaScaled) << 16) + (platform::word(fDeltaScaled) << 8) + platform::word(fDeltaScaled);
				wDeltaScaled = platform::word(fDeltaScaled);

                platform::dword dwPixel = i * lookup.getBytesPerLine() + j * PixelSize;
                *((platform::word*)(pBits + dwPixel)) = wDeltaScaled;
            }
	    }

		//lookup.flip();
		((img::TGAImage *)&lookup)->save(std::ofstream(szFilename, std::ios::binary));
		
		return tex::Texture2(lookup);
  //  }
}

#define MAX_GENERATED_SEGMENTS 25
void ClipPolyToRect2D(math::Vector2 *pPoly, unsigned int *puiNumSegments )
{
	math::Vector2 TempPoly[MAX_GENERATED_SEGMENTS];
	
	math::Vector2 *pIn = pPoly;;
	math::Vector2 *pOut = &TempPoly[0];
	
	unsigned int j;
	unsigned int uiNext;
	math::Vector2 DV;
	float t;

	// 0 x and 0 y axis
	for(unsigned int p = 0; p < 2; p++)
	{
		j = 0;
		for(unsigned int i = 0; i < *puiNumSegments; i++)
		{
			// point behind plane
			if(pIn[i][p] >= 0)
				pOut[j++] = pIn[i];

			uiNext = (i == *puiNumSegments - 1) ? 0 : i+1;
			DV = pIn[uiNext] - pIn[i];
			if(DV[p] != 0.0f)
			{
				t = (0 - pIn[i][p]) / DV[p];
				if(t > 0.0f && t < 1.0f) 
					pOut[j++] = pIn[i] + DV*t;
			}
		}
		swap(&pIn, &pOut);
		*puiNumSegments = j;

	}

	// 1 x and 1 y axis
	for(unsigned int p = 0; p < 2; p++)
	{
		j = 0;
		for(unsigned int i = 0; i < *puiNumSegments; i++)
		{
			// point behind plane
			if(pIn[i][p] <= 1.0f)
				pOut[j++] = pIn[i];

			uiNext = (i == *puiNumSegments - 1) ? 0 : i+1;
			DV = pIn[uiNext] - pIn[i];
			if(DV[p] != 0.0f)
			{
				t = (1.0f - pIn[i][p]) / DV[p];
				if(t > 0.0f && t < 1.0f) 
					pOut[j++] = pIn[i] + DV*t;
			}
		}
		swap(&pIn, &pOut);
		*puiNumSegments = j;
	}
}

float CalcConvexPolyArea2D(math::Vector2 *pPoly, unsigned int uiNumSegments)
{
	float fArea = 0.0f;
	math::Vector2 V0, V1;
	for (unsigned int i = 2; i < uiNumSegments; i++)
	{
		V0 = pPoly[i] - pPoly[0];
		V1 = pPoly[i-1] - pPoly[0];
		
		fArea += fabsf(V0[0] * V1[1] - V0[1] * V1[0]);
	}

	return fArea / 2.0f;
}

//-----------------------------------------------------------------------------
// Name: CreateAtan2Cubemap()
// Desc: Create a cubemap the encodes atan2(x,y), z is not used.
//-----------------------------------------------------------------------------
tex::TextureCube createAtan2Cubemap(void)
{
	const unsigned int TEXTURE_SIZE = 128;

	img::Image neg_x(TEXTURE_SIZE, TEXTURE_SIZE, 16);
	img::Image pos_x(TEXTURE_SIZE, TEXTURE_SIZE, 16);
	img::Image neg_y(TEXTURE_SIZE, TEXTURE_SIZE, 16);
	img::Image pos_y(TEXTURE_SIZE, TEXTURE_SIZE, 16);
	img::Image neg_z(TEXTURE_SIZE, TEXTURE_SIZE, 16);
	img::Image pos_z(TEXTURE_SIZE, TEXTURE_SIZE, 16);
	
	img::Image *cube_faces[] = {
		&neg_x,
		&pos_x,
		&neg_y,
		&pos_y,
		&neg_z,
		&pos_z
	};

	for (int face = 0; face < 6; face++) {
		platform::word* pBits = (platform::word *)cube_faces[face]->getData();

        for (platform::dword v = 0; v < TEXTURE_SIZE; v++) {
   			float fv = (float(v) / TEXTURE_SIZE + 1.0f / (2.0f * TEXTURE_SIZE)) * 2.0f - 1.0f;

            for (platform::dword u = 0; u < TEXTURE_SIZE; u++) {
	 			float fu = (float(u) / TEXTURE_SIZE + 1.0f / (2.0f * TEXTURE_SIZE)) * 2.0f - 1.0f;
	 			
	 			float x, y;

				// Calc the normal for this texel
                switch(face) {
                    case 1:    // +x
                        x = 1.0;
                        y = -fv;
                        break;
                        
                    case 0:    // -x
                        x = -1.0;
                        y = -fv;
                        break;
                        
                    case 3:    // y
                        x = fu;
                        y = 1.0;
                        break;
                        
                    case 2:    // -y
                        x = fu;
                        y = -1.0;
                        break;
                        
                    case 5:    // +z (not used)
                        x = fu;
                        y = -fv;
                        break;
                        
                    case 4:    // -z (not used)
                        x = -fu;
                        y = -fv;
                        break;
                }
                // Calc the normal for this texel
                /*switch(face) {
                    case 1:    // +x
                        x = 1.0;
                        y = -fv;
                        break;
                        
                    case 0:    // -x
                        x = -1.0;
                        y = -fv;
                        break;
                        
                    case 3:    // y
                        x = fu;
                        y = 1.0;
                        break;
                        
                    case 2:    // -y
                        x = fu;
                        y = -1.0;
                        break;
                        
                    case 5:    // +z (not used)
                        x = fu;
                        y = -fv;
                        break;
                        
                    case 4:    // -z (not used)
                        x = -fu;
                        y = -fv;
                        break;
                }*/

                // Store atan2f(y, x)
				float fValue = atan2f(y, x);		// -PI to PI.
				
				const float fPI = 3.1415926535f;
				
				if (fValue < 0.0f)
					fValue += 2.0f * fPI;
				
				fValue = (fValue / (2.0f * fPI));
				
				pBits[u] = (platform::word)(fValue * 65535.0f); //255.0f);
				//pBits[u] = (platform::dword(fValue * 255.0f) << 24) + (platform::word(fValue * 255.0f) << 16) + (platform::word(fValue * 255.0f) << 8) + platform::word(fValue * 255.0f); //65535.0f);
            }
            
			pBits += cube_faces[face]->getBytesPerLine() / sizeof(platform::word);
        }
	}
	
	((img::TGAImage *)&neg_x)->save(std::ofstream("media\\temptextures\\Atan2Cubemap_negx.tga", std::ios::binary));
	((img::TGAImage *)&pos_x)->save(std::ofstream("media\\temptextures\\Atan2Cubemap_posx.tga", std::ios::binary));
	((img::TGAImage *)&neg_y)->save(std::ofstream("media\\temptextures\\Atan2Cubemap_negy.tga", std::ios::binary));
	((img::TGAImage *)&pos_y)->save(std::ofstream("media\\temptextures\\Atan2Cubemap_posy.tga", std::ios::binary));
	((img::TGAImage *)&neg_z)->save(std::ofstream("media\\temptextures\\Atan2Cubemap_negz.tga", std::ios::binary));
	((img::TGAImage *)&pos_z)->save(std::ofstream("media\\temptextures\\Atan2Cubemap_posz.tga", std::ios::binary));

/*	neg_x.flip();
	pos_x.flip();
	neg_y.flip();
	pos_y.flip();
	neg_z.flip();
	pos_z.flip();*/

	return tex::TextureCube(neg_x, pos_x, neg_y, pos_y, neg_z, pos_z);//neg_x, pos_x, neg_y, pos_y, neg_z, pos_z);
}

//-----------------------------------------------------------------------------
// Name: CreateRectAreaTexture()
// Desc: Create a 2D texture that gives the area in the rectangle between two 
//       vectors from the center of the rectangle defined two angles.
//-----------------------------------------------------------------------------
tex::Texture2 createRectAreaTexture(void)
{
	const unsigned int TEXTURE_SIZE = 256;

	img::Image lookup(TEXTURE_SIZE, TEXTURE_SIZE, 16);

	math::Vector2 *Poly = new math::Vector2[50];

	platform::word *pBits = (platform::word *)lookup.getData();

	math::Vector2 vCenter(0.5f, 0.5f);

	const float fEpsilon = -0.999848f;	// cos(179 deg)
	const float fPI = 3.1415926535f;

	for(unsigned int y = 0; y < TEXTURE_SIZE; y++) {
		float theta1 = (float(y) / TEXTURE_SIZE + 1.0f / (2.0f * TEXTURE_SIZE)) * 2.0f * fPI;
		math::Vector2 v1(cosf(theta1), sinf(theta1));

		for(unsigned int x = 0; x < TEXTURE_SIZE; x++) {
			float theta2 = (float(x) / TEXTURE_SIZE + 1.0f / (2.0f * TEXTURE_SIZE)) * 2.0f * fPI;
			math::Vector2 v2(cosf(theta2), sinf(theta2));

			float fCoverage;
			if (v1 * v2 < fEpsilon) {
				fCoverage = 0.5f;
			}
			else {
				unsigned int uiNumSegs = 3;

				Poly[0] = vCenter;
				Poly[1] = vCenter + v1*1000.0f;
				Poly[2] = vCenter + v2*1000.0f;
				
				ClipPolyToRect2D( Poly, &uiNumSegs );
				fCoverage = CalcConvexPolyArea2D( Poly, uiNumSegs );
			}

			pBits[x] = platform::word(fCoverage * 65535.0f);
		}

		pBits += lookup.getBytesPerLine() / sizeof(platform::word);
	}

	delete[] Poly;

	((img::TGAImage *)&lookup)->save(std::ofstream("media\\temptextures\\RectArea.tga", std::ios::binary));

	return tex::Texture2(lookup);
}

//-----------------------------------------------------------------------------
// Name: CreateColorLookupTexture()
// Desc: Creates a 1D texture that is used to convert a coverage value to
//       a split color value.
//-----------------------------------------------------------------------------
tex::Texture1 createColorLookupTexture(void)
{
/*	const unsigned int TEXTURE_SIZE = 1024;

	img::Image lookup(TEXTURE_SIZE, 1, 32);

	platform::dword *pBits = (platform::dword *)lookup.getData();
		
	for(unsigned int x = 0; x < TEXTURE_SIZE; x++) {
		float fValue = float(x) / TEXTURE_SIZE + 1.0f / (2.0f * TEXTURE_SIZE);

		float fValueScaled = fValue * 4095.0f;

		// Round
        platform::dword dwValue;
        if (fValueScaled - floorf(fValueScaled) > 0.5f)
			dwValue = platform::dword(fValueScaled + 1.0f);
		else
			dwValue = platform::dword(fValueScaled);

        platform::byte bSplitR = platform::byte(dwValue >> 9);
        platform::byte bSplitG = platform::byte((dwValue >> 6) & 0x07);
        platform::byte bSplitB = platform::byte((dwValue >> 3) & 0x07);
        platform::byte bSplitA = platform::byte(dwValue & 0x07);

		pBits[x] = (bSplitA << 24) | (bSplitB << 16) | (bSplitG << 8) | bSplitR;
		//pBits[x] = (bSplitA << 24) | (bSplitR << 16) | (bSplitG << 8) | bSplitB;
		//pBits[x] = (bSplitR << 24) | (bSplitG << 16) | (bSplitB << 8) | bSplitA;
	}

	((img::TGAImage *)&lookup)->save(std::ofstream("media\\temptextures\\ColorLookup.tga", std::ios::binary));

	return tex::Texture1(lookup);
	*/
    const char* szSplitFilename = "media\\temptextures\\ColorLookup.tga";
    const unsigned int SPLIT_TEXTURE_SIZE  = 1024;

	img::Image lookup(SPLIT_TEXTURE_SIZE, 1, 32);	// TODO: try/catch out_of_mem...

    unsigned int PixelSize = 4;

	platform::byte *pBits = lookup.getData();
	
	for(unsigned int i = 0; i < SPLIT_TEXTURE_SIZE; i++)
	{
		float fValue = float(i) / SPLIT_TEXTURE_SIZE + 1.0f / (2.0f * SPLIT_TEXTURE_SIZE);

		float fValueScaled = fValue * 4095.0f;

		// Round
        platform::dword dwValue;
        if (fValueScaled - floorf(fValueScaled) > 0.5f)
			dwValue = platform::dword(fValueScaled + 1.0f);
		else
			dwValue = platform::dword(fValueScaled);

        platform::byte bSplitR = platform::byte(dwValue >> 9);
        platform::byte bSplitG = platform::byte((dwValue >> 6) & 0x07);
        platform::byte bSplitB = platform::byte((dwValue >> 3) & 0x07);
        platform::byte bSplitA = platform::byte(dwValue & 0x07);

        platform::dword dwPixel = i * PixelSize;
		pBits[dwPixel + 0] = bSplitR;
		pBits[dwPixel + 1] = bSplitG;
		pBits[dwPixel + 2] = bSplitB;
		pBits[dwPixel + 3] = bSplitA;
	}

	((img::TGAImage *)&lookup)->save(std::ofstream(szSplitFilename, std::ios::binary));

	return tex::Texture1(lookup);
}
