
#include "HistoGUI.hpp"

int HistoGUI::Refresh(){
	printf("Refresh function not set!\n");
	return 0;
}

int HistoGUI::SetData(std::vector<double> a, std::vector<double>b){
	for(int i=0; i < a.size(); i++){
		x.push_back(a[i]);
		y.push_back(b[i]);
	}
	return a.size();
}

int HistoGUI::SetData2D(std::vector<double> a, std::vector<double> b, std::vector<std::vector<double> > c){
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

	nIterations = 1000;
	scaleZ  = 1.0;
	drawLog = false;
	edgeSet = false;
	funcDrawn = false;
	startingIndex = 0;
	endingIndex   = 0;
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

	if(Draw2D_On){
		int x_zpos = (int) (pos_x - min_x)/bw_x; 
		int y_zpos = (int) (pos_y - min_y)/bw_y;
		if(x_zpos < 0 or y_zpos < 0 or x_zpos > x.size() -1 or y_zpos > y.size()-1){
			pos_z = 0.0;
		} else { 
			pos_z = z[x_zpos][y_zpos]; 	
		}
		sprintf(coord, "(%.2f, %.2f) z=%.2f", pos_x, pos_y, pos_z);
	} else {
		if(drawLog){
			sprintf(coord, "(%.2f, %.2f)", pos_x, exp(pos_y));
		} else { 
			sprintf(coord, "(%.2f, %.2f)", pos_x, pos_y);
		}
	}
	XDrawString(disp, wind, DefaultGC(disp, screen), mouse_x + 5, mouse_y + 12, coord, strlen(coord));
	 
	return 1;
}

int HistoGUI::SetFitEdge(int mouse_x){

	int j1, j2;
	unsigned int j3, j4;  
	Window root_return;

	XGetGeometry(disp, wind, &root_return, &j1, &j2, &width, &height, &j3, &j4);
	XSetForeground(disp,  DefaultGC(disp, screen), xcolour_red.pixel);
	//XDrawLine(disp, wind, DefaultGC(disp, screen), mouse_x, 0.0, mouse_x, height);	
	pos_x = mouse_x * width_scale  - x_offset;


	if(edgeSet){
		if(oldEdge > pos_x){
			FitEdgeLo = pos_x;
			FitEdgeHi = oldEdge;
		} else if(oldEdge < pos_x){
			FitEdgeLo = oldEdge;
			FitEdgeHi = pos_x;
		} else {
			// Do nothing
			return 1;
		}
		XDrawLine(disp, wind, DefaultGC(disp, screen), mouse_x, 0.0, mouse_x, height);	

		Fit();
		edgeSet = false;

	} else {
		edgeSet = true;
		oldEdge = pos_x;

		XClearWindow(disp, wind);
		DrawData(old_xl, old_yl, old_xh, old_yh);
		XSetForeground(disp,  DefaultGC(disp, screen), xcolour_red.pixel);
		XDrawLine(disp, wind, DefaultGC(disp, screen), mouse_x, 0.0, mouse_x, height);	
	}

	return 1;
}


int HistoGUI::DrawGaus(){

	int j1, j2;
	unsigned int j3, j4;  
	Window root_return;

	XGetGeometry(disp, wind, &root_return, &j1, &j2, &width, &height, &j3, &j4);
	XSetForeground(disp, DefaultGC(disp,screen), xcolour_red.pixel);	
	double x_wid ;
	double y_wid ;
	double x_wid2;
	double y_wid2;
//	printf("WO = %f, WS = %f\n", x_offset, width_scale);
//	printf("HO = %f, HS = %f\n", y_offset, height_scale);
	//startingIndex = 0;
	//endingIndex   = 0;
	
	for(int i=startingIndex; i < endingIndex-2; i++){
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
			y_wid  = (Gaus(x[i]) + y_offset) / height_scale;
			y_wid2 = (Gaus(x[i + 1]) + y_offset) / height_scale;
		}
//		printf("(%f, %f), (%f,%f)\n", x_wid,y_wid,x_wid2,y_wid2);
		XDrawLine(disp, wind, DefaultGC(disp, screen), x_wid, y_wid, x_wid2, y_wid2);
		XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid -2, y_wid -2, 4, 4);
	}
	XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid2 -2, y_wid2 -2, 4, 4);

	int XposLabel = ((FitEdgeLo + x_offset) / width_scale) + 5;
	//int YposLabel = (fitParams[2] + y_offset) / height_scale; 
	int YposLabel = height*0.25;//(fitParams[2] + y_offset) / height_scale; 
	char gausInfo1[32];
	char gausInfo2[32];
	char gausInfo3[32];
	sprintf(gausInfo1, "Mean  = %.2f", fitParams[0]);	
	sprintf(gausInfo2, "Sigma = %.2f", fitParams[1]);	
	sprintf(gausInfo3, "Scale = %.2f", fitParams[2]);	
	XDrawString(disp, wind, DefaultGC(disp, screen), XposLabel, YposLabel,    gausInfo1, strlen(gausInfo1));
	XDrawString(disp, wind, DefaultGC(disp, screen), XposLabel, YposLabel+12, gausInfo2, strlen(gausInfo2));
	XDrawString(disp, wind, DefaultGC(disp, screen), XposLabel, YposLabel+24, gausInfo3, strlen(gausInfo3));

	funcDrawn = true;
	
	return 1;
}



double HistoGUI::Gaus(double inX){
	//double mean  = fitParams[0]; 
	//double sigma = fitParams[1];
	//double scale = fitParams[2];
	//double ret = ( scale / (sqrt2Pi* sigma) ) * einXp(pow((inX - mean),2.0) / (2.0*sigma*sigma));
	double ret = ( fitParams[2] / (sqrt2Pi* fitParams[1]) ) * exp(-1.0 * pow((inX - fitParams[0]),2.0) / (2.0*fitParams[1]*fitParams[1]));
	//printf("Gaus(%f) = %f\n", inX, ret);
	return ret;
}

double HistoGUI::DiffGaus_Mean(double inX){
	return Gaus(inX) * (inX - fitParams[0]) / (fitParams[1] * fitParams[1]);
}
double HistoGUI::DiffGaus_Sigma(double inX){
	return Gaus(inX) * ((-fitParams[0] - fitParams[1] + inX) * (-fitParams[0] + fitParams[1] + inX)) / (fitParams[1] * fitParams[1] * fitParams[1]);
}
double HistoGUI::DiffGaus_Scale(double inX){
	return Gaus(inX) * (1.0 / fitParams[2]);
}

double HistoGUI::SelectDiffGaus(int index, double inX){
	if(index == 0){
		return -1.0*DiffGaus_Mean(inX);
	} else if(index == 1){
		return -1.0*DiffGaus_Sigma(inX);
	} else if(index == 2){
		return -1.0*DiffGaus_Scale(inX);
	} else {
		printf("Error! - returning zero for fit partial derivative\n");
		return 0.0;
	}
}

int HistoGUI::Fit(){

	startingIndex = 0;
	endingIndex   = 0;
	bool lookForMax = false;
	double maxSearch_Fit; 
	
	double lastX_fit = x[0];
	for(int i=1; i < x.size(); i++){
		if(lastX_fit < FitEdgeLo and x[i] > FitEdgeLo){
			startingIndex = i;
			lookForMax = true;
			maxSearch_Fit = y[i];
		} 
		if(lookForMax and y[i] > maxSearch_Fit){
			maxSearch_Fit = y[i];
			printf("Val = %f, %f \n", x[i], y[i]);
		}
		if(lastX_fit < FitEdgeHi and x[i] > FitEdgeHi){
			endingIndex = i;
			lookForMax = false;
			break;
		}
		lastX_fit =  x[i];
	}

	//Estimate starting parameters
	fitParams[0] = (FitEdgeHi + FitEdgeLo) * 0.5;   // mean
	fitParams[1] = (FitEdgeHi - FitEdgeLo) * 1.1775 / 4;// sigma
	fitParams[2] = maxSearch_Fit * (sqrt2Pi* fitParams[1]); // scale

	//DrawGaus();

	int nData = endingIndex - startingIndex;
	printf("nData = %i = %i-%i\n", nData, endingIndex, startingIndex);

	int nParams = 3; // To add more fit params the size of matricies will have to change also

	double LastS = 0.0;
	for(int k=startingIndex; k < endingIndex; k++){
		LastS += pow((y[k] - Gaus(x[k])),2.0);
		printf("gaus(%f) = %f\n",x[k],Gaus(x[k]));
	}

	printf("INITIAL =========\n");
	printf(" [%f - %f] (%i - %i, %i)\n", FitEdgeLo, FitEdgeHi, startingIndex, endingIndex, nData);
	printf(" Mean  = %f \n", fitParams[0]);
	printf(" Sigma = %f \n", fitParams[1]);
	printf(" Scale = %f \n", fitParams[2]);
	printf(" S = %f\n", LastS);
	printf("=================\n");	

	int iteration = 0;
	for(; iteration < nIterations; iteration++){
		double JTJ[3][3]    = {0.0};
		double LDcomp[3][3] = {0.0};
	
		double Bmat[3] = {0.0};
		double Ymat[3] = {0.0};
		double Xmat[3] = {0.0};
		
		// Fill JTJ
		for(int i = 0; i < nParams; i++){
			for(int j = 0; j < nParams; j++){
				for(int k = startingIndex; k < endingIndex; k++){
					JTJ[i][j] += SelectDiffGaus(i, x[k]) * SelectDiffGaus(j, x[k]); 	
				}
			}
		}
	
		// Invert JTJ - Cholesky decomposition
		for(int i = 0; i < nParams; i++) {
	 		for(int j = 0; j <= i; j++) {
	    		double sum = 0.0;
	    		for(int k = startingIndex; k < j; k++) sum += LDcomp[i][k] * LDcomp[j][k];
	    		if(i == j){
					LDcomp[i][j] = sqrt(JTJ[i][i] - sum);
	    		} else { 
					LDcomp[i][j] = (1.0 / LDcomp[j][j] * (JTJ[i][j] - sum));
				}
			}			
		}    
	//	printf("JTJ:\n");
	//	for(int  i=0; i < 3; i++){
	//	for(int  j=0; j < 3; j++){
	//		printf("[%f] ", JTJ[i][j]);
	//	}
	//	printf("\n");		
	//	} 
	//	printf("\n");
	//	printf("LDcomp:\n");
	//	for(int  i=0; i < 3; i++){
	//	for(int  j=0; j < 3; j++){
	//		printf("[%f] ", LDcomp[i][j]);
	//	}
	//	printf("\n");		
	//	} 
	//	printf("\n");
	
		// Find residuals
		for(int i=0; i < nParams; i++){
			for(int k=startingIndex; k < endingIndex; k++){
				Bmat[i] += 1.0 * (y[k] - Gaus(x[k])) * SelectDiffGaus(i, x[k]);
			}
		}
	//	printf("BMat:\n");
	//	for(int  i=0; i < 3; i++){
	//		printf("[%f]\n", Bmat[i]);
	//	} 
	//	printf("\n");
	
		// Forward Substitution
		Ymat[0] = Bmat[0] / LDcomp[0][0];
		for(int m = 1; m < nParams; m++){
			double sum = 0.0;
			for(int i=0; i < m; i++){
				sum -= LDcomp[m][i] * Ymat[i];
			}
			Ymat[m] = (Bmat[m] - sum) / LDcomp[m][m];
		}
	//	printf("YMat:\n");
	//	for(int  i=0; i < 3; i++){
	//		printf("[%f]\n", Ymat[i]);
	//	} 
	//	printf("\n");
	
		// Backwards Substitution
		Xmat[nParams - 1] = Ymat[nParams - 1] / LDcomp[nParams - 1][nParams - 1];
		for(int m = nParams - 2; m >= 0; m--){
			double sum = 0.0;
			for(int i= m+1; i < nParams; i++){
				sum += LDcomp[i][m] * Xmat[i];
			}
			Xmat[m] = (Ymat[m] - sum) / LDcomp[m][m];
		}
	
		for(int i=0; i < nParams; i++){
			fitParams[i] -= Xmat[i];
		}	
	//	printf("XMat:\n");
	//	for(int  i=0; i < 3; i++){
	//		printf("[%f]\n", Xmat[i]);
	//	} 
	//	printf("\n");

//		if(fitParams[0] != fitParams[0] or fitParams[0] > FitEdgeHi or fitParams[0] < FitEdgeLo)
//			fitParams[0] = (FitEdgeHi + FitEdgeLo) * 0.5;   // mean
//		if(fitParams[1] != fitParams[1] or fitParams[1] > FitEdgeHi - FitEdgeLo or fitParams[1] <= 0)
//			fitParams[1] = (FitEdgeHi - FitEdgeLo) * 1.1775 / 4;// sigma
//		if(fitParams[2] != fitParams[2])
//			fitParams[2] = maxSearch_Fit * (sqrt2Pi* fitParams[1]); // scale

		if(fitParams[0] > FitEdgeHi or fitParams[0] < FitEdgeLo)
			fitParams[0] = (FitEdgeHi + FitEdgeLo) * 0.5;   // mean
		if(fitParams[1] > FitEdgeHi - FitEdgeLo or fitParams[1] <= 0)
			fitParams[1] = (FitEdgeHi - FitEdgeLo) * 1.1775 / 4;// sigma

		// Get the ressquared
		double S = 0.0;
		for(int k=startingIndex; k < endingIndex; k++){
			S += pow((y[k] - Gaus(x[k])),2.0);
		}
	
		if(LastS - S < 10 and iteration > 20){
			LastS = S;
			break;
		}
		if(S != S){
			printf("Fit failed!\n");
			break;
		}
		LastS = S;
	//printf("=================\n");
	//printf(" Mean  = %f \n", fitParams[0]);
	//printf(" Sigma = %f \n", fitParams[1]);
	//printf(" Scale = %f \n", fitParams[2]);
	//printf(" S = %f\n", LastS);
	//printf(" Iterations = %i\n", iteration); 
	//printf("=================\n");	
	}
	
	printf("=================\n");
	printf(" Mean  = %f \n", fitParams[0]);
	printf(" Sigma = %f \n", fitParams[1]);
	printf(" Scale = %f \n", fitParams[2]);
	printf(" S = %f\n", LastS);
	printf(" Iterations = %i\n", iteration); 
	printf("=================\n");	
	// Xmat[nData] is now the change in the params
	DrawGaus();

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
	if(funcDrawn) DrawGaus();
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

		int drawoffset = (int) 8 * strlen(yAxisTitle);
		XDrawString(disp, wind, DefaultGC(disp, screen), 20, axis_y - 3, xAxisTitle, strlen(xAxisTitle));
		XDrawString(disp, wind, DefaultGC(disp, screen), axis_x - drawoffset, 20, yAxisTitle, strlen(yAxisTitle));

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
			if(drawLog){
				sprintf(axis_val, "%.1f", exp(y_val));
			} else { 
				sprintf(axis_val, "%.1f", y_val);
			}
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

		int drawoffset = (int) 8 * strlen(yAxisTitle);
		XDrawString(disp, wind, DefaultGC(disp, screen), 20, axis_y - 3, xAxisTitle, strlen(xAxisTitle));
		XDrawString(disp, wind, DefaultGC(disp, screen), axis_x - drawoffset, 20, yAxisTitle, strlen(yAxisTitle));

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
			if(drawLog){
				sprintf(axis_val, "%.1f", exp(y_val));
			} else { 
				sprintf(axis_val, "%.1f", y_val);
			}
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

//	using namespace std::chrono;
	//printf("Drawing 2d\n");
	int j1, j2;
	unsigned int j3, j4;  
	Window root_return;
	XGetGeometry(disp, wind, &root_return, &j1, &j2, &width, &height, &j3, &j4);

	double x_step;// = (width  * 0.8) / x.size();
	double y_step;// = (height * 0.8) / x.size();

//	auto start = high_resolution_clock::now();

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
		if(drawLog) max_cont = 0;

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
		//printf(" max_z = %f\n", max_cont);

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

		bw_x = (max_x - min_x) / x.size();
		bw_y = (max_y - min_y) / y.size();
	//	x_wid  = (x[i] + x_offset) / width_scale;
	
		for(int i=0; i < width; i++){
			x_wid  = 1+ (i * width_scale) - x_offset;
			if(x_wid > max_x) x_wid = max_x;
			if(x_wid < min_x) x_wid = min_x;
			//printf("(%f, ??)\n", x_wid);
			//x_wid  = (x[i] + x_offset) / width_scale;

			for(int j=0; j < height; j++){
				y_wid  = 1 + (j * height_scale) - y_offset;
	
			//	printf("(%f, %f)\n", x_wid, y_wid);
				if(y_wid > max_y) y_wid = max_y;
				if(y_wid < min_y) y_wid = min_y;

	//			printf("(%f, %f) = ", x_wid,y_wid);
				//printf(" %f\n", z[(int)x_wid - min_x][(int)y_wid - min_y]);
				int x_zpos = (int) (x_wid - min_x)/bw_x; 
				int y_zpos = (int) (y_wid - min_y)/bw_y;
				if(x_zpos < 0 or y_zpos < 0 or x_zpos > x.size() -1 or y_zpos > y.size()-1) continue;
				//printf("%d, %d [%f, %f] (%f,%f)\n", x_zpos, y_zpos, x_wid, y_wid , bw_x, bw_y);

				int colindex = (int) 10 * z[x_zpos][y_zpos] / max_cont; 
				//printf("%d\n" , colindex);
				//printf("%f - %f = %i (%d)\n", (int)(x_wid - min_x), colindex);
				if(drawLog){
					if(z[x_zpos][y_zpos] <= 0){
						 colindex = 0;
					} else {
 						colindex = (int) 20 * log(z[x_zpos][y_zpos]) / max_cont;
					}
				}
				if(colindex > 19) colindex = 19;
				//int colindex = (int) 10 * i / width; 
				//if(colindex > 0){
				//printf("Colour = %i\n",colindex);
				//printf("(%f, %f) = %f\n", x_wid,y_wid, z[(int)x_wid - min_x][(int)y_wid - min_y]);
				//}
				XSetForeground(disp, DefaultGC(disp,screen), PixelColour[colindex].pixel);

				//XFillRectangle(disp, wind, DefaultGC(disp, screen), x_wid - 0.5* binwidth_x, y_wid -0.5*binwidth_y, binwidth_x, binwidth_y);
				//printf("BW = %ii\n", (int)binwidth_y  / 1.0);
				XDrawPoint(disp, wind, DefaultGC(disp, screen), i,j);
			}
		}

		double axis_x  = (0. + x_offset) / width_scale;
		double axis_y  = (0. + y_offset) / height_scale;

		XSetForeground(disp, DefaultGC(disp,screen), xcolour.pixel);
		XDrawLine(disp, wind, DefaultGC(disp, screen), axis_x, 0.0, axis_x, height);
		XDrawLine(disp, wind, DefaultGC(disp, screen), 0.0, axis_y, width, axis_y);

		int drawoffset = (int) 8 * strlen(yAxisTitle);
		XDrawString(disp, wind, DefaultGC(disp, screen), 20, axis_y - 3, xAxisTitle, strlen(xAxisTitle));
		XDrawString(disp, wind, DefaultGC(disp, screen), axis_x - drawoffset, 20, yAxisTitle, strlen(yAxisTitle));

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
		bw_x = (max_x - min_x) / x.size();
		bw_y = (max_y - min_y) / y.size();

		double max_cont = z[0][0];	
		for(int i=0; i<width; i++){
			x_wid  = (i * width_scale) - x_offset;
			for(int j=0; j<height; j++){
				y_wid  = (j * height_scale) - y_offset;
				int x_zpos = (int) (x_wid - min_x)/bw_x; 
				int y_zpos = (int) (y_wid - min_y)/bw_y;
				if(x_zpos < 0 or y_zpos < 0 or x_zpos > x.size() -1 or y_zpos > y.size()-1) continue;
				if(drawLog) {
					if(z[x_zpos][y_zpos] <= 0) continue;
					if(log(z[x_zpos][y_zpos]) > max_cont) max_cont = log(z[x_zpos][y_zpos]);
				} else {
					if(z[x_zpos][y_zpos] > max_cont) max_cont = z[x_zpos][y_zpos];
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
				int x_zpos = (int) (x_wid - min_x)/bw_x; 
				int y_zpos = (int) (y_wid - min_y)/bw_y;
				if(x_zpos < 0 or y_zpos < 0 or x_zpos > x.size() -1 or y_zpos > y.size()-1) continue;
				if(y_wid > max_y) y_wid = max_y;
				if(y_wid < min_y) y_wid = min_y;
				int colindex = (int) 10 * z[x_zpos][y_zpos] / max_cont; 
				if(drawLog){
					if(z[x_zpos][y_zpos] <= 0){
						 colindex = 0;
					} else {
						colindex = (int)220 * log(z[(int)(x_wid - min_x)][(int)(y_wid - min_x)]) / max_cont;
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

		int drawoffset = (int) 8 * strlen(yAxisTitle);
		XDrawString(disp, wind, DefaultGC(disp, screen), 20, axis_y - 3, xAxisTitle, strlen(xAxisTitle));
		XDrawString(disp, wind, DefaultGC(disp, screen), axis_x - drawoffset, 20, yAxisTitle, strlen(yAxisTitle));

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
	

//	auto stop = high_resolution_clock::now();
//	auto duration = duration_cast<microseconds>(stop - start);
//	printf("Time taken = %lld\n", duration.count());

	return 1;
}


int HistoGUI::Loop(){

	bool MousePressed  = false;
	bool MousePressed2 = false;
	

	while (1) {
		if (auto_refresh == true and !XPending(disp)){
				Refresh();
				std::this_thread::sleep_for(std::chrono::milliseconds(refresh_time)); 
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
				continue;
		}

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
				DrawData(old_xl, old_yl, old_xh, old_yh);
				DrawCrosshairs(mouse_x, mouse_y);
				MousePressed = true;
				if(Draw2D_On){
					printf("(%f, %f) z=%.2f\n", pos_x, pos_y, pos_z);
				} else {
					printf("(%f, %f)\n", pos_x, pos_y);
				}
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

			//printf("Key pressed = %lx \n", XkbKeycodeToKeysym(disp, evt.xkey.keycode, 0,0));
			double keySym = XkbKeycodeToKeysym(disp, evt.xkey.keycode, 0,0);
			if(keySym == 0x20){
				scaleZ = 1.0;
				funcDrawn = false;
				XClearWindow(disp, wind);
				DrawData(-1,-1,-1,-1);
				
			}else if(keySym == 0x70){
				scaleZ *= 0.9;
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}else if(keySym == 0x73 and auto_refresh){
				auto_refresh = false;
			}else if(keySym == 0x73 and !auto_refresh){
				auto_refresh = true;
			}else if(keySym == 0x6f){
				scaleZ *= 1.1;
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}else if(keySym == 0x72){
				Refresh();
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}else if(keySym == 0x6c){
				if(drawLog){
					drawLog = false;
				} else {
					drawLog = true;
				}
				XClearWindow(disp, wind);
				DrawData(old_xl, old_yl, old_xh, old_yh);
			}else if(keySym == 0x66){
				// Set fit edge
				int mouse_x = evt.xbutton.x;
				SetFitEdge(mouse_x);
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
	printf("l - log plot (y in 1D, z in 2D)\n");
	printf("p - reduce z scale in 2D\n");
	printf("o - increase z scale in 2D\n");
	printf("r - call refresh function (if set)\n");
	printf("s - toggle auto-refresh\n");
	printf("f - place fit ranges\n");
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
	printf("To implement a refresh function:\n");
	printf(" - Make a specific subclass of HistoGUI\n");
	printf(" - Redefine the function 'Refresh()'\n");
	printf(" example is in 'Example.cxx'\n");
	printf("\n");
	printf("To implement an auto-refresh that calls your refresh function include:\n");
	printf("\tgui.SetAutoRefresh(true, refreshTime_in_milliseconds);\n");
	printf("\n");
	printf("Don't have the refesh too long as the GUI will be unresponsive during the wait.\n");
	printf("\n");
	printf("----\n");
	printf("\n");

	return 0;
}


















