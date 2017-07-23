#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
#include <iostream>
#include "triangle_rast.h"
using namespace std;

unsigned int window[480][360];
int windX = 360;
int windY = 480;
myGL::Polygon** polys;
int numPl;
int numLine=0;

void drawLine(myGL::Line* line);
void drawBren(myGL::Line* line);
myGL::Pixel* getCenter(myGL::Polygon pl);

myGL::Pixel::Pixel(double x1, double y1)
{
	x = x1;
	y = y1;
	next = NULL;
}
myGL::Pixel::~Pixel()
{

}
myGL::Line::Line()
{
	num = 2;
}
myGL::Line::Line(Pixel* first1, Pixel* last1)
{
	first = first1;
	last = last1;
	first->next = last;
	num = 2;
}
myGL::Line::~Line()
{

}
myGL::Polygon::Polygon()
{

}
myGL::Polygon::~Polygon()
{
	delete[] lines;
	delete[] points;
}
void read()
{
	int numPt = 0;
	//cout << "number of polygons: ";
	//cin >> numPl;
	numPl = 1;
	polys = new myGL::Polygon*[numPl];

	for (int p = 0; p < numPl; p++)
	{
		polys[p] = new myGL::Polygon();
		cout << "Triangle "<< p+1 << "\n";
		numPt = 3;
		myGL::Pixel** pix = new myGL::Pixel*[numPt];
		polys[p]->lines = new myGL::Line*[numPt];

		for (int i = 0; i < numPt; i++)
		{
			int x, y;
			cout << "x: ";
			cin >> x;
			cout << "y: ";
			cin >> y;

			pix[i] = new myGL::Pixel(x, y);
		}//gets all the points

		int j;
		for (j = 0; j < numPt - 1; j++)
		{
			myGL::Line *line1 = new myGL::Line(pix[j], pix[j + 1]);
			polys[p]->lines[j] = line1;
		}
		myGL::Line *line1 = new myGL::Line(pix[0], pix[j]);
		polys[p]->lines[j] = line1;
		polys[p]->numPts = numPt; //creates lines from points

		polys[p]->points = pix; //sets points
		polys[p]->polyNum = p;
	}//creates each polygon

}
void getLineAlg(myGL::Line* line)
{
	if (numLine == 0)
	{
		cout << "1 for DDA, 2 for Bresenham: ";
		cin >> numLine;
	}
	if (numLine == 1)
	{
		drawLine(line);
	}
	else
	{
		drawBren(line);
	}
	
	
}

myGL::Pixel *getCenter(const myGL::Polygon *pl)
{
	int xAvg=0, yAvg=0;
	for (int i = 0; i < pl->numPts; i++)
	{
		xAvg += pl->points[i]->x;
		yAvg += pl->points[i]->y;
	}

	xAvg = xAvg / pl->numPts;
	yAvg = yAvg / pl->numPts;

	//cout << xAvg << " " << yAvg << endl;
	myGL::Pixel *pix = new myGL::Pixel(xAvg, yAvg);
	return pix;
}
void inputLine(myGL::Line* line)
{
	int x = line->first->x;
	int y = line->first->y;
	window[y][x] = 255;

	x = line->last->x;
	y = line->last->y;
	window[y][x] = 255;

	line->toFirst();
	for (int i = 0; i < line->num; i++)
	{
		int x = (line->itr)->x;
		int y = (line->itr)->y;
		window[y][x] = 255;
		line->itr = line->itr->next;
	}
}
void drawLine(myGL::Line* line)
{
	if (line->last->x < line->first->x)
	{
		myGL::Pixel *temp = line->last;
		line->last = line->first;
		line->first = temp;
	}
	double difX = (line->last->x) - (line->first->x);
	double difY = (line->last->y) - (line->first->y);

	//cout << difX << " " << difY << " " << ((difY / difX) >= 1 || (difY / difX) <= -1) << endl;
	if (difY == 0)
	{
		int y = line->first->y;
		int xi = line->first->x;
		int xf = line->last->x;
		line->toFirst();
		for (int i = xi + 1; i < xf; i++)
		{
			myGL::Pixel *pixel = new myGL::Pixel(i, y);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}

	else if (difX == 0)
	{
		int x = line->first->x, yf, yi;

		if ((line->first->y)>(line->last->y))
		{
			yf = line->first->y;
			yi = line->last->y;
		}

		else
		{
			yi = line->first->y;
			yf = line->last->y;
		}

		line->toFirst();
		for (int i = yi + 1; i < yf; i++)
		{
			myGL::Pixel *pixel = new myGL::Pixel(x, i);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}

	else if ((difY / difX) < 1 && (difY / difX) > -1)
	{
		double m = difY / difX;
		double b = line->first->y;

		line->toFirst();
		for (int i = 1; i < abs(difX); i++)
		{
			int y = (m*i + b + 0.5);
			myGL::Pixel *pixel = new myGL::Pixel((line->itr->x) + 1, y);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}
	else if ((difY / difX) >= 1)
	{
		if (line->last->x < line->first->x)
		{
			myGL::Pixel *temp = line->last;
			line->last = line->first;
			line->first = temp;
		}
		double m = difX / difY;
		double b = line->first->x;

		line->toFirst();
		for (int i = 1; i < abs(difY); i++)
		{
			int x = (m*i + b + 0.5);
			myGL::Pixel *pixel = new myGL::Pixel(x, (line->itr->y) + 1);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}

	else if ((difY / difX) <= -1)
	{
		
		double m = difX / difY;
		double b = line->first->x;

		line->toFirst();
		for (int i = 1; i < abs(difY); i++)
		{
			int x = (abs(m)*i + b + 0.5);
			myGL::Pixel *pixel = new myGL::Pixel(x, (line->itr->y) - 1);
			line->itr->next = pixel;
			line->itr = line->itr->next;
			(line->num)++;
		}
		line->itr->next = line->last;
	}

}
void drawBren(myGL::Line* line)
{
	if (line->last->x < line->first->x)
	{
		myGL::Pixel *temp = line->last;
		line->last = line->first;
		line->first = temp;
	}
	double difX = (line->last->x) - (line->first->x);
	double difY = (line->last->y) - (line->first->y);
	int Pi = 2 * difY - difX;

	//cout << (difY / difX) << endl;
	if (0 <= (difY / difX) && (difY/difX) < 1)
	{
		int yadd;

		if (Pi > 0)
		{
			yadd = 1;
		}

		else
		{
			yadd = 0;
		}

		line->toFirst();
		int yi = line->first->y;//initial yi

		line->first->next = new myGL::Pixel((line->first->x)+1, yi + yadd);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		for (int i = line->itr->x; i < line->last->x; i++)
		{
			int y = line->itr->y;
			Pi = Pi + (2 * difY) - (2 * difX*(y - yi));

			if (Pi > 0)
			{
				yadd = 1;
			}
			else
			{
				yadd = 0;
			}

			myGL::Pixel *pix = new myGL::Pixel(i + 1, y + yadd);
			yi = y;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}//Breneham

	else if (-1 < (difY/difX) && (difY/difX) < 0)
	{
		int yadd;

		if (Pi < 0)
		{
			yadd = -1;
		}

		else
		{
			yadd = 0;
		}

		line->toFirst();
		int yi = line->first->y;//initial yi

		line->first->next = new myGL::Pixel((line->first->x) + 1, yi + yadd);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		int y = line->itr->y;
		for (int i = line->itr->x; i < line->last->x; i++)
		{
			y = line->itr->y;
			Pi = Pi + (2 * difY) - (2 * difX*(y - yi));

			if (Pi <= 0 && y > line->last->y)
			{
				yadd = -1;
			}
			else if (Pi > 0)
			{
				yadd = 0;
			}

			myGL::Pixel *pix = new myGL::Pixel(i + 1, y + yadd);
			yi = y;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}

	else if (difY / difX <= -1)
	{
		int xadd;

		if (Pi < 0)
		{
			xadd = 1;
		}

		else
		{
			xadd = 0;
		}

		line->toFirst();
		int xi = line->first->x;//initial xi

		line->first->next = new myGL::Pixel(xi+xadd,(line->first->y) - 1);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		for (int i = line->itr->y; i > line->last->y; i--)
		{
			int x = line->itr->x;
			Pi = Pi + (2 * difX) - (2 * difY*(x - xi));

			if (Pi > 0)
			{
				xadd = 1;
			}
			else
			{
				xadd = 0;
			}

			myGL::Pixel *pix = new myGL::Pixel(x + xadd,i-1);
			xi = x;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}

	else if (difY / difX >= 1)
	{
		int xadd;

		if (Pi >= 0)
		{
			xadd = 1;
		}

		else
		{
			xadd = 0;
		}

		line->toFirst();
		int xi = line->first->x;//initial xi

		line->first->next = new myGL::Pixel(xi + xadd, (line->first->y) + 1);//create next Pixel with x+1, yi + either 1 or 0
		line->num++;
		line->itr = line->itr->next;

		for (int i = line->itr->y; i < line->last->y; i++)
		{
			int x = line->itr->x;
			Pi = Pi + (2 * difX) - (2 * difY*(x - xi));

			if (Pi >= 0)
			{
				xadd = 1;
			}
			else
			{
				xadd = 0;
			}

			myGL::Pixel *pix = new myGL::Pixel(x + xadd, i + 1);
			xi = x;
			line->itr->next = pix;
			line->num++;
			line->itr = line->itr->next;
		}

		line->itr->next = line->last;
	}

	/*
	line->toFirst();
	for (int i = 0; i < line->num; i++)
	{
		int x = (line->itr)->x;
		int y = (line->itr)->y;
		window[y][x] = 255;
		line->itr = line->itr->next;
	}*/
}
void rastPoly()
{
	bool in = true;
	for (int i = 0; i < 480; i++)
	{
		myGL::Pixel *prev = NULL;
		myGL::Pixel *first = NULL;
		for (int j = 0; j < 360; j++)
		{
			if (window[i][j] == 255)
			{
				myGL::Pixel *out = new myGL::Pixel(j, i);
				if (prev == NULL)
				{
					prev = out;
					first = prev;
				}
				else
				{
					prev->next = out;
					prev = prev->next;

				}
			}
		}
		if (prev != NULL)
		{
			myGL::Pixel *itr =first;
			while (itr!=NULL && itr->next != NULL)
			{
				for (int k = 0; k < numPl; k++)
				{
					//cout << "(" << j << ", " << i << ")" << endl;
					for (int l = 0; l < polys[k]->numPts; l++)
					{
						if ((polys[k]->points[l]->x == itr->x && polys[k]->points[l]->y == itr->y) || (polys[k]->points[l]->x == itr->next->x && polys[k]->points[l]->y == itr->next->y))
						{
							itr = itr->next;
							k = numPl;
							break;
							in = false;
						}
					}
				}
				if (in == true && itr->next != NULL)
				{
					myGL::Line *line = new myGL::Line(itr, itr->next);
					drawLine(line);
					inputLine(line);
					itr = itr->next->next;
				}
			}
		}
		
	}
	glutPostRedisplay();
	
}
void drawPoly()
{
	for (int i = 0; i < numPl; i++)
	{
		for (int j = 0; j < polys[i]->numPts; j++)
		{
			getLineAlg(polys[i]->lines[j]);
			inputLine(polys[i]->lines[j]);
		}
		//getCenter(polys[i]);
	}

	glutPostRedisplay();
}


void init()
{
	//specify all values for the color buffer; (red, green, blue, alpha)
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//can set viewing mode and projection here...not for first project
}

void display(void)
{
	// clear buffer to values set in glClearColor
	glClear(GL_COLOR_BUFFER_BIT);
	//replace current matrix with identity matrix
	glLoadIdentity();

	glDrawPixels(480, 360, GL_RGB, GL_UNSIGNED_BYTE, window);
	/////  use glDrawPixels instead
	glColor3f(1.0, 0.0, 1.0);
	
	////end block
	glutSwapBuffers();
	//force execution of glcommands
	glFlush();
}

int main(int argc, char* argv[])
{
	// initialize and run program
	//Line *line = new Line();
	
	// GLUT initialization
	glutInit(&argc, argv);
	//set initial display mode, also GLUT_DOUBLE  
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size (width, height)
	glutInitWindowSize(480,360);
	//set position, (x,y)
	glutInitWindowPosition(100, 100);
	//create with name
	glutCreateWindow("Triangle Rasterizer");
	//optional function "init"
	init();
	read();
	drawPoly();
	rastPoly();
	glutDisplayFunc(display);
	
	//set the callback function, called with each redisplay
	//enter processing loop (never ending)
	glutMainLoop();

	return 0;
}