#include <fstream>
#include <thread>
#include <ctime>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;

struct Point {
	double x, y;
};

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
double bruteForce(vector<Point> *points, int start, int end, int *numComparissons)
{
	double min = DBL_MAX;
	for (int i = start; i < end; ++i) {
		for (int j = i + 1; j < end; ++j) {
			if (distance(&(*points)[i], &(*points)[j]) < min)
				min = distance(&(*points)[i], &(*points)[j]);
			(*numComparissons)++;
		}
	}
			
	return min;
}

// A utility function to find the distance beween the closest points of
// strip of given size. All points in strip[] are sorted accordint to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
double stripClosest(vector<Point> *strip, int size, double d, int *numComparissons)
{
	double min = d;  // Initialize the minimum distance as d

	sort((*strip).begin(), (*strip).end(), compareY);

	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size - 1; ++i) {
		for (int j = i + 1; j < size && ((*strip)[j].y - (*strip)[i].y) < min; ++j) {
			if (distance(&(*strip)[i], &(*strip)[j]) < min)
				min = distance(&(*strip)[i], &(*strip)[j]);
			(*numComparissons)++;
		}
	}

	return min;
}

// A recursive function to find the smallest distance. The array P contains
// all points sorted according to x coordinate
double closestUtil(vector<Point> *points, int start, int end, int *numComparissons)
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
	double dl = closestUtil(points, start, mid, numComparissons);
	double dr = closestUtil(points, mid + 1, end, numComparissons);

	// Find the smaller of two distances
	double d = min(dl, dr);

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	vector<Point> strip;
	int j = 0;
	for (int i = start; i < end; i++) {
		if (abs((*points)[i].x - midPoint.x) < d) {
			strip.push_back((*points)[i]);
			j++;
		}
	}

	// Find the closest points in strip.  Return the minimum of d and closest
	// distance is strip[]
	return min(d, stripClosest(&strip, j, d, numComparissons));
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
void closest(vector<Point> *points, double *distance, int *numComparissons)
{
	sort(points->begin(), points->end(), compareX);

	// Use recursive function closestUtil() to find the smallest distance
	*distance = closestUtil(points, 0, points->size() - 1, numComparissons);
}

int main() {
	ifstream inputFile;
	time_t startTime, endTime;
	double totalSeconds, halvesSeconds, quarterSeconds;
	vector<Point> Q1Points, Q2Points, Q3Points, Q4Points;
	int Q1Comparissons, Q2Comparissons, Q3Comparissons, Q4Comparissons,
		firstHalfComparissons, secondHalfComparissons, totalComparissons;

	inputFile.open("Million Points 4 Decimals.txt");
	//inputFile.open("10Points.txt");

	if (inputFile.good()) {
		//get length of file
		inputFile.seekg(0, inputFile.end);
		int fileLength = inputFile.tellg();
		inputFile.seekg(0, inputFile.beg);

		//read file in 4 parts
		readFile(inputFile, fileLength /*/ 4*/, &Q1Points);
		//readFile(inputFile, fileLength / 2, &Q2Points);
		//readFile(inputFile, 3 * fileLength / 4, &Q3Points);
		//readFile(inputFile, fileLength, &Q4Points);
		inputFile.close();
		
		//start sorting each quarter in parallel
		time(&startTime);
		//thread Q1(mergeSort, &Q1Words, 0, Q1Words.size() - 1, &Q1Comparissons);
		double d = 0;
		Q1Comparissons = 0;
		closest(&Q1Points, &d, &Q1Comparissons);
		//thread Q1(closest, &Q1Points, &d);

		//synchronize threads
		//Q1.join();
		time(&endTime);
		quarterSeconds = difftime(endTime, startTime);
	}

	return 0;
}