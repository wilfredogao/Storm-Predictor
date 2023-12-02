#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

class weatherData //weather data to store the associated data in the map
{
    public:
        string year;
        string eventType;
        string injuriesDirect;
        string injuriesIndirect;
        string deathsDirect;
        string deathsIndirect;
        string propertyDamage;
        string torScale;

    //constructor
    weatherData(string y, string event, string injuriesD, string injuriesInd, string deathD, string deathInd, string propertyD, string TorS):
        year(y), 
        eventType(event), 
        injuriesDirect(injuriesD), 
        injuriesIndirect(injuriesInd), 
        deathsDirect(deathD), 
        deathsIndirect(deathInd), 
        propertyDamage(propertyD), 
        torScale(TorS){}
};

std::multimap<std::string, weatherData> populateMap(string csvFile)
{
    std::multimap<std::string, weatherData> tempMap;
    fstream file(csvFile, ios::in);
    string line;

    if(file.is_open())
    {
        while(getline(file, line))
        {
            stringstream str(line);
            string state, event, dmg, tor, year, injuryD, injuryInd, deathDirect, deathIndirect;

            getline(str, state, ',');
            getline(str, year, ',');
            getline(str, event, ',');
            getline(str, injuryD, ',');
            getline(str, injuryInd, ',');
            getline(str, deathDirect, ',');
            getline(str, deathIndirect, ',');
            getline(str, dmg, ',');
            getline(str, tor, ',');

            tempMap.insert({state, weatherData(year, event, injuryD, injuryInd, deathDirect, deathIndirect, dmg, tor)});
        }
    }

    return tempMap;
}

void printMap(std::multimap<std::string, weatherData> weatherMap)
{
    for(auto& data :weatherMap)
    {
        std::cout << "State: " << data.first << std:: endl;
    }
}

int main() 
{
    std::multimap<std::string, weatherData> weatherMap = populateMap("filteredWeatherData.csv");
    printMap(weatherMap);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

/*
//basic merge sort function
string <pair<string, string>> mergeSort(string <pair<string, string>> map) { //edit based on type of map used and return type
    if length(map) < = 1;
        return map;
    int mid = length(map)/2; //divide map in half and recursively sort each half
    string<pair<string, string>> left = mergeSort(); //recursively sort the left half of data
    string<pair<string, string>> right = mergeSort(); //recursively sort the right half of data
    return merge(left, right);
}

//merge function for merge sort
string<pair<string, string>> merge(string<pair<string, string>> left, string<pair<string, string>> right) {
    string <pair<string, string>> result; //create empty map to store results
    while(!left.empty() && !right.empty()) {
        if (left[0] < right[0]) {
            result.append(left[0]);
            left = left[1:];
        }
        else {
            result.append(right[0]);
            right = right[1:];
        }
    }
    result.extend(left); //append remaining data points
    result.extend(right);
    return result;
}


//basic quick sort function
string<pair<string, string>> quickSort (string<pair<string, string>> map, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(map, low, high);
        quickSort(map, low, pivotIndex-1); //recursively sort the left section
        quickSort(map, pivotIndex+1, high); //recursively sort the right section
    }
    return map;
}

//partition function for quick sort
int partition (string<pair<string, string>> map, int low, int high) { //edit all based on variable names and return types
    pair<string, string> pivot = map[high]; //choose the rightmost element as pivot
    int left = low;
    int right = high - 1;
    while (true) {
        while (left <= right && map[left] < pivot) {
            left = left + 1;
        }
        while (left <= right && map[right] > pivot) {
            right = right - 1;
        }
        if (left >= right) {
            break;
        }
        swap(map[left], map[right]);
    }
    swap(map[left], map[high]) //move pivot to the middle
    return left; //return partitioning index
}*/
