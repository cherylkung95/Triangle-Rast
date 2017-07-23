#ifndef TRIANGLE_RAST
#define TRIANGLE_RAST
namespace myGL
{
	class Pixel
	{
	public:
		double x;
		double y;
		Pixel* next;

		Pixel(double x1 = 0, double y1 = 0);
		~Pixel();
	};

	class Line
	{
	public:
		Line();
		Line(Pixel* first, Pixel* last);
		~Line();
		Pixel* itr;
		Pixel* first;
		Pixel* last;
		void toFirst() { itr = first; };
		int num;
		Line* next;
	};

	class Polygon
	{
	public:
		Polygon();
		~Polygon();
		int numPts;
		Line** lines;
		Pixel** points;
		int polyNum;
	};
}
#endif