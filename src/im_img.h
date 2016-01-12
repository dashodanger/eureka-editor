//------------------------------------------------------------------------
//  IMAGES
//------------------------------------------------------------------------
//
//  Eureka DOOM Editor
//
//  Copyright (C) 2001-2016 Andrew Apted
//  Copyright (C) 1997-2003 André Majorel et al
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------
//
//  Based on Yadex which incorporated code from DEU 5.21 that was put
//  in the public domain in 1994 by Raphaël Quinet and Brendon Wyber.
//
//------------------------------------------------------------------------

#ifndef __EUREKA_IM_IMG_H__
#define __EUREKA_IM_IMG_H__

#include "im_color.h"

typedef byte  img_pixel_t;


// the color number used to represent transparent pixels in an Img_c.
const img_pixel_t TRANS_PIXEL = 255;


class Img_c
{
private:
	img_pixel_t *pixels;

	int  w;  // Width
	int  h;  // Height

public:
	 Img_c();
	 Img_c(int width, int height, bool _dummy = false);
	~Img_c();

	bool is_null() const
	{
		return (! pixels);
	}
	
	int width() const
	{
		return w;
	}

	int height() const
	{
		return h;
	}

	// read access
	const img_pixel_t *buf() const;

	// read/write access
	img_pixel_t *wbuf();

public:
	// set all pixels to TRANS_PIXEL
	void clear();

	void resize(int new_width, int new_height);

	Img_c * spectrify() const;

	Img_c * scale_img(double scale) const;

	Img_c * color_remap(int src1, int src2, int targ1, int targ2) const;

	bool has_transparent() const;

private:
	Img_c            (const Img_c&);  // Too lazy to implement it
	Img_c& operator= (const Img_c&);  // Too lazy to implement it
};


void IM_ResetDummyTextures();

Img_c * IM_MissingTex();
Img_c * IM_UnknownTex();
Img_c * IM_UnknownFlat();

Img_c * IM_CreateFromText(int W, int H, const char **text, const rgb_color_t *palette, int pal_size);


#endif  /* __EUREKA_IM_IMG_H__*/

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
