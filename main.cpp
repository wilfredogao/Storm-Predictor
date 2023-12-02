#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
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

std::unordered_multimap<std::string, weatherData> populateMap(string csvFile) //read the file and allocate data to a multimap
{
    std::unordered_multimap<std::string, weatherData> tempMap;
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

void printMap(std::unordered_multimap<std::string, weatherData> weatherMap) //prints the multimap data
{
    auto it = weatherMap.begin();    //test to print first element
        std::cout << "State: " << it->first << "\n"
                  << "Year: " << it->second.year << "\n"
                  << "Event: " << it->second.eventType << "\n"
                  << "Direct Injuries: " << it->second.injuriesDirect << "\n"
                  << "Indirect Injuries: " << it->second.injuriesIndirect << "\n"
                  << "Direct Deaths: " << it->second.deathsDirect << "\n"
                  << "Indirect Deaths: " << it->second.deathsIndirect << "\n"
                  << "Property Damage: " << it->second.propertyDamage << "\n"
                  << "Tor Scale: " << it->second.torScale << "\n\n";

    /*
    for(auto& data :weatherMap) //print all elements
    {
        std::cout << "State: " << data.first << "\n"
                  << "Year: " << data.second.year << "\n"
                  << "Event: " << data.second.eventType << "\n"
                  << "Direct Injuries: " << data.second.injuriesDirect << "\n"
                  << "Indirect Injuries: " << data.second.injuriesIndirect << "\n"
                  << "Direct Deaths: " << data.econd.deathsDirect << "\n"
                  << "Indirect Deaths: " << data.second.deathsIndirect << "\n"
                  << "Property Damage: " << data.second.propertyDamage << "\n"
                  << "Tor Scale: " << data.second.torScale << "\n\n";
    }*/
}

int main() 
{
    std::unordered_multimap<std::string, weatherData> weatherMap = populateMap("filteredWeatherData.csv");
    printMap(weatherMap);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}


/*
//basic merge sort function
unordered_multimap<string, weatherData> mergeSort(unordered_multimap<string, weatherData> map) {
    if length(map) < = 1;
        return map;
    int mid = length(map)/2; //divide map in half and recursively sort each half
    unordered_multimap<string, weatherData> left = mergeSort(); //recursively sort the left half of data
    unordered_multimap<string, weatherData> right = mergeSort(); //recursively sort the right half of data
    return merge(left, right);
}

//merge function for merge sort
unordered_multimap<string, weatherData> merge(unordered_multimap<string, weatherData> left, unordered_multimap<string, weatherData> right) {
    unordered_multimap<string, weatherData> result; //create empty map to store results
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
unordered_multimap<string, weatherData> quickSort (unordered_multimap<string, weatherData> map, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(map, low, high);
        quickSort(map, low, pivotIndex-1); //recursively sort the left section
        quickSort(map, pivotIndex+1, high); //recursively sort the right section
    }
    return map;
}

//partition function for quick sort
int partition (unordered_multimap<string, weatherData> map, int low, int high) {
    string pivot = map[high]; //choose the rightmost element as pivot
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
}
*/
