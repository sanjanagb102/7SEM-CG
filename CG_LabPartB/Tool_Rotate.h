/*
	Tool_Rotate.h
	Implements the Move tool
*/
#include<stdlib.h>
#include "cmath"
#pragma once
static float rotate_about_x, rotate_about_y, x_new, y_new, prev_key; float angle =90;
static float radian = angle * 3.141593/180;
/*
	Initializes tool when selected
*/
void Tool_Rotate::Start() {
	isDisplaying = false;
	isMouseDown = false;
	flickerFrameCount = 20;
	flickerColor = false;
}


/*
	Disables tool when another is selected
*/
void Tool_Rotate::End() {
	isDisplaying = false;
	isMouseDown = false;
}


/*
	Displays rect around selected pixels

	@param window_width - the width of the window
	@param window_height - the height of the window
*/
void Tool_Rotate::Display(int window_width, int window_height) {
	if ((!Tool_Rotate::isDisplaying) || isMouseDown) {
		// display white cover over canvas while user needs to draw rectangle to select pixels
		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
		glVertex2f(currentCanvas.xOffset, window_height - currentCanvas.yOffset);
		glVertex2f(currentCanvas.xOffset + (currentCanvas.width * currentCanvas.zoom), window_height - currentCanvas.yOffset);
		glVertex2f(currentCanvas.xOffset + (currentCanvas.width * currentCanvas.zoom), window_height - currentCanvas.yOffset - (currentCanvas.height * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset, window_height - currentCanvas.yOffset - (currentCanvas.height * currentCanvas.zoom));
		glEnd();
	}
	else {
		// display the blue/red flickering rectangle around selected pixels
		glBegin(GL_LINES);
		if (flickerColor) {
			glColor4f(0.4f, 0.4f, 1.0f, 1.0f);
		}
		else {
			glColor4f(1.0f, 0.4f, 0.4f, 1.0f);
		}
		glVertex2f(currentCanvas.xOffset + (startMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (endMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (endMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (endMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (endMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (startMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (startMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY * currentCanvas.zoom));
		glVertex2f(currentCanvas.xOffset + (startMouseX * currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY * currentCanvas.zoom));
		glEnd();
		// change the flickering colour every 20 frames
		flickerFrameCount--;
		if (flickerFrameCount <= 0) {
			flickerFrameCount = 20;
			flickerColor = !flickerColor;
		}
	}
}


/*
	Handles mouse press events passed onto the Move tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Rotate::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// get mouse position in canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the drag start position (mouse down) and end position (mouse up)
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			return true;
		}
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			isMouseDown = false;
			endMouseX = cx;
			endMouseY = cy;
			isDisplaying = true;
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
bool Tool_Rotate::BlockMousePress(int button, int state, int x, int y) {
	if (isMouseDown) {
		// take priority with mouse events when tool is selected and mouse is over canvas
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}


/*
	Handles special key events (arrow keys) for the tool

	@param key - the key that was pressed
	@param x - x position of the mouse
	@param y - y position of the mouse
	@return Has the tool handled the event?

bool Tool_Rotate::SpecialKey(int key, int x, int y) {

	std::string x_y;
	if (isDisplaying) {
		// get rect coordinates
		int minX = std::min(startMouseX, endMouseX);
		int maxX = std::max(startMouseX, endMouseX);
		int minY = std::min(startMouseY, endMouseY);
		int maxY = std::max(startMouseY, endMouseY);
		switch (key)
		{
		case GLUT_KEY_LEFT:
			// left key pressed
			if (minX > 0) {
				Colour white = { 1.0f, 1.0f, 1.0f };
				for (int x = 0; x < maxX - minX; x++) {
					for (int y = 0; y < maxY - minY; y++) {
						// move all pixels left
						if (x == 0)
						{
							x_y = std::to_string(minX - 1) + "*" + std::to_string(minY + y);
							pix_color[x_y] = currentCanvas.GetPixelColour(minX - 1, minY + y);
						}
						currentCanvas.SetPixelColour(minX + x - 1, minY + y, currentCanvas.GetPixelColour(minX + x, minY + y));
						if (x == maxX - minX - 1) {
							// leave column of white pixels on the right
							x_y = std::to_string(minX + x) + "*" + std::to_string(minY + y);
							if (pix_color.count(x_y) > 0) {
								currentCanvas.SetPixelColour(minX + x, minY + y, pix_color[x_y]);
								pix_color.erase(x_y);
							}
							else {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
				}
				// rectangle has moved
				startMouseX--;
				endMouseX--;
				return true;
			}
			break;
			// do the same for other arrow keys
		case GLUT_KEY_RIGHT:
			if (maxX < currentCanvas.width) {
				Colour white = { 1.0f, 1.0f, 1.0f };
				for (int x = maxX - minX - 1; x >= 0; x--) {
					for (int y = 0; y < maxY - minY; y++) {
						if (x == maxX - minX - 1)
						{
							x_y = std::to_string(minX + x + 1) + "*" + std::to_string(minY + y);
							pix_color[x_y] = currentCanvas.GetPixelColour(minX + x + 1, minY + y);
						}
						currentCanvas.SetPixelColour(minX + x + 1, minY + y, currentCanvas.GetPixelColour(minX + x, minY + y));
						if (x == 0) {
							x_y = std::to_string(minX + x) + "*" + std::to_string(minY + y);
							if (pix_color.count(x_y) > 0) {
								currentCanvas.SetPixelColour(minX + x, minY + y, pix_color[x_y]);
								pix_color.erase(x_y);
							}
							else {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
				}
				startMouseX++;
				endMouseX++;
				return true;
			}
			break;
		case GLUT_KEY_UP:
			if (minY > 0) {
				Colour white = { 1.0f, 1.0f, 1.0f };
				for (int x = 0; x < maxX - minX; x++) {
					for (int y = 0; y < maxY - minY; y++) {
						if (y == 0)
						{
							x_y = std::to_string(minX + x) + "*" + std::to_string(minY + y - 1);
							pix_color[x_y] = currentCanvas.GetPixelColour(minX + x, minY + y - 1);
						}
						currentCanvas.SetPixelColour(minX + x, minY + y - 1, currentCanvas.GetPixelColour(minX + x, minY + y));
						if (y == maxY - minY - 1) {
							x_y = std::to_string(minX + x) + "*" + std::to_string(minY + y);
							if (pix_color.count(x_y) > 0) {
								currentCanvas.SetPixelColour(minX + x, minY + y, pix_color[x_y]);
								pix_color.erase(x_y);
							}
							else {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
				}
				startMouseY--;
				endMouseY--;
				return true;
			}
			break;
		case GLUT_KEY_DOWN:
			if (maxY < currentCanvas.height) {
				Colour white = { 1.0f, 1.0f, 1.0f };
				for (int x = 0; x < maxX - minX; x++) {
					for (int y = maxY - minY - 1; y >= 0; y--) {
						if (y == maxY - minY - 1)
						{
							x_y = std::to_string(minX + x) + "*" + std::to_string(minY + y + 1);
							pix_color[x_y] = currentCanvas.GetPixelColour(minX + x, minY + y + 1);
						}
						currentCanvas.SetPixelColour(minX + x, minY + y + 1, currentCanvas.GetPixelColour(minX + x, minY + y));
						if (y == 0) {
							x_y = std::to_string(minX + x) + "*" + std::to_string(minY + y);
							if (pix_color.count(x_y) > 0) {
								currentCanvas.SetPixelColour(minX + x, minY + y, pix_color[x_y]);
								pix_color.erase(x_y);
							}
							else {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
				}
				startMouseY++;
				endMouseY++;
				return true;
			}
			break;
		}
		return true;
	}
	return false;
}*/
bool Tool_Rotate::SpecialKey(int key, int x, int y) {	
	if (isDisplaying) {
		// get rect coordinates
		
		float minX = std::min(startMouseX, endMouseX);
		float maxX = std::max(startMouseX, endMouseX);
		float minY = std::min(startMouseY, endMouseY);
		float maxY = std::max(startMouseY, endMouseY);
		float x_dist = maxX - minX;
		float y_dist = maxY - minY;
		rotate_about_x = minX;
		rotate_about_y = minY;
		switch (key)
		{
		case GLUT_KEY_DOWN:
			Colour white = { 1.0f, 1.0f, 1.0f };
			for (y = 0; y < maxY - minY ; y++) {
				for (x = 0; x < maxX - minX ; x++) {
					x_new = ((minX + x - rotate_about_x) * std::cos(radian)) + ((minY + y - rotate_about_y) * std::sin(radian)) + rotate_about_x - x_dist ;
					y_new = -((minX + x - rotate_about_x) * std::sin(radian)) + ((minY + y - rotate_about_y) * std::cos(radian)) + rotate_about_y + y_dist;
					currentCanvas.SetPixelColour(x_new, y_new, currentCanvas.GetPixelColour(minX + x, minY + y));
					currentCanvas.SetPixelColour(minX + x, minY + y, white);
				}
			}
			startMouseX = ((startMouseX - rotate_about_x) * std::cos(radian)) - ((startMouseY- rotate_about_y) * std::sin(radian)) + rotate_about_x ;
			startMouseY = ((startMouseX - rotate_about_x) * std::sin(radian)) + ((startMouseY- rotate_about_y) * std::cos(radian)) + rotate_about_y +  y_dist;
			endMouseX = ((endMouseX - rotate_about_x) * std::cos(radian)) - ((endMouseY - rotate_about_y) * std::sin(radian)) + rotate_about_x;
			endMouseY = ((endMouseX - rotate_about_x) * std::sin(radian)) + ((endMouseY - rotate_about_y) * std::cos(radian)) + rotate_about_y +  y_dist;
			
			break;
		
		
		}
		return true;
	}
	return false;
}



