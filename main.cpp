/*Alex Macedo
February 13th, 2015

This program displays a map of central Mexico with 7 cities.
The user starts at Acapulco and the final destination is
Mexico City. The program asks to click where their destination
is that they would like to drive to from their current position.
The program will read each click the user gives to display lines
that connect the previous location to the selected destination,
will draw a line between the departure and arrival locations,
will display the distance traveled between the two, and the graphic
of a truck that will always point towards Mexico City and will
resize to become larger the closer the user/route is to Mexico City.
The program will continue to read the user's clicks of infinite
destinations to display their route and distance traveled at each
leg until the user clicks within a 20km radius of Mexico City,
the final destination. Using the coordinates of the clicks along
the way, the program will count how many times the user's path
passes within a 50km radius of Cuernavaca, but a pass will not
count should the user's arrivala nd departure of a leg of travel
both be within this radius. Also, the coordinates of the clicks
are used by the program to calculate the cumulative distance
traveled by the user's entire route until Mexico City. Upon
clicking in the 20km radius of Mexico City, the program will
display the total number of passes through Cuernavaca, total
distanced traveled in km, and will ask the user if they would
like to do another route. If so, the map will clear the route
for the user to run the program infinitely many times for as 
many routes as they would like. If not, the program will terminate.*/

#include "ccc_win.h"
#include <cmath>

/*Functions*/
void drawMap();
void drawTruck(double xStart, double yStart, double xEnd, double yEnd, double distanceToMC);
void rotateTruck(double xStart, double yStart, double xEnd, double yEnd, double tireRadius,
	double bottomLeftX, double bottomLeftY, double farBottomRightX, double farBottomRightY,
	double topLeftX, double topLeftY, double topRightX, double topRightY, double middleRightX,
	double middleRightY, double farMiddleRightX, double farMiddleRightY, double bottomMiddleX,
	double bottomMiddleY, double leftTireX, double leftTireY, double rightTireX, double rightTireY);
void connectCities(double xStart, double yStart, double xEnd, double yEnd);
double distanceBetweenCities(double xStart, double yStart, double xEnd, double yEnd, double distanceTraveled);
double distanceToCuernavaca(double passes, double h, double k, double xStart, double yStart, double xEnd, double yEnd);
double distanceToPoint(double xStart, double yStart, double xEnd, double yEnd);
double rotateX(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint);
double rotateY(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint);
double reflectX(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint);
double reflectY(double xStart, double yStart, double xEnd, double yEnd,double xPoint, double yPoint);
int main();

/*Global Constants of Map Coordinates of Cities*/

const double CUERNAVACA_X = 0;
const double CUERNAVACA_Y = 0;
const double MEXICO_CITY_X = 0;
const double MEXICO_CITY_Y = 75;
const double ACAPULCO_X = -50;
const double ACAPULCO_Y = -225;
const double IXTAPA_X = -300;
const double IXTAPA_Y = -200;
const double SALAMANCA_X = -225;
const double SALAMANCA_Y = 250;
const double MORELIA_X = -250;
const double MORELIA_Y = 125;
const double CASITAS_X = 300;
const double CASITAS_Y = 225;

/*This function displays a map of Central MExico that is 
10x10 coordinate units. It contains hash marks to note every
200 km north to south and west to east, with Cuernavaca located
at the center and the other 6 cities displayed relative to it.
It also displays the circles around Cuernavaca and Mexico City
so the user can visualize when its route passes through the 50 km
radius around Cuernavaca and when the user selects a destination 
within 20 km of Mexico City */
void drawMap(){

	//Radius of the board
	const double R = 10;

	//Horizontal Lines
	cwin<< Line (Point(-R, R), Point(R, R));
	cwin<< Line (Point(-R, -R), Point(R, -R));

	//Vertical Lines
	cwin << Line (Point(-R, R), Point(-R, -R));
	cwin << Line (Point(R, R), Point(R, -R));

	//Vertical Tick Marks on Top
	cwin << Line (Point(-R*4/5, R), Point(-R*4/5, R-.25));
		cwin << Message(Point(-R*4/5, R-.25), "-400");
	cwin << Line (Point(-R*3/5, R), Point(-R*3/5, R-.25));
	cwin << Line (Point(-R*2/5, R), Point(-R*2/5, R-.25));
		cwin << Message(Point(-R*2/5, R-.25), "-200");
	cwin << Line (Point(-R/5, R), Point(-R/5, R-.25));
	cwin << Line (Point(0, R), Point(0, R-.25));
		cwin << Message(Point(0, R-.25), "0");
	cwin << Line (Point(R/5, R), Point(R/5, R-.25));
	cwin << Line (Point(R*2/5, R), Point(R*2/5, R-.25));
		cwin << Message(Point(R*2/5, R-.25), "200");
	cwin << Line (Point(R*3/5, R), Point(R*3/5, R-.25));
	cwin << Line (Point(R*4/5, R), Point(R*4/5, R-.25));
		cwin << Message(Point(R*4/5, R-.25), "400");

	//Vertical Tick Marks on Bottom
	cwin << Line(Point(-R * 4 / 5, -R), Point(-R * 4 / 5, -R + .25));
	cwin << Line(Point(-R * 3 / 5, -R), Point(-R * 3 / 5, -R + .25));
	cwin << Line(Point(-R * 2 / 5, -R), Point(-R * 2 / 5, -R + .25));
	cwin << Line(Point(-R / 5, -R), Point(-R / 5, -R + .25));
	cwin << Line(Point(0, -R), Point(0, -R + .25));
	cwin << Line(Point(R / 5, -R), Point(R / 5, -R + .25));
	cwin << Line(Point(R * 2 / 5, -R), Point(R * 2 / 5, -R + .25));
	cwin << Line(Point(R * 3 / 5, -R), Point(R * 3 / 5, -R + .25));
	cwin << Line(Point(R * 4 / 5, -R), Point(R * 4 / 5, -R + .25));

	//Horizontal Tick Marks on Left
	cwin << Line(Point(-R, R*4/5), Point(-R+.25, R*4/5));
		cwin << Message(Point(-R + .25, R * 4 / 5), "400");
	cwin << Line(Point(-R, R * 3 / 5), Point(-R + .25, R * 3 / 5));
	cwin << Line(Point(-R, R * 2 / 5), Point(-R + .25, R * 2 / 5));
		cwin << Message(Point(-R + .25, R * 2 / 5), "200");
	cwin << Line(Point(-R, R/5), Point(-R+.25, R/5));
	cwin << Line(Point(-R, 0), Point(-R+.25, 0));
		cwin << Message(Point(-R+.25, 0), "0");
	cwin << Line(Point(-R, -R/5), Point(-R+.25, -R/5));
	cwin << Line(Point(-R, -R * 2 / 5), Point(-R + .25, -R * 2 / 5));
		cwin << Message(Point(-R + .25, -R * 2 / 5), "-200");
	cwin << Line(Point(-R, -R * 3 / 5), Point(-R + .25, -R *3 / 5));
	cwin << Line(Point(-R, -R * 4 / 5), Point(-R + .25, -R * 4 / 5));
		cwin << Message(Point(-R + .25, -R * 4 / 5), "-400");

	//Horizontal Tick Marks on Right
	cwin << Line(Point(R, R * 4 / 5), Point(R - .25, R * 4 / 5));
	cwin << Line(Point(R, R * 3 / 5), Point(R - .25, R * 3 / 5));
	cwin << Line(Point(R, R * 2 / 5), Point(R - .25, R * 2 / 5));
	cwin << Line(Point(R, R / 5), Point(R - .25, R / 5));
	cwin << Line(Point(R, 0), Point(R - .25, 0));
	cwin << Line(Point(R, -R / 5), Point(R - .25, -R / 5));
	cwin << Line(Point(R, -R * 2 / 5), Point(R - .25, -R * 2 / 5));
	cwin << Line(Point(R, -R * 3 / 5), Point(R - .25, -R * 3 / 5));
	cwin << Line(Point(R, -R * 4 / 5), Point(R - .25, -R * 4 / 5));

	//Scaling Map Coordinates to Graph Coordinates
	const double BOARD_DOMAIN = 500;
	const double SCALE = R/BOARD_DOMAIN;

	//Locations of Cities
	cwin << Point(0, 0);
	cwin << Message(Point(CUERNAVACA_X, CUERNAVACA_Y), "Cuernavaca");
	cwin << Point(MEXICO_CITY_X, SCALE * MEXICO_CITY_Y);
	cwin << Message(Point(MEXICO_CITY_X, SCALE * MEXICO_CITY_Y), "Mexico City");
	cwin << Point(SCALE*ACAPULCO_X, SCALE*ACAPULCO_Y);
	cwin << Message(Point(SCALE*ACAPULCO_X, SCALE*ACAPULCO_Y), "Acapulco");
	cwin << Point(SCALE * IXTAPA_X, SCALE * IXTAPA_Y);
	cwin << Message(Point(SCALE*IXTAPA_X, SCALE*IXTAPA_Y), "Ixtapa");
	cwin << Point(SCALE * SALAMANCA_X, SCALE * SALAMANCA_Y);
	cwin << Message(Point(SCALE*SALAMANCA_X, SCALE*SALAMANCA_Y), "Salamanca");
	cwin << Point(SCALE * MORELIA_X, SCALE * MORELIA_Y);
	cwin << Message(Point(SCALE*MORELIA_X, SCALE*MORELIA_Y), "Morelia");
	cwin << Point(SCALE * CASITAS_X, SCALE * CASITAS_Y);
	cwin << Message(Point(SCALE*CASITAS_X, SCALE*CASITAS_Y), "Casitas");

	//Circles Around Mexico City and Cuernavaca
	Circle cuernavaca(Point(CUERNAVACA_X, CUERNAVACA_Y), SCALE * 50);
	Circle mexicoCity(Point(MEXICO_CITY_X, SCALE * MEXICO_CITY_Y), SCALE * 20);

	cwin << cuernavaca << mexicoCity;
}

/*This function reads the users click to select their next destination
and displays a graphic of a truck. The program will pass the calculated
distance that the selected destination is from Mexico City and will scale
the coordinate X and Y values of each point on the graphic of the truck
to resize the truck to be larger the closer it is to Mexico City and
smaller the farther away it is. It will call a function at the end
to rotate the graphic fo the truck towards Mexico City.*/
void drawTruck(double xStart, double yStart,double xEnd, double yEnd, double distanceToMC){
	
	const double R = 10;
	const double BOARD_DOMAIN = 500;
	const double SCALE = R / BOARD_DOMAIN;
	
	/*Scales to Resize the X and Y Coordinates of Truck Graphics
	based on distance from Mexico City to current location*/
	const double SCALE_X = 10/distanceToMC;
	const double SCALE_Y = 6/distanceToMC;

	/*Scaled X and Y Coordinates of Each Point of Truck Graphic based
	based on distance to Mexico City*/
	double bottomLeftX = xStart;
	double bottomLeftY = yStart;
	double farBottomRightX = xStart + SCALE_X * 1;
	double farBottomRightY = yStart;
	double topLeftX = xStart;
	double topLeftY = yStart + SCALE_Y*.5;
	double topRightX = xStart + SCALE_X*.75;
	double topRightY = yStart + SCALE_Y*.5;
	double middleRightX = xStart + SCALE_X*.75;
	double middleRightY = yStart + SCALE_Y*.25;
	double farMiddleRightX = xStart + SCALE_X * 1;
	double farMiddleRightY = yStart + SCALE_Y*.25;
	double bottomMiddleX = xStart + SCALE_X*.75;
	double bottomMiddleY = yStart;
	double leftTireX = xStart + SCALE_X*.1;
	double leftTireY = yStart - SCALE_Y*.1;
	double rightTireX = xStart + SCALE_X*.75;
	double rightTireY = yStart - SCALE_Y*.1;
	double tireRadius = SCALE_Y*.1;

	//Rotate the Truck Towards Mexico City
	rotateTruck(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y,tireRadius,
		bottomLeftX, bottomLeftY, farBottomRightX, farBottomRightY,
		topLeftX, topLeftY, topRightX, topRightY,middleRightX,
		middleRightY, farMiddleRightX, farMiddleRightY, bottomMiddleX,
		 bottomMiddleY, leftTireX, leftTireY, rightTireX, rightTireY);
}

/*This function passes all of the scaled X and Y coordinates of the truck
graphic to be located at the user's selected destination. It calls two 
functions that will rotate the X and Y coordinates of the points of the
truck to display the front end of the truck to point toward Mexico City.
If the user clicks a destination so that the truck will drive West,
the program will also call two functions to reflect the image of the truck
to face its front end to the left, as opposed to the right, to properly
orient the graphic.*/
void rotateTruck(double xStart, double yStart, double xEnd, double yEnd, double tireRadius,
				 double bottomLeftX, double bottomLeftY, double farBottomRightX, double farBottomRightY,
				 double topLeftX, double topLeftY, double topRightX, double topRightY, double middleRightX,
				 double middleRightY, double farMiddleRightX, double farMiddleRightY, double bottomMiddleX,
				 double bottomMiddleY, double leftTireX, double leftTireY, double rightTireX, double rightTireY){

		//Radius of the Board
		const double R = 10;
		const double BOARD_DOMAIN = 500;
		const double SCALE = R / BOARD_DOMAIN;	

		//Rotates the coordinates of the truck towards Mexico City
		double rotatedBottomLeftX = xStart;
		double rotatedBottomLeftY = yStart;
		double rotatedFarBottomRightX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, farBottomRightX, farBottomRightY);
		double rotatedFarBottomRightY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, farBottomRightX, farBottomRightY);
		double rotatedTopLeftX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, topLeftX, topLeftY);
		double rotatedTopLeftY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, topLeftX, topLeftY);
		double rotatedTopRightX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, topRightX, topRightY);
		double rotatedTopRightY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, topRightX, topRightY);
		double rotatedMiddleRightX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, middleRightX, middleRightY);
		double rotatedMiddleRightY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, middleRightX, middleRightY);
		double rotatedFarMiddleRightX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, farMiddleRightX, farMiddleRightY);
		double rotatedFarMiddleRightY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, farMiddleRightX, farMiddleRightY);
		double rotatedBottomMiddleX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, bottomMiddleX, bottomMiddleY);
		double rotatedBottomMiddleY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, bottomMiddleX, bottomMiddleY);
		double rotatedLeftTireX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, leftTireX, leftTireY);
		double rotatedLeftTireY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, leftTireX, leftTireY);
		double rotatedRightTireX = rotateX(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, rightTireX, rightTireY);
		double rotatedRightTireY = rotateY(xStart, yStart, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, rightTireX, rightTireY);

		//If the truck travels east, the coordinates are printed to display the rotated truck.
		if (xStart <= xEnd){

			//Horizontal Lines

			cwin << Line(Point(rotatedBottomLeftX, rotatedBottomLeftY), Point(rotatedFarBottomRightX, rotatedFarBottomRightY));
			cwin << Line(Point(rotatedTopLeftX, rotatedTopLeftY), Point(rotatedTopRightX, rotatedTopRightY));
			cwin << Line(Point(rotatedMiddleRightX, rotatedMiddleRightY), Point(rotatedFarMiddleRightX, rotatedFarMiddleRightY));

			//Veritcal Lines

			cwin << Line(Point(rotatedBottomLeftX, rotatedBottomLeftY), Point(rotatedTopLeftX, rotatedTopLeftY));
			cwin << Line(Point(rotatedBottomMiddleX, rotatedBottomMiddleY), Point(rotatedTopRightX, rotatedTopRightY));
			cwin << Line(Point(rotatedFarBottomRightX, rotatedFarBottomRightY), Point(rotatedFarMiddleRightX, rotatedFarMiddleRightY));

			//Tires

			cwin << Circle(Point(rotatedLeftTireX, rotatedLeftTireY), tireRadius);
			cwin << Circle(Point(rotatedRightTireX, rotatedRightTireY), tireRadius);
		}

		/*If the truck travels west, the rotated points are reflected about a line 
		perpendicular to the line of travel and displayed.*/
		else {

			double reflectedBottomLeftX = xStart;
			double reflectedBottomLeftY = yStart;
			double reflectedFarBottomRightX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedFarBottomRightX, rotatedFarBottomRightY);
			double reflectedFarBottomRightY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedFarBottomRightX, rotatedFarBottomRightY);
			double reflectedTopLeftX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedTopLeftX, rotatedTopLeftY);
			double reflectedTopLeftY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedTopLeftX, rotatedTopLeftY);
			double reflectedTopRightX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedTopRightX, rotatedTopRightY);
			double reflectedTopRightY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedTopRightX, rotatedTopRightY);
			double reflectedMiddleRightX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedMiddleRightX, rotatedMiddleRightY);
			double reflectedMiddleRightY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedMiddleRightX, rotatedMiddleRightY);
			double reflectedFarMiddleRightX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedFarMiddleRightX, rotatedFarMiddleRightY);
			double reflectedFarMiddleRightY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedFarMiddleRightX, rotatedFarMiddleRightY);
			double reflectedBottomMiddleX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedBottomMiddleX, rotatedBottomMiddleY);
			double reflectedBottomMiddleY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedBottomMiddleX, rotatedBottomMiddleY);
			double reflectedLeftTireX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedLeftTireX, rotatedLeftTireY);
			double reflectedLeftTireY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedLeftTireX, rotatedLeftTireY);
			double reflectedRightTireX = reflectX(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedRightTireX, rotatedRightTireY);
			double reflectedRightTireY = reflectY(xStart, yStart, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y, rotatedRightTireX, rotatedRightTireY);

			//Horizontal Lines

			cwin << Line(Point(reflectedBottomLeftX, reflectedBottomLeftY), Point(reflectedFarBottomRightX, reflectedFarBottomRightY));
			cwin << Line(Point(reflectedTopLeftX, reflectedTopLeftY), Point(reflectedTopRightX, reflectedTopRightY));
			cwin << Line(Point(reflectedMiddleRightX, reflectedMiddleRightY), Point(reflectedFarMiddleRightX, reflectedFarMiddleRightY));

			//Veritcal Lines

			cwin << Line(Point(reflectedBottomLeftX, reflectedBottomLeftY), Point(reflectedTopLeftX, reflectedTopLeftY));
			cwin << Line(Point(reflectedBottomMiddleX, reflectedBottomMiddleY), Point(reflectedTopRightX, reflectedTopRightY));
			cwin << Line(Point(reflectedFarBottomRightX, reflectedFarBottomRightY), Point(reflectedFarMiddleRightX, reflectedFarMiddleRightY));

			//Tires

			cwin << Circle(Point(reflectedLeftTireX, reflectedLeftTireY), tireRadius);
			cwin << Circle(Point(reflectedRightTireX, reflectedRightTireY), tireRadius);
		}			
}

/*This function passes the coordinates of the starting location and
destination that the user selects by clicking to draw a line
from starting point to end point.*/
void connectCities(double xStart, double yStart, double xEnd,double yEnd){

	cwin << Line(Point(xStart,yStart), Point(xEnd, yEnd));
}

/*This function passes the coordinates of the starting location and
destination that the user selects by clicking to calculate the
distance traveled between the two. The program will display this
distance, in km, at the destination. It will also keep a running
count of the total distance traveled by adding up the individual
distances of each leg traveled each time the suer selects a new
destination.*/
double distanceBetweenCities(double xStart, double yStart, double xEnd, double yEnd,double distanceTraveled){

	const double R = 10;
	const double BOARD_DOMAIN = 500;
	const double SCALE = R/ BOARD_DOMAIN;

	double distance = sqrt(pow((xStart - xEnd), 2) + pow((yStart - yEnd), 2));

	cwin << Point(xEnd, yEnd);
	cwin << Message(Point(xEnd+.5, yEnd-.5), distance/SCALE);

	distanceTraveled += (distance/SCALE);

	return distanceTraveled;
}

/*This program passes the coordinates of the starting location 
and desintation that the suer selects by clicking to calculate
whether or not the path traveled passes within 50 km of Cuernavaca.
If so, the number of passes will increment each time the user's
path goes through Cuernavaca. However, if at any point the user's
new starting location is in Cuernavaca and selects their next
destination to also be within the 50 km radius around it, this
will not be considered as a pass. In this case, the user would have
to select a destination outside of Cuernavaca and return to select
one inside the radius again for the number of passes to increment.*/
double distanceToCuernavaca(double passes, double h, double k, double xStart, double yStart, double xEnd, double yEnd){

	const double R = 10;
	const double BOARD_DOMAIN = 500;
	const double SCALE = R/ BOARD_DOMAIN;
	double Distance=0;

		double t;

		t = ((h - xStart)*(xEnd - xStart) + (k - yStart)*(yEnd - yStart)) / (pow((xEnd - xStart), 2) + pow((yEnd - yStart), 2));

		if (t <= 0){
			Distance = sqrt(pow((h - xStart), 2) + pow((k - yStart), 2));
		}

		else if (t > 0 && t < 1){
			Distance = sqrt(pow((h - xStart - t*xEnd + t*xStart), 2) + pow((k - yStart - t*yEnd + t*yStart), 2));
		}

		else if (t >= 1){
			Distance = sqrt(pow((h - xEnd), 2) + pow((k - yEnd), 2));
		}

		
		if (xStart>SCALE * 50 || xStart<-SCALE * 50 || yStart>SCALE * 50 || yStart < -SCALE * 50){
			if (Distance <= SCALE * 50){
				passes++;
				}
			}

		return passes;
}

/*This program passes the coordinates of two points to calculate
the distance between them. It will be used to calculate the
user's distance from their current location to Mexico City
to pass into the drawTruck function to resize the graphic of
the truck, based on this distance. This function will also
be used to calculate the distance from the user's location
to a coordinate of a point on the truck graphic to rotate
the coordinates to rotate the whole graphic towards Mexico City.*/
double distanceToPoint(double xStart, double yStart, double xEnd, double yEnd){

	double distance = sqrt(pow((xStart - xEnd), 2) + pow((yStart - yEnd), 2));

	return distance;
}

/*This function passes the coordinates of the starting location, 
destination that the user clicks, and the X and Y coordinates of
a point on the resized truck graphic. It will use these coordinates
to rotate the X coordinates of the points on the truck graphic to
point towards Mexico City.*/
double rotateX(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint){

	double lineSlope = (yEnd - yStart) / (xEnd - xStart);//lope of line from starting location to destination

	double lineAngle = atan(lineSlope);//angle of the path traveled compared to a horizontal line

	double pointSlope = (yPoint - yStart) / (xPoint - xStart);//slope of the line from point on truck graphic to starting location

	double pointAngle = atan(pointSlope);//angle of the slope of the line connecting the truck point to starting point

	double totalAngle = lineAngle + pointAngle;//adding together the two angles gives the total angle the coordinate must be
											   //rotated to point toward Mexico City

	double distance = distanceToPoint(xStart, yStart, xPoint, yPoint); //distance of the truck point to the starting point

	double x = (distance)*(cos(totalAngle))+xStart; //calculating the new coordinate for the rotated X to point towards
													//Mexico City

		return x;
}

/*This function passes the coordinates of the starting location,
destination that the user clicks, and the X and Y coordinates of
a point on the resized truck graphic. It will use these coordinates
to rotate the Y coordinates of the points on the truck graphic to
point towards Mexico City.*/
double rotateY(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint){

	double lineSlope = (yEnd - yStart) / (xEnd - xStart);

	double lineAngle = atan(lineSlope);

	double pointSlope = (yPoint - yStart) / (xPoint - xStart);

	double pointAngle = atan(pointSlope);

	double totalAngle = lineAngle + pointAngle;


	double distance = distanceToPoint(xStart, yStart, xPoint, yPoint);

	double y = (distance)*(sin(totalAngle))+yStart;

		return y;
}

/*This function passes the coordinates of the starting location,
destination that the user clicks, and the X and Y coordinates of
a point on the rotated truck graphic. It will use these coordinates
to reflect the X coordinates of the points on the truck graphic to
orient the truck to face to the left when the user selects a 
destination that is west of their starting location.*/
double reflectX(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint){

	double lineSlope = (yEnd - yStart) / (xEnd - xStart);//slope of the line between starting location and destination

	double perpSlope = -1 / lineSlope;//slope of the line perpendicular to apth of travel


	double d = (xPoint + (yPoint - (yStart - perpSlope*xStart))*perpSlope) /(1 + pow(perpSlope, 2));

	double x = 2 * d - xPoint; //formula to reflect X coordinate about the perpendicular line through the user's location

	return x;
}

/*This function passes the coordinates of the starting location,
destination that the user clicks, and the X and Y coordinates of
a point on the rotated truck graphic. It will use these coordinates
to reflect the Y coordinates of the points on the truck graphic to
orient the truck to face to the left when the user selects a
destination that is west of their starting location.*/
double reflectY(double xStart, double yStart, double xEnd, double yEnd, double xPoint, double yPoint){
	
	double lineSlope = (yEnd - yStart) / (xEnd - xStart);

	double perpSlope = -1 / lineSlope;


	double d = (xPoint + (yPoint - (yStart - perpSlope*xStart))*perpSlope) /(1 + pow(perpSlope, 2));

	double y = 2 * d*perpSlope - yPoint+2*(yStart-perpSlope*xStart);

	return y;
}

/*The main function will call the other functions to display the user's route,
the truck, calculate the distance traveled, and count the passes through Cuernavaca.*/
int ccc_win_main(){
	
	//Resizing the window
	cwin.coord(-12, 12, 12, -15);
	
	bool doRoute = true;

	const double R = 10;
	const double BOARD_DOMAIN= 500;
	const double SCALE = R / BOARD_DOMAIN;
	const double MEXICOCITYRADIUS = SCALE * 20;

	while (doRoute == true){

		double distanceToMC;
		double passes = 0;
		double distanceTraveled = 0;
		double xStart = -SCALE * 50;
		double yStart = -SCALE * 225;
		double xEnd = 0;
		double yEnd = 0;

		/*Blank map is displayed with user starting at Acapulco and the program asking
		the user to select their next destination. The program will do this until the 
		user selects a destination within 20 km of Mexico City on the map.*/
		drawMap();

		while (xEnd<-MEXICOCITYRADIUS || xEnd>MEXICOCITYRADIUS || yEnd<SCALE * MEXICO_CITY_Y - MEXICOCITYRADIUS || yEnd>SCALE * MEXICO_CITY_Y + MEXICOCITYRADIUS){

			Point destination = cwin.get_mouse("Click to select the next destination");

			//Coordinates of user's destination are stored
			xEnd = destination.get_x();
			yEnd = destination.get_y();

			/*Any click outside of the map will be rejected and the user will be
			asked to click a destination on the map until they do so.*/
			while (xEnd > 10 || xEnd < -10 || yEnd>10 || yEnd < -10){

				Point destination = cwin.get_mouse("I'm sorry! Please select a destination inside the map");

				xEnd = destination.get_x();
				yEnd = destination.get_y();
			}

			/*Line is drawn between starting location and destination*/
			connectCities(xStart, yStart, xEnd, yEnd);

			/*Distance in km between starting location and destination is calculated,
			displayed,and the total amount accumulates.*/
			distanceTraveled = distanceBetweenCities(xStart, yStart, xEnd, yEnd, distanceTraveled);

			/*The distance from the destination to Mexico City is calculated to use
			to resize the truck to be larger the closer it is to Mexico City.*/
			distanceToMC = distanceToPoint(xEnd, yEnd, MEXICO_CITY_X, SCALE*MEXICO_CITY_Y);
			
			/*The number of passes within 50 km of Cuernavaca is stored*/
			passes = distanceToCuernavaca(passes, CUERNAVACA_X, CUERNAVACA_Y, xStart, yStart, xEnd, yEnd);

			/*The truck is drawn, resized, roated, and reflected if the destination is west
			of the starting location. The truck is only displayed for every destination except
			for the last destination selected, Mexico City.*/
			if (xEnd<-MEXICOCITYRADIUS || xEnd>MEXICOCITYRADIUS || yEnd<SCALE * MEXICO_CITY_Y - MEXICOCITYRADIUS || yEnd>SCALE * MEXICO_CITY_Y + MEXICOCITYRADIUS){

				drawTruck(xEnd, yEnd, MEXICO_CITY_X, SCALE * MEXICO_CITY_Y, distanceToMC);
			}

			/*The destination coordinates now become the starting coordinates for
			when the user selects its next destination*/
			xStart = xEnd;
			yStart = yEnd;
		}

		//Total distance traveled and passes through Cuernavaca displayed
		cwin << Message(Point(-12, -11), "Welcome to Mexico City!");
		cwin << Message(Point(-12, -12), "Distance Traveled (km) =");
		cwin << Message(Point(-4.25, -12), distanceTraveled);
		cwin << Message(Point(-12, -13), "You pass within 50 km of Cuernavaca");
		cwin << Message(Point(-12, -14), passes);
		cwin << Message(Point(-8, -14), "times.");

		/*User is asked to run the program again. If yes, the route and trucks
		will clear to leave a blank map. If no, the program terminates.*/
		string replay = cwin.get_string("Would you like to do another route? Y/N");

		if (replay == "Y" || replay == "y" || replay == "Yes" || replay == "yes"){
			cwin.clear();
		}

		else if (replay == "N" || replay == "n" || replay == "No" || replay == "no"){
			doRoute = false;
			cwin.clear();
			cwin << Message(Point(-3, 0), "Have a safe drive!");
		}

		else {
			bool reask = true;
			while (reask == true){
				string replay = cwin.get_string("Try again! Would you like to do another route? Y/N");
				if (replay == "Y" || replay == "y" || replay == "Yes" || replay == "yes"){
					reask = false;
					cwin.clear();
				}

				else if (replay == "N" || replay == "n" || replay == "No" || replay == "no"){
					doRoute = false;
					reask = false;
					cwin.clear();
					cwin << Message(Point(-3, 0), "Have a safe drive!");
				}
			}
		}
	}
	return 0;
}