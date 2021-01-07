/*
	Tool_Circ.h
	Implements the Circle drawing tool
*/

#pragma once


/*
	Handles mouse press events passed onto the Circ tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?*/

	// Function to put pixels
// at subsequence points
//mid point elipse drawing algorithm
void midptellipse(int rx, int ry,
	int xc, int yc)
{
	float dx, dy, d1, d2, x, y;
	x = 0;
	y = ry;

	// Initial decision parameter of region 1 
	d1 = (ry * ry) - (rx * rx * ry) +
		(0.25 * rx * rx);
	dx = 2 * ry * ry * x;
	dy = 2 * rx * rx * y;

	// For region 1 
	while (dx < dy)
	{

		// Print points based on 4-way symmetry 
		currentCanvas.SetPixelColour(xc + x, yc + y, selectedColour);
		currentCanvas.SetPixelColour(xc - x, yc + y, selectedColour);
		currentCanvas.SetPixelColour(xc + x, yc - y, selectedColour);
		currentCanvas.SetPixelColour(xc - x, yc - y, selectedColour);

		// Checking and updating value of 
		// decision parameter based on algorithm 
		if (d1 < 0)
		{
			x++;
			dx = dx + (2 * ry * ry);
			d1 = d1 + dx + (ry * ry);
		}
		else
		{
			x++;
			y--;
			dx = dx + (2 * ry * ry);
			dy = dy - (2 * rx * rx);
			d1 = d1 + dx - dy + (ry * ry);
		}
	}

	// Decision parameter of region 2 
	d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
		((rx * rx) * ((y - 1) * (y - 1))) -
		(rx * rx * ry * ry);

	// Plotting points of region 2 
	while (y >= 0)
	{

		// Print points based on 4-way symmetry 
		currentCanvas.SetPixelColour(xc + x, yc + y, selectedColour);
		currentCanvas.SetPixelColour(xc - x, yc + y, selectedColour);
		currentCanvas.SetPixelColour(xc + x, yc - y, selectedColour);
		currentCanvas.SetPixelColour(xc - x, yc - y, selectedColour);

		// Checking and updating parameter 
		// value based on algorithm 
		if (d2 > 0)
		{
			y--;
			dy = dy - (2 * rx * rx);
			d2 = d2 + (rx * rx) - dy;
		}
		else
		{
			y--;
			x++;
			dx = dx + (2 * ry * ry);
			dy = dy - (2 * rx * rx);
			d2 = d2 + dx - dy + (rx * rx);
		}
	}
}
//Bresenhams Line drawing algo
void drawCircle(int xc, int yc, int x, int y)
{
	currentCanvas.SetPixelColour(xc+x, yc+y, selectedColour);
	currentCanvas.SetPixelColour(xc-x, yc+y, selectedColour);
	currentCanvas.SetPixelColour(xc+x, yc-y, selectedColour);
	currentCanvas.SetPixelColour(xc-x, yc-y, selectedColour);
	currentCanvas.SetPixelColour(xc+y, yc+x, selectedColour);
	currentCanvas.SetPixelColour(xc-y, yc+x, selectedColour);
	currentCanvas.SetPixelColour(xc+y, yc-x, selectedColour);
	currentCanvas.SetPixelColour(xc-y, yc-x, selectedColour);
}

// Function for circle-generation
// using Bresenham's algorithm
void circleBres(int xc, int yc, int r)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	drawCircle(xc, yc, x, y);
	while (y >= x)
	{
		// for each pixel we will
		// draw all eight pixels

		x++;

		// check for decision parameter
		// and correspondingly
		// update d, x, y
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		drawCircle(xc, yc, x, y);
	}
}

bool Tool_Circ::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// convert mouse position into canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the start mouse position if this is start of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			return true;
		}
		// draw the circle if this is the end of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			if ((startMouseX == cx) && (startMouseY == cy)) {
				// if the mouse hasn't moved, just colour one pixel
				currentCanvas.SetPixelColour(cx, cy, selectedColour);
			}
			else {
				// get the rect coordinates to put the circle in
				int minX = std::min(cx, startMouseX);
				int maxX = std::max(cx, startMouseX);
				int minY = std::min(cy, startMouseY);
				int maxY = std::max(cy, startMouseY);
				// work out the radii and center coords
				int radH = (maxX - minX) / 2;
				int radV = (maxY - minY) / 2;
				int centX = (maxX + minX) / 2;
				int centY = (maxY + minY) / 2;
				// call Bresenhams algoritm
				if (radH == radV) {
					circleBres(centX, centY, radH);
				}
				else {
					midptellipse(radH, radV,
						centX, centY);
				}
				
			}
			isMouseDown = false;
			return true;
		}
	}
	return false;
}


/*
	Should this tool take priority for receiving mouse events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Should this tool take priority for receiving mouse events
*/
bool Tool_Circ::BlockMousePress(int button, int state, int x, int y) {
	// if during a drag, this tool should take the mouse events first
	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}