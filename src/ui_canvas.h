//------------------------------------------------------------------------
//  EDITING CANVAS
//------------------------------------------------------------------------
//
//  Eureka DOOM Editor
//
//  Copyright (C) 2008-2019 Andrew Apted
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

#ifndef __EUREKA_UI_CANVAS_H__
#define __EUREKA_UI_CANVAS_H__

#ifndef NO_OPENGL
#include <FL/Fl_Gl_Window.H>
#endif

#include "m_events.h"
#include "m_select.h"
#include "e_objects.h"
#include "r_grid.h"


class Img_c;


#ifdef NO_OPENGL
class UI_Canvas : public Fl_Widget
#else
class UI_Canvas : public Fl_Gl_Window
#endif
{
private:
	// this mirrors edit.highlight, used to detect real changes
	// [ to prevent unnecessary redraws ]
	Objid highlight;

	// this mirrors edit.split_line (etc), used to detect changes
	int split_ld;
	double split_x;
	double split_y;

	// drawing state only
	double map_lx, map_ly;
	double map_hx, map_hy;

	bitvec_c seen_sectors;

	// a copy of x() and y() for software renderer, 0 for OpenGL
	int xx, yy;

public:
	UI_Canvas(int X, int Y, int W, int H, const char *label = NULL);
	virtual ~UI_Canvas();

public:
	// FLTK virtual method for handling input events.
	int handle(int event);

	// FLTK virtual method for resizing.
	void resize(int X, int Y, int W, int H);

	// delete any OpenGL context which is assigned to the window.
	// call this whenever OpenGL textures need to be reloaded.
	void DeleteContext();

	void DrawEverything();

	void HighlightSet(Objid& obj);
	void HighlightForget();

	void SplitLineSet(int ld, double new_x, double new_y);
	void SplitLineForget();

	void DrawSelection(selection_c *list);
	void DrawSectorSelection(selection_c *list, double dx, double dy);
	void DrawHighlight(int objtype, int objnum,
	                   bool skip_lines = false, double dx=0, double dy=0);
	void DrawHighlightTransform(int objtype, int objnum);
	void DrawTagged(int objtype, int objnum);

	// returns true if ok, false if box was very small
	bool SelboxGet(double& x1, double& y1, double& x2, double& y2);

	void DragDelta(double *dx, double *dy);

	void PointerPos(bool in_event = false);

	// return -1 if too small, 0 is OK, 1 is too big to fit
	int ApproxBoxSize(int mx1, int my1, int mx2, int my2);

private:
	// FLTK virtual method for drawing
	void draw();

	void DrawMap();

	void DrawGrid_Dotty();
	void DrawGrid_Normal();
	void DrawAxes(Fl_Color col);

	void DrawMapBounds();
	void DrawVertices();
	void DrawLinedefs();
	void DrawThings();
	void DrawThingBodies();
	void DrawThingSprites();

	void DrawMapLine(double map_x1, double map_y1, double map_x2, double map_y2);
	void DrawMapVector(double map_x1, double map_y1, double map_x2, double map_y2);
	void DrawMapArrow(double map_x1, double map_y1, int r, int angle);

	void DrawKnobbyLine(double map_x1, double map_y1, double map_x2, double map_y2, bool reverse = false);
	void DrawSplitLine(double map_x1, double map_y1, double map_x2, double map_y2);
	void DrawSplitPoint(double map_x, double map_y);
	void DrawVertex(double map_x, double map_y, int r);
	void DrawThing(double map_x, double map_y, int r, int angle, bool big_arrow);
	void DrawCamera();

	void DrawLineNumber(int mx1, int my1, int mx2, int my2, int side, int n);
	void DrawSectorNum(int mx1, int my1, int mx2, int my2, int side, int n);
	void DrawNumber(int x, int y, int num);
	void DrawCurrentLine();

	void RenderSprite(int sx, int sy, float scale, Img_c *img);
	void RenderSector(int num);

	void SelboxDraw();

	// calc screen-space normal of a line
	int NORMALX(int len, double dx, double dy);
	int NORMALY(int len, double dx, double dy);

#ifdef NO_OPENGL
	// convert screen coordinates to map coordinates
	inline double MAPX(int sx) const { return grid.orig_x + (sx - w()/2 - x()) / grid.Scale; }
	inline double MAPY(int sy) const { return grid.orig_y + (h()/2 - sy + y()) / grid.Scale; }

	// convert map coordinates to screen coordinates
	inline int SCREENX(double mx) const { return (x() + w()/2 + I_ROUND((mx - grid.orig_x) * grid.Scale)); }
	inline int SCREENY(double my) const { return (y() + h()/2 + I_ROUND((grid.orig_y - my) * grid.Scale)); }
#else
	// convert GL coordinates to map coordinates
	inline double MAPX(int sx) const { return grid.orig_x + (sx - w()/2) / grid.Scale; }
	inline double MAPY(int sy) const { return grid.orig_y + (sy - h()/2) / grid.Scale; }

	// convert map coordinates to GL coordinates
	inline int SCREENX(double mx) const { return (w()/2 + I_ROUND((mx - grid.orig_x) * grid.Scale)); }
	inline int SCREENY(double my) const { return (h()/2 + I_ROUND((my - grid.orig_y) * grid.Scale)); }
#endif

	inline bool Vis(double x, double y, int r) const
	{
		return (x+r >= map_lx) && (x-r <= map_hx) &&
		       (y+r >= map_ly) && (y-r <= map_hy);
	}
	inline bool Vis(double x1, double y1, double x2, double y2) const
	{
		return (x2 >= map_lx) && (x1 <= map_hx) &&
		       (y2 >= map_ly) && (y1 <= map_hy);
	}

#ifdef NO_OPENGL
	void gl_color(Fl_Color c);
	void gl_rectf(int rx, int ry, int rw, int rh);
	void raw_pixel(int rx, int ry);

	void PrepareToDraw();
	int  Calc_Outcode(int x, int y);
	void Blit();
#endif

	void gl_line(int x1, int y1, int x2, int y2);
	void gl_line_width(int w);
	void gl_number_string(const char *s, int x, int y);
	void gl_image_part(int rx, int ry, Img_c *img, int ix, int iy, int iw, int ih);
};


#endif  /* __EUREKA_UI_CANVAS_H__ */

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
