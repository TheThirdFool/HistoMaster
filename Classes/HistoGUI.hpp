#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <cmath> 
	
//#include "HistoGUI_Library.cpp"

#ifndef HISTOGUI
#define HISTOGUI

class HistoGUI{


	public:

	HistoGUI(){}

	Display * disp;
	Window    wind;
	Window    wind_overlay;
	XEvent    evt;
	int       screen;

	XColor xcolour;
	XColor xcolour_red;
	XColor xcolour_veryred;
	Colormap cmap;

	XColor PixelColour[20];

	std::vector<double> x;
	std::vector<double> y;
	std::vector< std::vector<double> > z;

	double max_x;
	double min_x;
	double max_y;
	double min_y;

	double pos_x;
	double pos_y;
	double new_pos_x;
	double new_pos_y;

	unsigned int width;  
	unsigned int height;  
	double window_width;
    double window_height;
	double width_scale; 
	double x_offset;
	double height_scale;
	double y_offset;
	bool Draw2D_On;
	double scaleZ;
	bool drawLog;

	double old_xl, old_xh, old_yl, old_yh;
	double old_mouse_x, old_mouse_y;

	int Init();
	int SetData(std::vector<double> a, std::vector<double>b);
	int Loop();	
	void Close(){ printf("Closing now!\n"); XCloseDisplay(disp); }
	int DrawData(double x_low_win, double y_low_win, double x_hi_win, double y_hi_win);
	int DrawCrosshairs(int mouse_x, int mouse_y);
	int Zoom(int mouse_x, int mouse_y);

	int SetData2D(std::vector<double> a, std::vector<double> b, std::vector< std::vector<double> > c);
	int DrawData2D(double x_low_win, double y_low_win, double x_hi_win, double y_hi_win);
	
	bool Set2D( bool a ){ Draw2D_On = a; return a;}

	int Help();
	int HelpCode();
	int ReturnViridis(int i, int j);

};

#endif


