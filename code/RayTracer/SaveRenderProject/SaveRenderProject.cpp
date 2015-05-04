// SaveRenderProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <amp.h> 
using namespace concurrency;

struct RGBType{
public:
	int r, g, b;
};

void savebmp(const char *filename, int w, int h, int dpi, RGBType *data)
{/*
	FILE *f;
	int k = w*h;
	int s = 4 * k;
	int filesize = 54 + s;
	double factor = 39.375;
	int m = static_cast<int>(factor);
	int ppm = dpi*m;
	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);

	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb"); 
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	for (int i = 0; i < w*h; i++)
	{
		RGBType rgb = data[i];
		unsigned char color[3] = { rgb.b, rgb.g, rgb.r };
		fwrite(color, 1, 3, f);
	}
	fclose(f);*/
	return;
}

int _tmain(int argc, _TCHAR* argv[])
{ 
	int v[2][2] = { { 'A', 'P' }, { 'T', 'Q' } };

	extent<2> e(2, 2);
	
	array_view<int> av(11, v);
	parallel_for_each(av.extent, [=](index<1> idx) restrict(amp)
	{
		av[idx] += 1;
	});

	for (unsigned int i = 0; i < 11; i++)
		std::cout << static_cast<char>(av[i]);
}

	/*int dpi = 72;
	int width = 1000;
	int height = 1000;
	int n = width*height;

	RGBType *pixels = new RGBType[n];
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int thisone = y*width + x;
			pixels[thisone].r = 23;
			pixels[thisone].g = 222;
			pixels[thisone].b = 10;
		}
	}
	savebmp("render.bmp", 100, 100, 50, pixels);
	return 0;
}*/

