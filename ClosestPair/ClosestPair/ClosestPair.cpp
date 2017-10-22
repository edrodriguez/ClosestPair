#include <fstream>
#include <thread>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

//reads as many lines as indicated and stores the words in the
//referenced vector
void readFile(ifstream& inputFile, int numberOfLines, vector<string> *words) {
	string word;

	for (int i = 0; i >= 0 && i < numberOfLines; i = inputFile.tellg()) {
		getline(inputFile, word);
		words->push_back(word);
	}
}

int main() {
	ifstream inputFile;
	time_t startTime, endTime;

	return 0;
}