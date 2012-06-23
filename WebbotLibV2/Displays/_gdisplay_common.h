/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
 *
 * Copyright (C) 2011 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *        File: _gdisplay_common.h
 *  Created on: 15 Jul 2011
 *      Author: Clive Webster
 *
 *  Defines common things for all graphical displays
 *
 */

#ifndef _GDISPLAY_COMMON_H_
#define _GDISPLAY_COMMON_H_

#include "_display_common.h"
#include "../color.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

typedef int16_t PIXEL;

// Define the data for a generic graphical display
typedef struct s_gdisplay {
	const struct /*PROGMEM*/ c_gdisplay*	gclass;		// Class info for this graphic display
	PIXEL	gxRes,gyRes;								// Graphics resolution X,y
	PIXEL	gx,gy;										// Graphics cursor X,y

	COLOR	fgnd;										// Foreground colour
	COLOR	bgnd;										// Background colour
} GDISPLAY;

#define MAKE_GDISPLAY(gclass) { \
	gclass, 0,0, 0,0 \
}

// Define the API for graphic display commands
typedef struct /*PROGMEM*/ c_gdisplay{
	void    (*shutdown)(GDISPLAY* gdisplay);			// Shutdown the display
	void    (*fgnd)(GDISPLAY* display);					// Set foreground colour
	void    (*bkgnd)(GDISPLAY* display);				// Set background colour
	void    (*line)(GDISPLAY* display, PIXEL x, PIXEL y);
														// Draw line in fgnd colour
	void    (*triangle)(GDISPLAY* display, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, PIXEL x3, PIXEL y3, boolean fill);
														// Draw triangle in fgnd colour
	void    (*circle)(GDISPLAY* display, PIXEL radius, boolean fill); // Draw circle in fgnd colour
	void    (*rectangle)(GDISPLAY* display, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill);
														// Draw rectangle in fgnd colour
	void 	(*replace)(GDISPLAY*, const COLOR *,const COLOR *, PIXEL, PIXEL,PIXEL,PIXEL);
	void 	(*plot)(GDISPLAY*, PIXEL,PIXEL);			// Draw pixel at x,y
	boolean (*read)(GDISPLAY*, PIXEL,PIXEL, COLOR*);	// Read pixel at x,y
	PIXEL 	(*drawChar)(GDISPLAY*, uint8_t, PIXEL,PIXEL,boolean);// Draw char at x,y
} GDISPLAY_CLASS;

#define MAKE_GDISPLAY_CLASS(shutdown, fgnd, bgnd, line, triangle, circle, rectangle, replace,plot,read,draw) { \
		shutdown, \
		fgnd, \
		bgnd, \
		line, \
		triangle, \
		circle, \
		rectangle, \
		replace,\
		plot, \
		read, \
		draw \
}

// Declare the public methods that can be called on a given display instance
// These pass the call onto the matching abstract method starting with an '_'
#define displayShutdown(display) 				_displayShutdown(&((display)->_gdisplay_))
#define displaySetPaper(display, color)			_displaySetPaper(&((display)->_gdisplay_), color)
#define displaySetPaperRGB(display, r,g,b)		_displaySetPaperRGB(&((display)->_gdisplay_), r,g,b)
#define displaySetInk(display, color)			_displaySetInk(&((display)->_gdisplay_), color)
#define displaySetInkRGB(display, r,g,b)		_displaySetInkRGB(&((display)->_gdisplay_), r,g,b)
#define displayMoveTo(display, x , y)			_displayMoveTo(&((display)->_gdisplay_), x, y)
#define displayMoveBy(display, x , y)			_displayMoveBy(&((display)->_gdisplay_), x, y)
#define displayPlotAt(display, x , y)			_displayPlotAt(&((display)->_gdisplay_), x, y)
#define displayPlotBy(display, x , y)			_displayPlotBy(&((display)->_gdisplay_), x, y)
#define displayReadAt(display, x , y, c)		_displayReadAt(&((display)->_gdisplay_), x, y, c)
#define displayReadBy(display, x , y, c)		_displayReadBy(&((display)->_gdisplay_), x, y, c)
#define displayLineTo(display, x , y)			_displayLineTo(&((display)->_gdisplay_), x, y)
#define displayLineBy(display, x , y)			_displayLineBy(&((display)->_gdisplay_), x, y)
#define displayGetXRes(display)					_displayGetXRes(&((display)->_gdisplay_))
#define displayGetYRes(display)					_displayGetYRes(&((display)->_gdisplay_))
#define displayGetXPos(display)					_displayGetXPos(&((display)->_gdisplay_))
#define displayGetYPos(display)					_displayGetYPos(&((display)->_gdisplay_))
#define displayTriangleAt(display,x1,y1,x2,y2,x3,y3,fill) _displayTriangleAt(&((display)->_gdisplay_),x1,y1,x2,y2,x3,y3,fill)
#define displayTriangleBy(display,x1,y1,x2,y2,x3,y3,fill) _displayTriangleBy(&((display)->_gdisplay_),x1,y1,x2,y2,x3,y3,fill)
#define displayRectangleAt(display,x1,y1,x2,y2,fill) _displayRectangleAt(&((display)->_gdisplay_),x1,y1,x2,y2,fill)
#define displayRectangleBy(display,x1,y1,x2,y2,fill) _displayRectangleBy(&((display)->_gdisplay_),x1,y1,x2,y2,fill)
#define displayCircle(display,radius,fill) 		_displayCircle(&((display)->_gdisplay_),radius,fill)
#define displayReplace(display,src,dst,x,y,w,h) _displayReplace(&((display)->_gdisplay_),src,dst, x,y,w,h)
#define displayChar(display,c,t) 				_displayChar(&((display)->_gdisplay_),c,t)

extern void      _displayShutdown(GDISPLAY* display);
extern void      _displayChar(GDISPLAY* display, uint8_t c, boolean transparent);
extern void      _displaySetPaper(GDISPLAY* display, const COLOR* color);
extern void      _displaySetInk(GDISPLAY* display, const COLOR* color);
extern void      _displaySetPaperRGB(GDISPLAY* display, uint8_t r,uint8_t g,uint8_t b);
extern void      _displaySetInkRGB(GDISPLAY* display, uint8_t r,uint8_t g,uint8_t b);
extern void      _displayMoveTo(GDISPLAY* display, PIXEL x, PIXEL y);
extern void      _displayLineTo(GDISPLAY* display, PIXEL x, PIXEL y);
extern void      _displayPlotAt(GDISPLAY* display, PIXEL x, PIXEL y);
extern void      _displayPlotBy(GDISPLAY* display, PIXEL x, PIXEL y);
extern boolean   _displayReadAt(GDISPLAY* display, PIXEL x, PIXEL y, COLOR* color);
extern boolean   _displayReadBy(GDISPLAY* display, PIXEL x, PIXEL y, COLOR* color);
extern void      _displayMoveBy(GDISPLAY* display, PIXEL x, PIXEL y);
extern void      _displayLineBy(GDISPLAY* display, PIXEL x, PIXEL y);
static __inline__ PIXEL  _displayGetXRes(const GDISPLAY* display){
	return display->gxRes;
}
static __inline__ PIXEL  _displayGetYRes(const GDISPLAY* display){
	return display->gyRes;
}
static __inline__ PIXEL  _displayGetXPos(const GDISPLAY* display){
	return display->gx;
}
static __inline__ PIXEL  _displayGetYPos(const GDISPLAY* display){
	return display->gy;
}
extern void      _displayTriangleAt(GDISPLAY* display, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, PIXEL x3, PIXEL y3, boolean fill);
extern void      _displayTriangleBy(GDISPLAY* display, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, PIXEL x3, PIXEL y3, boolean fill);
extern void      _displayRectangleAt(GDISPLAY* display, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill);
extern void      _displayRectangleBy(GDISPLAY* display, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill);
extern void      _displayCircle(GDISPLAY* display, PIXEL radius, boolean fill);
extern void 	 _displayReplace(GDISPLAY* display, const COLOR *src, const COLOR *dst, PIXEL x, PIXEL y, PIXEL w, PIXEL h);

//-------------- Widgets ---------------------------------
struct s_gdisplay_widget;

typedef struct c_gdisplay_widget{
	void (*draw)	(struct s_gdisplay_widget* widget);
	void (*update)	(struct s_gdisplay_widget* widget);
} GDISPLAY_WIDGET_CLASS;
#define MAKE_GDISPLAY_WIDGET_CLASS( draw, update){ draw,update}

// The abstract class which is the base for all widgets
typedef struct s_gdisplay_widget{
	const GDISPLAY_WIDGET_CLASS* clazz;	/* The implementation for this widget				*/
	GDISPLAY* 	_owner_;				/* The graphical display it is placed within		*/
	PIXEL		x, y;					/* Top left corner of the widget					*/
	PIXEL		w, h;					/* The size of the widget							*/
	PIXEL		padding;				/* The padding on each edge of widget				*/
	COLOR		bgnd;					/* The background colour of the widget				*/
	boolean		dirty;					/* Do the values need to be redrawn?				*/
	COLOR		content;				/* The colour of the variable content				*/
} GDISPLAY_WIDGET;
#define MAKE_GDISPLAY_WIDGET(cls,display, x,y,w,h, p, red,green,blue, rc,gc,bc){ \
		cls, &((display)->_gdisplay_), x,y,w,h, p, MAKE_COLOR_RGB(red,green,blue), TRUE, MAKE_COLOR_RGB(rc,gc,bc) \
}

#define widgetDraw(widget) 	 _widgetDraw(&((widget)->_widget_))
#define widgetUpdate(widget) _widgetUpdate(&((widget)->_widget_))
extern void _widgetDraw(GDISPLAY_WIDGET* widget);
extern void _widgetUpdate(GDISPLAY_WIDGET* widget);


// Each value in a widget has a min, max and current value
typedef struct s_widget_value {
	double min,max;
	double value;
	double old;
} WIDGET_VALUE;
#define MAKE_WIDGET_VALUE(min,max){ min, max, min, max }

#ifdef BUILDING_LIBRARY
extern boolean _widgetSetValue(WIDGET_VALUE* var, double val) __attribute__ ((warn_unused_result));
#endif

// -------------- Some widget implementations ----------------

//					-- DIAL --

// The dial widgets own properties
typedef struct p_dial_widget{
	COLOR		dial;			/* The background colour of the dial				*/
	COLOR		grad;			/* The foreground colour of the graduation marks	*/
	int16_t		minAng,maxAng;	/* The angular range								*/
	WIDGET_VALUE setting;
	double		gradVal;		/* Show graduation marks every 						*/
} DIAL_WIDGET_PROPERTIES;

// A dial widget
typedef struct s_dial_widget{
	GDISPLAY_WIDGET _widget_;
	DIAL_WIDGET_PROPERTIES _props_;
} DIAL_WIDGET;

extern const GDISPLAY_WIDGET_CLASS c_dial_widget;

#define MAKE_DIAL_WIDGET(display, x,y,w,h, p, br,bg,bb, nr,ng,nb, \
		dr,dg,db, gr,gg,gb, minAng, maxAng, minVal,maxVal, gradVal){ \
	MAKE_GDISPLAY_WIDGET(&c_dial_widget,display, x,y,w,h,p, br,bg,bb,  nr,ng,nb), \
	{MAKE_COLOR_RGB(dr,dg,db), \
	MAKE_COLOR_RGB(gr,gg,gb), \
	minAng,maxAng,			  \
	MAKE_WIDGET_VALUE(minVal,maxVal), \
	gradVal} \
}
extern void dialSetValue(DIAL_WIDGET* dial, double val);

//					-- NUMERIC --

// The number widgets own properties
typedef struct p_number_widget{
	COLOR		dial;			/* The background colour of the dial				*/
	WIDGET_VALUE setting;		/* The current value 								*/
	uint8_t		numDigits;		/* The number of digits to the left of '.'			*/
	uint8_t		numDecimals;	/* The number of digits to the right of '.'			*/
	boolean		first;			/* True if no content yet plotted					*/
} NUMBER_WIDGET_PROPERTIES;

// A number widget
typedef struct s_number_widget{
	GDISPLAY_WIDGET _widget_;
	NUMBER_WIDGET_PROPERTIES _props_;
} NUMBER_WIDGET;

extern const GDISPLAY_WIDGET_CLASS c_number_widget;

#define MAKE_NUMBER_WIDGET(display, x,y,w,h, p, br,bg,bb, nr,ng,nb, \
		dr,dg,db, digits, places, minVal, maxVal){ \
	MAKE_GDISPLAY_WIDGET(&c_number_widget,display, x,y,w,h,p, br,bg,bb,  nr,ng,nb), \
	{MAKE_COLOR_RGB(dr,dg,db), \
	MAKE_WIDGET_VALUE(minVal,maxVal), \
	digits,places,TRUE} \
}

extern void numberSetValue(NUMBER_WIDGET* numWidget, double val);

//					-- Thermometer --

// The thermometer widgets own properties
typedef struct p_thermo_widget{
	COLOR		border;			/* The colour of the border          				*/
	COLOR		grad;			/* The foreground colour of the graduation marks	*/
	WIDGET_VALUE setting;		/* The current value 								*/
	double		gradVal;		/* Show graduation marks every 						*/
} THERMO_WIDGET_PROPERTIES;

// A thermometer widget
typedef struct s_thermo_widget{
	GDISPLAY_WIDGET _widget_;
	THERMO_WIDGET_PROPERTIES _props_;
} THERMO_WIDGET;

extern const GDISPLAY_WIDGET_CLASS c_thermo_widget;

#define MAKE_THERMO_WIDGET(display, x,y,w,h, p, br,bg,bb, nr,ng,nb, \
		bdr,bdg,bdb, gr,gg,gb, minVal, maxVal, gradVal){ \
	MAKE_GDISPLAY_WIDGET(&c_thermo_widget,display, x,y,w,h,p, br,bg,bb,  nr,ng,nb), \
	{MAKE_COLOR_RGB(bdr,bdg,bdb), \
	MAKE_COLOR_RGB(gr,gg,gb), \
	MAKE_WIDGET_VALUE(minVal,maxVal), \
	gradVal} \
}

extern void thermoSetValue(THERMO_WIDGET* widget, double val);



#ifdef __cplusplus
}
// ================= C++ code =========================
class GraphicDisplay : public Display {
public:
	GraphicDisplay(GDISPLAY* cstr, DISPLAY* disp) : Display(disp)
	{
		m_cstr = cstr;
	}

	void shutdown(void) const{
		_displayShutdown(m_cstr);
	}

	void setPaper(uint8_t r, uint8_t g, uint8_t b) const{
		_displaySetPaperRGB(m_cstr, r,g,b);
	}
	void setPaper(Color& color) const{
		_displaySetPaper(m_cstr, color);
	}
	void setPaper(const COLOR* color) const{
		_displaySetPaper(m_cstr, color);
	}

	void setInk(uint8_t r, uint8_t g, uint8_t b) const{
		_displaySetInkRGB(m_cstr, r,g,b);
	}
	void setInk(Color& color) const{
		_displaySetInk(m_cstr, color);
	}
	void setInk(const COLOR* color) const{
		_displaySetInk(m_cstr, color);
	}

	void moveTo(PIXEL x , PIXEL y) const{
		_displayMoveTo(m_cstr, x, y);
	}
	void moveBy(PIXEL x , PIXEL y) const{
		_displayMoveBy(m_cstr, x, y);
	}

	void plotAt(PIXEL x , PIXEL y) const{
		_displayPlotAt(m_cstr, x, y);
	}
	void plotBy(PIXEL x , PIXEL y) const{
		_displayPlotBy(m_cstr, x, y);
	}
	boolean readAt(PIXEL x, PIXEL y, Color& color) const   __attribute__ ((warn_unused_result)){
		return _displayReadAt(m_cstr, x,y, color);
	}
	boolean readAt(PIXEL x, PIXEL y, COLOR* color) const   __attribute__ ((warn_unused_result)){
		return _displayReadAt(m_cstr, x,y, color);
	}
	boolean readBy(PIXEL x, PIXEL y, Color& color) const   __attribute__ ((warn_unused_result)){
		return _displayReadBy(m_cstr, x,y, color);
	}
	boolean readBy(PIXEL x, PIXEL y, COLOR* color) const   __attribute__ ((warn_unused_result)){
		return _displayReadBy(m_cstr, x,y, color);
	}

	void lineTo(PIXEL x , PIXEL y) const{
		_displayLineTo(m_cstr, x, y);
	}
	void lineBy(PIXEL x , PIXEL y) const{
		_displayLineBy(m_cstr, x, y);
	}

	PIXEL getXRes(void) const{
		return _displayGetXRes(m_cstr);
	}
	PIXEL getYRes(void) const{
		return _displayGetYRes(m_cstr);
	}

	PIXEL getXPos(void) const{
		return _displayGetXPos(m_cstr);
	}
	PIXEL getYPos(void) const{
		return _displayGetYPos(m_cstr);
	}

	void triangleAt(PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, PIXEL x3, PIXEL y3, boolean fill) const{
		_displayTriangleAt(m_cstr,x1,y1,x2,y2,x3,y3,fill);
	}
	void triangleBy(PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, PIXEL x3, PIXEL y3, boolean fill) const{
		_displayTriangleBy(m_cstr,x1,y1,x2,y2,x3,y3,fill);
	}

	void rectangleAt(PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill) const{
		_displayRectangleAt(m_cstr,x1,y1,x2,y2,fill);
	}
	void rectangleBy(PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill) const{
		_displayRectangleBy(m_cstr,x1,y1,x2,y2,fill);
	}

	void circle(PIXEL radius, boolean fill) const{
		_displayCircle(m_cstr,radius,fill);
	}

	void replace(const COLOR *src, const COLOR *dst, PIXEL x, PIXEL y, PIXEL w, PIXEL h) const{
		_displayReplace(m_cstr,src,dst,x,y,w,h);
	}
	void replace(Color& src, Color& dst, PIXEL x, PIXEL y, PIXEL w, PIXEL h) const{
		_displayReplace(m_cstr,src,dst,x,y,w,h);
	}

	void draw(char c, boolean transparent) const{
		_displayChar(m_cstr,(uint8_t)c,transparent);
	}
	void draw(const char* str, boolean transparent) const{
		char c;
		while(c = *str++, c){
			_displayChar(m_cstr,(uint8_t)c,transparent);
		}
	}

	operator GDISPLAY* (){
		return m_cstr;
	}

private:
	GDISPLAY* m_cstr;
};


class Widget : public GDISPLAY_WIDGET {
public:
	Widget(const GDISPLAY_WIDGET_CLASS* clazz,GraphicDisplay& display,PIXEL x,PIXEL y,PIXEL w, PIXEL h, PIXEL padding, Color& bkgnd, Color& content){
		this->clazz = clazz;
		this->_owner_ = display;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->padding = padding;
		setBackgroundColour(bkgnd);
		setContentColour(bkgnd);
	}

	void setBackgroundColour(Color& c){
		this->bgnd = *c;
	}

	void draw(void){
		_widgetDraw(this);
	}
	void update(void) {
		_widgetUpdate(this);
	}


protected:
	void setContentColour(Color& c){
		if(colorEquals(&this->content,c)==FALSE){
			this->content = *c;
			this->dirty = TRUE;
		}
	}

	void setValue(WIDGET_VALUE* wval, double val){
		this->dirty |= _widgetSetValue(wval, val);
	}

	double getValue(const WIDGET_VALUE* wval) const{
		return wval->value;
	}
	static void initValue(WIDGET_VALUE* wval, double min, double max){
		wval->min = min;
		wval->max = max;
		wval->value = min;
		wval->old = max;
	}
};

class DialWidget : public Widget, public DIAL_WIDGET_PROPERTIES {
public:
	DialWidget(GraphicDisplay& display,PIXEL x,PIXEL y,PIXEL w, PIXEL h, PIXEL padding, Color& bkgnd, Color& needle,
			Color& dial, Color& ticks,int16_t minAngle, int16_t maxAngle, double minVal, double maxVal, double tickEvery)
	: Widget(&c_dial_widget,display, x,y,w,h,padding, bkgnd,needle){
		setDialColour(dial);
		setTickColour(ticks);
		this->minAng = minAngle;
		this->maxAng = maxAngle;
		this->gradVal = tickEvery;
		Widget::initValue(&this->setting,minVal,maxVal);
	}

	void setNeedleColour(Color& c){
		Widget::setContentColour(c);
	}

	void setDialColour(Color& c){
		this->dial = *c;
	}

	void setTickColour(Color& c){
		this->grad = *c;
	}

	void setValue(double val) {
		Widget::setValue(&this->setting, val);
	}

	double getValue(void) const {
		return Widget::getValue(&this->setting);
	}
};



class NumberWidget : public Widget, public NUMBER_WIDGET_PROPERTIES{
public:
	NumberWidget(GraphicDisplay& display,PIXEL x,PIXEL y,PIXEL w, PIXEL h, PIXEL padding, Color& bkgnd, Color& digits,
				 Color& displayColour, uint8_t numDigits, uint8_t numDecimals, double minVal, double maxVal
			)
	: Widget(&c_number_widget,display, x,y,w,h,padding, bkgnd,digits){
		setDisplayColour(displayColour);
		this->numDigits = numDigits;
		this->numDecimals = numDecimals;
		Widget::initValue(&this->setting,minVal,maxVal);
	}

	void setDigitColour(Color& c){
		this->first = TRUE;
		Widget::setContentColour(c);
	}

	void setDisplayColour(Color& c){
		this->dial = *c;
	}

	void setValue(double val){
		Widget::setValue(&this->setting, val);
	}

	double getValue(void) const {
		return Widget::getValue(&this->setting);
	}

};


class ThermometerWidget : public Widget, public THERMO_WIDGET_PROPERTIES{
public:
	ThermometerWidget(GraphicDisplay& display,PIXEL x,PIXEL y,PIXEL w, PIXEL h, PIXEL padding, Color& bkgnd, Color& mercury,
			 Color& outline, Color& ticks, double minVal, double maxVal, double tickEvery
		)
		: Widget(&c_thermo_widget,display, x,y,w,h,padding, bkgnd,mercury){

		setOutlineColour(outline);
		setTickColour(ticks);
		this->gradVal = tickEvery;
		Widget::initValue(&this->setting,minVal,maxVal);
	}

	void setMercuryColour(Color& c){
		Widget::setContentColour(c);
	}
	void setOutlineColour(Color& c){
		this->border = *c;
	}
	void setTickColour(Color& c){
		this->grad = *c;
	}

	void setValue(double val) {
		Widget::setValue(&this->setting, val);
	}

	double getValue(void) const {
		return Widget::getValue(&this->setting);
	}

};


#endif
#endif /* _GDISPLAY_COMMON_H_ */
