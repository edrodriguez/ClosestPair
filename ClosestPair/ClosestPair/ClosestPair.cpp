#include <fstream>
#include <stdio.h>
#include <thread>
#include <ctime>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <string>
#include <math.h>

using namespace std;

struct Point {
	double x, y;
};

struct PairOfPoints {
	Point PointA, PointB;
	double distance;
}; PairOfPoints closestPairs[2];

void updateTopTwo(PairOfPoints p) {
	if (p.distance < closestPairs[0].distance) {
		closestPairs[1] = closestPairs[0];
		closestPairs[0] = p;
	}
	else if (p.distance < closestPairs[1].distance &&
		p.PointA.x != closestPairs[0].PointA.x &&
		p.PointA.y != closestPairs[0].PointA.y &&
		p.PointB.x != closestPairs[0].PointB.x &&
		p.PointB.y != closestPairs[0].PointB.y) {
		closestPairs[1] = p;
	}
}

//reads as many lines as indicated and stores the points in the
//referenced vector
void readFile(ifstream& inputFile, int numberOfLines, vector<Point> *points) {
	Point p;

	for (int i = 0; i >= 0 && i < numberOfLines; i = inputFile.tellg()) {
		inputFile >> p.x;
		inputFile >> p.y;
		points->push_back(p);
	}
}

// Needed to sort array of points according to X coordinate
bool compareX(Point a, Point b)
{
	return (a.x < b.x);
}

// Needed to sort array of points according to Y coordinate
double compareY(Point a, Point b)
{
	return (a.y < b.y);
}

double distance(Point *a, Point *b)
{
	return sqrt((a->x - b->x)*(a->x - b->x) +
		(a->y - b->y)*(a->y - b->y)
	);
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
PairOfPoints bruteForce(vector<Point> *points, int start, int end, int *numComparissons)
{	
	PairOfPoints pair1, pair2;
	double min = DBL_MAX;
	Point PointA;
	PointA.x = 0;
	PointA.y = 0;
	Point PointB;
	PointB.x = 0;
	PointB.y = 0;

	for (int i = start; i < end; ++i) {
		for (int j = i + 1; j < end; ++j) {
			if (distance(&(*points)[i], &(*points)[j]) < min) {
				min = distance(&(*points)[i], &(*points)[j]);
				PointA = (*points)[i];
				PointB = (*points)[j];
			}
			(*numComparissons)++;
		}
	}
	pair1.distance = min;
	pair1.PointA = PointA;
	pair1.PointB = PointB;

	return pair1;
}

// A utility function to find the distance beween the closest points of
// strip of given size. All points in strip[] are sorted accordint to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
PairOfPoints stripClosest(vector<Point> *strip, int size, double d, int *numComparissons)
{
	double min = d;  // Initialize the minimum distance as d
	Point PointA;
	PointA.x = 0;
	PointA.y = 0;
	Point PointB;
	PointB.x = 0;
	PointB.y = 0;
	PairOfPoints pair1;

	sort((*strip).begin(), (*strip).end(), compareY);

	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size - 1; ++i) {
		for (int j = i + 1; j < size && ((*strip)[j].y - (*strip)[i].y) < min; ++j) {
			if (distance(&(*strip)[i], &(*strip)[j]) < min) {
				min = distance(&(*strip)[i], &(*strip)[j]);
				PointA = (*strip)[i];
				PointB = (*strip)[j];
			}
			(*numComparissons)++;
		}
	}

	pair1.distance = min;
	pair1.PointA = PointA;
	pair1.PointB = PointB;

	return pair1;
}

// A recursive function to find the smallest distance. The array P contains
// all points sorted according to x coordinate
PairOfPoints closestUtil(vector<Point> *points, int start, int end, int *numComparissons)
{
	// If there are 2 or 3 points, then use brute force
	if (end - start <= 3)
		return bruteForce(points, start, end, numComparissons);

	// Find the middle point
	int mid = ((end - start) / 2) + start;
	Point midPoint = (*points)[mid];

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	PairOfPoints dl = closestUtil(points, start, mid, numComparissons);
	PairOfPoints dr = closestUtil(points, mid + 1, end, numComparissons);

	// Find the smaller of two distances
	PairOfPoints d;
	if (dl.distance <= dr.distance)
		d = dl;
	else
		d = dr;
	updateTopTwo(d);

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	vector<Point> strip;
	int j = 0;
	for (int i = start; i < end; i++) {
		if (abs((*points)[i].x - midPoint.x) < d.distance) {
			strip.push_back((*points)[i]);
			j++;
		}
	}

	// Find the closest points in strip.  Return the minimum of d and closest
	// distance is strip[]
	PairOfPoints s = stripClosest(&strip, j, d.distance, numComparissons);
	if (s.distance < d.distance) {
		return s;
		updateTopTwo(s);
	}
	else
		return d;
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
void closest(vector<Point> *points, int *numComparissons)
{
	sort(points->begin(), points->end(), compareX);

	// Use recursive function closestUtil() to find the smallest distance
	closestPairs[0] = closestUtil(points, 0, points->size() - 1, numComparissons);
}

int main() {
	ifstream inputFile;
	time_t startTime, endTime;
	double totalSeconds;
	vector<Point> points;
	int comparissons;

	inputFile.open("Million Points 4 Decimals.txt");
	//inputFile.open("10Points.txt");

	if (inputFile.good()) {
		//get length of file
		inputFile.seekg(0, inputFile.end);
		int fileLength = inputFile.tellg();
		inputFile.seekg(0, inputFile.beg);

		readFile(inputFile, fileLength, &points);
		inputFile.close();
		
		closestPairs[0].PointA.x = 0;
		closestPairs[0].PointA.y = 0;
		closestPairs[0].PointB.x = 0;
		closestPairs[0].PointB.y = 0;
		closestPairs[0].distance = DBL_MAX;
		closestPairs[1].PointA.x = 0;
		closestPairs[1].PointA.y = 0;
		closestPairs[1].PointB.x = 0;
		closestPairs[1].PointB.y = 0;
		closestPairs[1].distance = DBL_MAX;


		time(&startTime);
		comparissons = 0;
		closest(&points, &comparissons);
		time(&endTime);
		totalSeconds = difftime(endTime, startTime);
		
		//output
		printf("Number of comparissons: %d\n", comparissons);
		printf("The first closest pair is: (%.4f,%.4f) and (%.4f,%.4f). Distance of %.4f\n",
			closestPairs[0].PointA.x, closestPairs[0].PointA.y,
			closestPairs[0].PointB.x, closestPairs[0].PointB.y, closestPairs[0].distance);
		printf("The second closest pair is: (%.4f,%.4f) and (%.4f,%.4f). Distance of %.4f\n",
			closestPairs[1].PointA.x, closestPairs[1].PointA.y,
			closestPairs[1].PointB.x, closestPairs[1].PointB.y, closestPairs[1].distance);
		printf("Running time: %.0f seconds\n", totalSeconds);

	}

	return 0;
}