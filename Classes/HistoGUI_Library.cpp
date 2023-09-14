#include "HistoGUI.hpp" 


int HistoGUI::SetData(std::vector<double> a, std::vector<double>b){
	for(int i=0; i < a.size(); i++){
		x.push_back(a[i]);
		y.push_back(b[i]);
	}
	return a.size();
}

int HistoGUI::SetData2D(std::vector<double> a, std::vector<double> b, std::vector<std::vector<double>> c){
	for(int i=0; i < b.size(); i++){
		y.push_back(b[i]);
	}
	for(int j=0; j < a.size(); j++){
		x.push_back(a[j]);
		std::vector<double> temp;
		for(int i=0; i < b.size(); i++){
			temp.push_back(c[j][i]);
		}
		z.push_back(temp);
	}
	return a.size();
}

int HistoGUI::Init(){

	disp = XOpenDisplay(NULL);
	if (disp == NULL) {
	   fprintf(stderr, "Cannot open display\n");
	   exit(1);
	}
	
	screen = DefaultScreen(disp);
	wind   = XCreateSimpleWindow(disp, RootWindow(disp, screen), 10, 10, 600, 400, 1,
	                        BlackPixel(disp, screen), WhitePixel(disp, screen));


	//XVisualInfo vinfo;
	//XMatchVisualInfo(disp, screen, 32, TrueColor, &vinfo);
	//
	//XSetWindowAttributes attr;
	//attr.colormap = XCreateColormap(disp, DefaultRootWindow(disp), vinfo.visual, AllocNone);
	//attr.override_redirect = 1;
	//attr.border_pixel = 0;
	//attr.background_pixel = 0;
	
	//wind_overlay = XCreateWindow(disp, wind, 10, 10, 600, 400, 0, vinfo.depth, InputOutput, vinfo.visual, /*CWColormap |*/ CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr);
	//XSelectInput(display, wind_overlay, StructureNotifyMask);
	//wind_overlay = XCreateSimpleWindow(disp, wind, 10, 10, 600, 400, 0, BlackPixel(disp, screen), WhitePixel(disp, screen)); 
	//XMapWindow(disp, wind_overlay);

	XGrabPointer(disp, wind, False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync,
                 GrabModeAsync, None, None, CurrentTime);
	XSelectInput(disp, wind, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
	XMapWindow(disp, wind);

	// colours
    cmap = DefaultColormap(disp, screen);
	xcolour.red = 32000; 
	xcolour.green = 32000; 
	xcolour.blue = 42000;
	xcolour.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(disp, cmap, &xcolour);

	xcolour_red.red   = 42000; 
	xcolour_red.green = 32000; 
	xcolour_red.blue  = 32000; 
	xcolour_red.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(disp, cmap, &xcolour_red);

	xcolour_veryred.red   = 65000; 
	xcolour_veryred.green = 32000; 
	xcolour_veryred.blue  = 32000; 
	xcolour_veryred.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(disp, cmap, &xcolour_veryred);

	/*
	for(int i = 0; i < 10; i++){	
		PixelColour[i].red   = 65535; 
		PixelColour[i].green = 65535 * (10 - i)/ 10; 
		PixelColour[i].blue  = 65535 * (10 - i)/ 10; 
		PixelColour[i].flags = DoRed | DoGreen | DoBlue;
		XAllocColor(disp, cmap, &PixelColour[i]);
	}
	*/

	for(int i = 0; i < 20; i++){
		PixelColour[i].red   = 65535 * ReturnViridis(i, 0) / 255;
		PixelColour[i].green = 65535 * ReturnViridis(i, 1) / 255;
		PixelColour[i].blue  = 65535 * ReturnViridis(i, 2) / 255;
		PixelColour[i].flags = DoRed | DoGreen | DoBlue;
		XAllocColor(disp, cmap, &PixelColour[i]);
	}

	scaleZ  = 1.0;
	drawLog = false;

	return 1;

}

int HistoGUI::ReturnViridis(int i, int j){
    int viridis[20][3] = {{253, 231, 37 },{218, 227, 25},{181, 222, 43},
                          {144, 215, 67 },{110, 206, 88},{78, 195, 107},
                          {53,  183, 121},{37,  171,130},{31, 158, 137},
                          {33,  145, 140},{38,  130,142},{43, 117, 142},
                          {49,  104, 142},{55,  90, 140},{62,  73, 137},
                          {68,  57,  131},{72,  40, 120},{72,  22, 104},
                          {68,  1,    84},{255, 255, 255}};
    /*
    viridis[19] = {253, 231, 37 };
    viridis[18] = {218, 227, 25 };
    viridis[17] = {181, 222, 43 };
    viridis[16] = {144, 215, 67 };
    viridis[15] = {110, 206, 88 };
    viridis[14] = {78,  195, 107};
    viridis[13] = {53,  183, 121};
    viridis[12] = {37,  171, 130};
    viridis[11] = {31,  158, 137};
    viridis[10] = {33,  145, 140};
    viridis[9] = {38,  130,  142};
    viridis[8] = {43,  117,  142};
    viridis[7] = {49,  104,  142};
    viridis[6] = {55,  90,   140};
    viridis[5] = {62,  73,   137};
    viridis[4] = {68,  57,   131};
    viridis[3] = {72,  40,   120};
    viridis[2] = {72,  22,   104};
    viridis[1] = {68,  1,    84 };
    viridis[0] = {255,  255, 255};
    */

	//printf("returning viridis [%i][%i] = %i\n",i,j, viridis[19-i][j]);
    return viridis[19-i][j];
}

int HistoGUI::DrawCrosshairs(int mouse_x, int mouse_y){

	int j1, j2;
	unsigned int j3, j4;  
	Window root_return;
	//XMapRaised(disp, wind_overlay);

	XGetGeometry(disp, wind, &root_return, &j1, &j2, &width, &height, &j3, &j4);
	XSetForeground(disp,  DefaultGC(disp, screen), xcolour_red.pixel);
	XDrawLine(disp, wind, DefaultGC(disp, screen), mouse_x, 0.0, mouse_x, height);
	XDrawLine(disp, wind, DefaultGC(disp, screen), 0.0, mouse_y, width,  mouse_y);
	
	pos_y = mouse_y * height_scale - y_offset;
	pos_x = mouse_x * width_scale  - x_offset;

	char coord[32];
	sprintf(coord, "(%.2f, %.2f)", pos_x, pos_y);
	XDrawString(disp, wind, DefaultGC(disp, screen), mouse_x + 5, mouse_y + 12, coord, strlen(coord));
	 
	return 1;
}

int HistoGUI::Zoom(int mouse_x, int mouse_y){
	
	new_pos_x = mouse_x * width_scale - x_offset;
	new_pos_y = mouse_y * height_scale  - y_offset;

	pos_x = old_mouse_x * width_scale - x_offset;
	pos_y = old_mouse_y * height_scale  - y_offset;
	

	if(std::abs(mouse_x - old_mouse_x) < 20 or std::abs(mouse_y - old_mouse_y) < 20) return 1;


	//printf("Zooming: %f - %f, %f - %f\n", mouse_x, old_mouse_x,mouse_y,old_mouse_y);

	double x_low_win, y_low_win, x_hi_win, y_hi_win;
	if(new_pos_x < pos_x){
		x_low_win = new_pos_x;
		x_hi_win  = pos_x;
		//printf("new low; [%f, %f]\n",x_low_win, x_hi_win);
	} else {
		x_low_win = pos_x;
		x_hi_win  = new_pos_x;
		//printf("new high; [%f, %f]\n",x_low_win, x_hi_win);
	}
	if(new_pos_y > pos_y){
		y_low_win = new_pos_y;
		y_hi_win  = pos_y;
	} else {
		y_low_win = pos_y;
		y_hi_win  = new_pos_y;
	}
	

	//printf("[(%f,%f) (%f,%f)]\n",x_low_win, y_low_win, x_hi_win, y_hi_win);
	XClearWindow(disp, wind);
//	XClearWindow(disp, wind_overlay);
	DrawData(x_low_win, y_low_win, x_hi_win, y_hi_win);
	//double x_low_win, double y_low_win, double x_hi_win, double y_hi_win

	return 1;

}


int HistoGUI::DrawData(double x_low_win, double y_low_win, double x_hi_win, double y_hi_win){

	if(Draw2D_On){
		DrawData2D(x_low_win, y_low_win, x_hi_win, y_hi_win);
		return 1;
	}

	int j1, j2;
	unsigned int j3, j4;  
	Window root_return;

	XGetGeometry(disp, wind, &root_return, &j1, &j2, &width, &height, &j3, &j4);

	//printf("Width = %u, Height = %u, x = %i, y = %i\n", width, height, j1, j2);
	//printf("[(%f,%f) (%f,%f)]\n",x_low_win, y_low_win, x_hi_win, y_hi_win);

	double x_step;// = (width  * 0.8) / x.size();
	double y_step;// = (height * 0.8) / x.size();


	if(x_low_win == -1 and y_low_win == -1 and x_hi_win == -1 and y_hi_win == -1){	
		// Audomatically decide data postition
		max_x = x[0];
		max_y = y[0];
		min_x = x[0];
		min_y = y[0];

		for(int i=0; i<x.size(); i++){
			if(x[i] > max_x) max_x = x[i];
			if(x[i] < min_x) min_x = x[i];

			if(drawLog){	
				if(y[i] <= 0){
					min_y = 0;
					continue;
				}
				if(log(y[i]) > max_y) max_y = log(y[i]);
				if(log(y[i]) < min_y) min_y = log(y[i]);
			} else { 
				if(y[i] > max_y) max_y = y[i];
				if(y[i] < min_y) min_y = y[i];
			}
		}
		//printf(" max_x = %f\n", max_x );
		//printf(" max_y = %f\n", max_y );
		//printf(" min_x = %f\n", min_x );
		//printf(" min_y = %f\n", min_y );

		width_scale = (max_x - min_x) / (0.8 * width);
		x_offset = 0.5 * ((max_x - min_x) / 0.8) - 0.5 * (min_x + max_x);

		height_scale = -1. * (max_y - min_y) / (0.8 * height);
		y_offset = -0.5 * ((max_y - min_y) / 0.8) + -0.5 * (min_y + max_y);
		
		//printf("width scale  = %f\n", width_scale);
		//printf("x_offset     = %f\n", x_offset);
		//printf("height scale = %f\n", height_scale);
		//printf("y_offset     = %f\n", y_offset);
		double axis_x  = (0. + x_offset) / width_scale;
		double axis_y  = (0. + y_offset) / height_scale;

		XSetForeground(disp, DefaultGC(disp,screen), xcolour.pixel);
		XDrawLine(disp, wind, DefaultGC(disp, screen), axis_x, 0.0, axis_x, height);
		XDrawLine(disp, wind, DefaultGC(disp, screen), 0.0, axis_y, width, axis_y);

		char axis_val[4];
		int w_step = width / 10;
		for(int i=0; i < (int) width; i += w_step){
			double x_val   = i * width_scale - x_offset;
			sprintf(axis_val, "%.1f", x_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), i, axis_y + 10, axis_val, strlen(axis_val));
		}

		int h_step = height / 10;
		for(int i=0; i < (int) height; i += h_step){
			double y_val = i * height_scale - y_offset;
			sprintf(axis_val, "%.1f", y_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), axis_x + 10, i, axis_val, strlen(axis_val));
		}
	
	
		XSetForeground(disp, DefaultGC(disp,screen), 0);	
		double x_wid ;
		double y_wid ;
		double x_wid2;
		double y_wid2;

		for(int i=0; i < x.size() - 2; i++){
			x_wid  = (x[i] + x_offset) / width_scale;
			x_wid2 = (x[i + 1] + x_offset) / width_scale;
			if(drawLog){
				if(y[i] <= 0){
					y_wid = 0;
				} else {
					y_wid  = (log(y[i]) + y_offset) / height_scale;
				}
				if(y[i+1] <= 0){
					y_wid2 = 0;
				} else {
					y_wid2 = (log(y[i + 1]) + y_offset) / height_scale;
				}
			} else {
				y_wid  = (y[i] + y_offset) / height_scale;
				y_wid2 = (y[i + 1] + y_offset) / height_scale;
			}
			//printf("(%f, %f), (%f,%f)\n", x_wid,y_wid,x_wid2,y_wid2);
			XDrawLine(disp, wind, DefaultGC(disp, screen), x_wid, y_wid, x_wid2, y_wid2);
			XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid -2, y_wid -2, 4, 4);
		}
		XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid2 -2, y_wid2 -2, 4, 4);

	} else {
		//double x_low_win, double y_low_win, double x_hi_win, double y_hi_win

		width_scale = (x_hi_win - x_low_win) / width;
		x_offset = -1.0 * x_low_win;

		height_scale = (y_hi_win - y_low_win) / height;
		y_offset = -1.0 * y_low_win;
		
		//printf("width scale  = %f\n", width_scale);
		//printf("x_offset     = %f\n", x_offset);
		//printf("height scale = %f\n", height_scale);
		//printf("y_offset     = %f\n", y_offset);
		double axis_x  = (0. + x_offset) / width_scale;
		double axis_y  = (0. + y_offset) / height_scale;

		XSetForeground(disp, DefaultGC(disp,screen), xcolour.pixel);
		XDrawLine(disp, wind, DefaultGC(disp, screen), axis_x, 0.0, axis_x, height);
		XDrawLine(disp, wind, DefaultGC(disp, screen), 0.0, axis_y, width, axis_y);

		char axis_val[4];
		int w_step = width / 10;
		for(int i=0; i < (int) width; i += w_step){
			double x_val   = i * width_scale - x_offset;
			sprintf(axis_val, "%.1f", x_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), i, axis_y + 10, axis_val, strlen(axis_val));
		}

		int h_step = height / 10;
		for(int i=0; i < (int) height; i += h_step){
			double y_val = i * height_scale - y_offset;
			sprintf(axis_val, "%.1f", y_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), axis_x + 10, i, axis_val, strlen(axis_val));
		}
	
	
		XSetForeground(disp, DefaultGC(disp,screen), 0);	
		double x_wid ;
		double y_wid ;
		double x_wid2;
		double y_wid2;
	
		for(int i=0; i < x.size() - 2; i++){
			x_wid  = (x[i] + x_offset) / width_scale;
			//y_wid  = (y[i] + y_offset) / height_scale;
			x_wid2 = (x[i + 1] + x_offset) / width_scale;
			//y_wid2 = (y[i + 1] + y_offset) / height_scale;
			if(drawLog){
				if(y[i] <= 0){
					y_wid = 0;
				} else {
					y_wid  = (log(y[i]) + y_offset) / height_scale;
				}
				if(y[i+1] <= 0){
					y_wid2 = 0;
				} else {
					y_wid2 = (log(y[i + 1]) + y_offset) / height_scale;
				}
			} else {
				y_wid  = (y[i] + y_offset) / height_scale;
				y_wid2 = (y[i + 1] + y_offset) / height_scale;
			}
		//	printf("(%f, %f), (%f,%f)\n", x_wid,y_wid,x_wid2,y_wid2);
			XDrawLine(disp, wind, DefaultGC(disp, screen), x_wid, y_wid, x_wid2, y_wid2);
			XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid -2, y_wid -2, 4, 4);
		}
		XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid2 -2, y_wid2 -2, 4, 4);


	}

	old_xl = x_low_win;
	old_yl = y_low_win;
	old_xh = x_hi_win;
	old_yh = y_hi_win;

	return 1;
}

int HistoGUI::DrawData2D(double x_low_win, double y_low_win, double x_hi_win, double y_hi_win){
	int j1, j2;
	unsigned int j3, j4;  
	Window root_return;
	XGetGeometry(disp, wind, &root_return, &j1, &j2, &width, &height, &j3, &j4);

	double x_step;// = (width  * 0.8) / x.size();
	double y_step;// = (height * 0.8) / x.size();

	if(x_low_win == -1 and y_low_win == -1 and x_hi_win == -1 and y_hi_win == -1){	
		// Audomatically decide data postition
		max_x = x[0];
		max_y = y[0];
		min_x = x[0];
		min_y = y[0];

		for(int i=0; i<x.size(); i++){
			if(x[i] > max_x) max_x = x[i];
			if(x[i] < min_x) min_x = x[i];
		}
		for(int i=0; i<y.size(); i++){
			if(y[i] > max_y) max_y = y[i];
			if(y[i] < min_y) min_y = y[i];
		}

		double max_cont = z[0][0];	

		for(int i=0; i<x.size(); i++){
			for(int j=0; j<y.size(); j++){
				if(drawLog) {
					if(z[i][j] <= 0) continue;
					if(log(z[i][j]) > max_cont) max_cont = log(z[i][j]);
				} else {
					if(z[i][j] > max_cont) max_cont = z[i][j];
				}
			}
		}

		max_cont *= scaleZ;

		//printf(" max_x = %f\n", max_x );
		//printf(" max_y = %f\n", max_y );
		//printf(" min_x = %f\n", min_x );
		//printf(" min_y = %f\n", min_y );

		width_scale = (max_x - min_x) / (0.8 * width);
		x_offset = 0.5 * ((max_x - min_x) / 0.8) - 0.5 * (min_x + max_x);

		height_scale = -1. * (max_y - min_y) / (0.8 * height);
		y_offset = -0.5 * ((max_y - min_y) / 0.8) + -0.5 * (min_y + max_y);
		
		//printf("width scale  = %f\n", width_scale);
		//printf("x_offset     = %f\n", x_offset);
		//printf("height scale = %f\n", height_scale);
		//printf("y_offset     = %f\n", y_offset);	
	
		XSetForeground(disp, DefaultGC(disp,screen), 0);	
		double x_wid ;
		double y_wid ;
		double x_wid2;
		double y_wid2;

		double binwidth_x = 0.8 * width_scale  / x.size();
		double binwidth_y = 0.8 * height_scale / y.size();
	

		for(int i=0; i < width; i++){
			x_wid  = 1+ (i * width_scale) - x_offset;
			if(x_wid > max_x) x_wid = max_x;
			if(x_wid < min_x) x_wid = min_x;

			for(int j=0; j < height; j++){
				y_wid  = 1 + (j * height_scale) - y_offset;
				if(y_wid > max_y) y_wid = max_y;
				if(y_wid < min_y) y_wid = min_y;
				int colindex = (int) 10 * z[(int)x_wid][(int)y_wid] / max_cont; 
				if(drawLog){
					if(z[(int)x_wid][(int)y_wid] <= 0){
						 colindex = 0;
					} else {
 						colindex = (int) 20 * log(z[(int)x_wid][(int)y_wid]) / max_cont;
					}
				}
				if(colindex > 19) colindex = 19;
				//int colindex = (int) 10 * i / width; 
				//if(colindex > 0){
				//printf("Colour = %i\n",colindex);
				//printf("(%f, %f) = %f\n", x_wid,y_wid, z[(int)x_wid][(int)y_wid]);
				//}
				XSetForeground(disp, DefaultGC(disp,screen), PixelColour[colindex].pixel);
				//XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid - 0.5* binwidth_x, y_wid -0.5*binwidth_y, binwidth_x, binwidth_y);
				XDrawPoint(disp, wind, DefaultGC(disp, screen), i,j);
			}
		}

		double axis_x  = (0. + x_offset) / width_scale;
		double axis_y  = (0. + y_offset) / height_scale;

		XSetForeground(disp, DefaultGC(disp,screen), xcolour.pixel);
		XDrawLine(disp, wind, DefaultGC(disp, screen), axis_x, 0.0, axis_x, height);
		XDrawLine(disp, wind, DefaultGC(disp, screen), 0.0, axis_y, width, axis_y);

		char axis_val[4];
		int w_step = width / 10;
		for(int i=0; i < (int) width; i += w_step){
			double x_val   = i * width_scale - x_offset;
			sprintf(axis_val, "%.1f", x_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), i, axis_y + 10, axis_val, strlen(axis_val));
		}

		int h_step = height / 10;
		for(int i=0; i < (int) height; i += h_step){
			double y_val = i * height_scale - y_offset;
			sprintf(axis_val, "%.1f", y_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), axis_x + 10, i, axis_val, strlen(axis_val));
		}

	} else {
		//double x_low_win, double y_low_win, double x_hi_win, double y_hi_win

		//double max_cont = z[0][0];	
		//for(int i=0; i<x.size(); i++){
		//	for(int j=0; j<y.size(); j++){
		//		if(z[i][j] > max_cont) max_cont = z[i][j];
		//	}
		//}

		width_scale = (x_hi_win - x_low_win) / width;
		x_offset = -1.0 * x_low_win;

		height_scale = (y_hi_win - y_low_win) / height;
		y_offset = -1.0 * y_low_win;
		
		//printf("width scale  = %f\n", width_scale);
		//printf("x_offset     = %f\n", x_offset);
		//printf("height scale = %f\n", height_scale);
		//printf("y_offset     = %f\n", y_offset);	
	
		XSetForeground(disp, DefaultGC(disp,screen), 0);	
		double x_wid ;
		double y_wid ;
		double x_wid2;
		double y_wid2;

		double binwidth_x = 0.8 * width_scale  / x.size();
		double binwidth_y = 0.8 * height_scale / y.size();

		double max_cont = z[0][0];	
		for(int i=0; i<width; i++){
			x_wid  = (i * width_scale) - x_offset;
			for(int j=0; j<height; j++){
				y_wid  = (j * height_scale) - y_offset;
				if(drawLog) {
					if(z[(int)x_wid][(int)y_wid] <= 0) continue;
					if(log(z[(int)x_wid][(int)y_wid]) > max_cont) max_cont = log(z[(int)x_wid][(int)y_wid]);
				} else {
					if(z[(int)x_wid][(int)y_wid] > max_cont) max_cont = z[(int)x_wid][(int)y_wid];
				}
			}
		}

		max_cont *= scaleZ;

		for(int i=0; i < width; i++){
			x_wid  = (i * width_scale) - x_offset;
			if(x_wid > max_x) x_wid = max_x;
			if(x_wid < min_x) x_wid = min_x;

			for(int j=0; j < height; j++){
				y_wid  = (j * height_scale) - y_offset;
				if(y_wid > max_y) y_wid = max_y;
				if(y_wid < min_y) y_wid = min_y;
				int colindex = (int) 10 * z[(int)x_wid][(int)y_wid] / max_cont; 
				if(drawLog){
					if(z[(int)x_wid][(int)y_wid] <= 0){
						 colindex = 0;
					} else {
						colindex = (int)220 * log(z[(int)x_wid][(int)y_wid]) / max_cont;
					}
				}
				if(colindex > 19) colindex = 19;
				//int colindex = (int) 10 * i / width; 
				//if(colindex > 0){
				//printf("Colour = %i\n",colindex);
				//printf("(%f, %f) = %f\n", x_wid,y_wid, z[(int)x_wid][(int)y_wid]);
				//}
				XSetForeground(disp, DefaultGC(disp,screen), PixelColour[colindex].pixel);
				//XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid - 0.5* binwidth_x, y_wid -0.5*binwidth_y, binwidth_x, binwidth_y);
				XDrawPoint(disp, wind, DefaultGC(disp, screen), i,j);
			}
		}

		double axis_x  = (0. + x_offset) / width_scale;
		double axis_y  = (0. + y_offset) / height_scale;

		XSetForeground(disp, DefaultGC(disp,screen), xcolour.pixel);
		XDrawLine(disp, wind, DefaultGC(disp, screen), axis_x, 0.0, axis_x, height);
		XDrawLine(disp, wind, DefaultGC(disp, screen), 0.0, axis_y, width, axis_y);

		char axis_val[4];
		int w_step = width / 10;
		for(int i=0; i < (int) width; i += w_step){
			double x_val   = i * width_scale - x_offset;
			sprintf(axis_val, "%.1f", x_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), i, axis_y + 10, axis_val, strlen(axis_val));
		}

		int h_step = height / 10;
		for(int i=0; i < (int) height; i += h_step){
			double y_val = i * height_scale - y_offset;
			sprintf(axis_val, "%.1f", y_val);
			XDrawString(disp, wind, DefaultGC(disp, screen), axis_x + 10, i, axis_val, strlen(axis_val));
		}


	}

	old_xl = x_low_win;
	old_yl = y_low_win;
	old_xh = x_hi_win;
	old_yh = y_hi_win;

	return 1;
}


int HistoGUI::Loop(){

	bool MousePressed  = false;
	bool MousePressed2 = false;

	while (1) {
		XNextEvent(disp, &evt);
		if (evt.type == Expose) {
	//		XFillRectangle(disp, wind, DefaultGC(disp, screen), 20, 20, 10, 10);
//			XDrawString   (disp, wind, DefaultGC(disp, screen), 10, 50, msg, strlen(msg));
			DrawData(-1,-1,-1,-1);
		} else if (evt.type == ButtonPress){
			/* store the mouse button coordinates in 'int' variables. */
			/* also store the ID of the window on which the mouse was */
			/* pressed.                                               */
			int mouse_x = evt.xbutton.x;
			int mouse_y = evt.xbutton.y;
			old_mouse_x = mouse_x;
			old_mouse_y = mouse_y;

			/* check which mouse button was pressed, and act accordingly. */
			if(evt.xbutton.button == Button1){
				/* draw a pixel at the mouse position. */
				XClearWindow(disp, wind);
				DrawCrosshairs(mouse_x, mouse_y);
				DrawData(old_xl, old_yl, old_xh, old_yh);
				MousePressed = true;
				printf("(%f, %f)\n", pos_x, pos_y);
			} if(evt.xbutton.button == Button3){
				MousePressed2 = true;
			} if(evt.xbutton.button == Button2){
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}
		} else if (evt.type == ButtonRelease){
			/* store the mouse button coordinates in 'int' variables. */
			/* also store the ID of the window on which the mouse was */
			/* pressed.                                               */
			int mouse_x = evt.xbutton.x;
			int mouse_y = evt.xbutton.y;
			//printf("Released button: %i, %i\n", mouse_x, mouse_y);
			//printf("Pressed button: %i, %i\n", mouse_x, mouse_y);
			/* check which mouse button was pressed, and act accordingly. */
			if(evt.xbutton.button == Button1){
				/* draw a pixel at the mouse position. */
				Zoom(mouse_x, mouse_y);
				//DrawData();
				MousePressed = false;
			} if(evt.xbutton.button == Button3){
				MousePressed2 = false;
			}
		} else if (evt.type == MotionNotify and MousePressed){

			int mouse_x = evt.xmotion.x;
			int mouse_y = evt.xmotion.y;

			//if(!Draw2D_On){
			//	XClearWindow(disp, wind);
			//	DrawCrosshairs(old_mouse_x, old_mouse_y);
			//	DrawCrosshairs(mouse_x, mouse_y);
			//	DrawData(old_xl, old_yl, old_xh, old_yh);
			//}

		} else if (evt.type == MotionNotify and MousePressed2){

			int mouse_x = evt.xmotion.x;
			int mouse_y = evt.xmotion.y;

			XSetForeground(disp, DefaultGC(disp,screen), xcolour_veryred.pixel);
			XFillRectangle(disp, wind, DefaultGC(disp, screen), mouse_x -2, mouse_y -2, 4, 4);
	
		} else if (evt.type == KeyPress){

			printf("Key pressed = %x\n", evt.xkey.keycode);
			if(evt.xkey.keycode == 0x41 or evt.xkey.keycode == 0x39){
				scaleZ = 1.0;
				XClearWindow(disp, wind);
				DrawData(-1,-1,-1,-1);
				
			}else if(evt.xkey.keycode == 0x2b){
				scaleZ *= 0.9;
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}else if(evt.xkey.keycode == 0x27){
				scaleZ *= 1.1;
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}else if(evt.xkey.keycode == 0x2d){
				if(drawLog){
					drawLog = false;
				} else {
					drawLog = true;
				}
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			} else {
				break;	
			}
		}
	}

	return 1;
}







int HistoGUI::Help(){

	printf("\n");
	printf("-- HistoGUI help --\n");
	printf("\n");
	printf("click          - Crosshairs\n");
	printf("click and drag - Zoom into region\n");
	printf("\n");
	printf("l - Log plot (y in 1D, z in 2D)\n");
	printf("p - Reduce z scale in 2D\n");
	printf("o - increase z scale in 2D\n");
	printf("q - quit\n");
	printf("\n");
	printf("--\n");
	printf("\n");
	printf("For help for using HistoGUI in your code run 'HelpCode()'.\n");
	printf("\n");
	printf("----\n");
	printf("\n");

	return 0;
}


int HistoGUI::HelpCode(){

	printf("\n");
	printf("-- HistoGUI help code --\n");
	printf("\n");
	printf("Create a HistoGUI object with:\n");
	printf("\tHistoGUI gui;\n");
	printf("\n");
	printf("Set the data:\n");
	printf("\tgui.Set2D(false);\n");
	printf("\tgui.SetData((std::vector<double>) X, (std::vector<double>)Y );\n");
	printf("\n");
	printf("Or if the data is 2D:\n");
	printf("\tgui.Set2D(true);\n");
	printf("\tgui.SetData2D(X,Y,Z);\n");
	printf("\n");
	printf("Then when you want to display the plot:\n");
	printf("\tgui.Init();\n");
	printf("\tgui.Loop();\n");
	printf("\n");
	printf("The loop will end when the user quits HistoGUI, after which use:\n");
	printf("\tgui.Close();\n");
	printf("\n");
	printf("This will complete the gui processes.\n");
	printf("\n");
	printf("----\n");
	printf("\n");

	return 0;
}


















