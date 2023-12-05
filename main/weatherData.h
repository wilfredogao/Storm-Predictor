#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;

class weatherData //weather data to store the associated data in the map
{
private:
    string year;
    string eventType;
    string injuriesDirect;
    string injuriesIndirect;
    string deathsDirect;
    string deathsIndirect;
    string propertyDamage;
    string torScale;

public:
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

    //accessors
    std::string getYear()
    {
        return year;
    }

    std::string getEvent()
    {
        string temp = "";
        if(eventType == "-100") //set blanks to -100 in data processing
        {
            return temp = "No event recorded";
        }
        else
            return temp = eventType;
    }

    std::string getInjuries()
    {
        std::string temp = "";
        try
        {
            if (injuriesDirect == "-100")
            {
                injuriesDirect = "0";
            }
            if (injuriesIndirect == "-100")
            {
                injuriesIndirect = "0";
            }

            temp = std::to_string(std::stoi(injuriesDirect) + std::stoi(injuriesIndirect));
        }
        catch (const std::invalid_argument& e)
        {
            temp = "0";
        }

        return temp;
    }

    std::string getDeaths()
    {
        std::string temp = "";
        try
        {
            if (deathsDirect == "-100")
            {
                deathsDirect = "0";
            }
            if (deathsIndirect == "-100")
            {
                deathsIndirect = "0";
            }

            temp = std::to_string(std::stoi(deathsDirect) + std::stoi(deathsIndirect));
        }
        catch (const std::invalid_argument& e)
        {
            temp = "0";
        }

        return temp;
    }

    std::string getPropertyDMG()
    {
        string temp = "";
        if(propertyDamage == "-100") //set blanks to -100 in data processing
        {
            return temp = "0";
        }
        else
        if(propertyDamage.substr(propertyDamage.size()-1, 1) == "0"){
            temp = "0";
        }
        else if(propertyDamage.at(propertyDamage.size() - 1) == 'K'){
            temp = to_string((int)stof(propertyDamage.substr(0, propertyDamage.size() - 1)) * 1000);
        }
        else if(propertyDamage.at(propertyDamage.size() - 1) == 'M'){
            temp = to_string((int)stof(propertyDamage.substr(0, propertyDamage.size() - 1)) * 1000000);
        }
        else{
            temp = propertyDamage;
        }
        return temp;
    }

    std::string getTorScale() //returns tor scale https://www.ncei.noaa.gov/pub/data/swdi/stormevents/csvfiles/Storm-Data-Bulk-csv-Format.pdf
    {
        string temp = "";
        if(torScale == "-100") //set blanks to -100 in data processing
        {
            return temp = "";
        }
        else if(torScale == "F0")
            return temp = "Tor Scale: Light Damage (40 to 72 mph)";
        else if(torScale == "F1")
            return temp = "Tor Scale: Moderate Damage (73 to 112 mph)";
        else if(torScale == "F2")
            return temp = "Tor Scale: Significant damage (113 to 157 mph)";
        else if(torScale == "F3")
            return temp = "Tor Scale: Severe Damage (158 to 206 mph)";
        else if(torScale == "F4")
            return temp = "Tor Scale: Devastating Damage (207 to 260 mph)";
        else if(torScale == "F5")
            return temp = "Tor Scale: Incredible Damage (261 to 318 mph)";
    }

    //basic merge sort function
    /* vector<weatherData> mergeSort(vector<weatherData> stateData) {
        if (stateData.size() <= 1)
            return stateData;
        int mid = floor(stateData.size()/2); //divide map in half and recursively sort each half
        int midYear = std::stoi(stateData.at(mid-1).getYear()); //remember index starts at 0, not 1
        vector<weatherData> leftMap;
        vector<weatherData> rightMap;

        //check if all data points are same year
        int sameYearCount = 0;
        for (int j = 0; j < stateData.size(); j++) {
            if (stoi(stateData[j].getYear()) == midYear) {
                sameYearCount++;
            }
        }
        //if all data points have the same year, split based on current index
        if (sameYearCount == stateData.size()) {
            for (int k = 0; k < stateData.size(); k++) {
                if (k < mid) {
                    leftMap.push_back(stateData[k]);
                }
                else {
                    rightMap.push_back(stateData[k]);
                }
            }
        }
        else {
            float midSplit = midYear + 0.5; // make sure midYear is between values so all indexes are split
            //ie: 1992 1992 1994 -> midYear = 1992 -> midYear = 1992.5 -> leftMap = [1992, 1992], rightMap = [1994]
            for (int i = 0; i < stateData.size(); i++) {
                if (std::stof(stateData[i].getYear()) < midSplit) {
                    leftMap.push_back(stateData[i]);
                } else {
                    rightMap.push_back(stateData[i]);
                }
            }
        }
        vector<weatherData> left = mergeSort(leftMap); //recursively sort the left half of data
        vector<weatherData> right = mergeSort(rightMap); //recursively sort the right half of data
        return merge(left, right);
    } */

    //merge function for merge sort
    /* vector<weatherData> merge(vector<weatherData> left, vector<weatherData> right) {
        vector<weatherData> result; //create empty map to store results
        while(!left.empty() && !right.empty()) {
            if (left[0].getYear() < right[0].getYear()) {
                result.push_back(left[0]);
                left.erase(left.begin());
            }
            else {
                result.push_back(right[0]);
                right.erase(right.begin());
            }
        }
        while(!left.empty()) {
            result.push_back(left[0]); //append remaining data points
            left.erase(left.begin());
        }
        while(!right.empty()) {
            result.push_back(right[0]);
            right.erase(right.begin());
        }
        return result;
    } */


    //basic quick sort function
    vector<weatherData> quickSort (vector<weatherData> stateData, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(stateData, low, high);
            quickSort(stateData, low, pivotIndex-1); //recursively sort the left section
            quickSort(stateData, pivotIndex+1, high); //recursively sort the right section
        }
        return stateData;
    }

    //partition function for quick sort
    int partition (vector<weatherData> stateData, int low, int high) {
        string pivot = stateData[high].getYear(); //choose the rightmost element as pivot
        int left = low;
        int right = high - 1;
        while (true) {
            while (left <= right && stateData[left].getYear() < pivot) {
                left = left + 1;
            }
            while (left <= right && stateData[right].getYear() > pivot) {
                right = right - 1;
            }
            if (left >= right) {
                break;
            }
            swap(stateData[left], stateData[right]);
        }
        swap(stateData[left], stateData[high]); //move pivot to the middle
        return left; //return partitioning index
    }

    vector<string> getPrediction(vector<weatherData> stateData, string year, bool &foundYear){
        unordered_map<string, int> events; //events stored as key, times the event happen as the value
        vector<weatherData> chosen; //the weather data that appears in the given time frame
        for (int i = 0; i < stateData.size();i++) {
            if ((stoi(year) - 10) < stoi(stateData.at(i).getYear()) && stoi(stateData.at(i).getYear()) <= stoi(year)) {
                foundYear = true;
                events[stateData.at(i).getEvent()]++;
                chosen.push_back(stateData.at(i));
            }
        }

        int max = 0; //most common event
        string mostEvent; //event that happens the most
        for(auto& data : events)
        {
            if(data.second > max){
                max = data.second; //if there is an event that happend the msot
                mostEvent = data.first; //the event
            }
        }
        //get the total data numbers from events of the most common event
        float totalDamage = 0.0;
        float totalInjured = 0.0;
        float totalDeath = 0.0;
        float totalEvents = 0.0; // total number of events
        for(int i = 0; i < chosen.size(); i++){
            if(chosen.at(i).getEvent() == mostEvent){
                totalDamage += stof(chosen.at(i).getPropertyDMG());
                totalInjured += stof(chosen.at(i).getInjuries());
                totalDeath += stof(chosen.at(i).getDeaths());
                totalEvents++;
            }
        }
        //get the average data
        int avgDamage = (totalDamage / totalEvents) + .5;
        int avgDeath = (totalDeath / totalEvents) + .5;
        int avgInjured = (totalInjured / totalEvents) + .5;

        vector<string> predictedData;
        predictedData.push_back("Predicted event: " + mostEvent);
        predictedData.push_back("Predicted amount of property damage: $" + to_string(avgDamage));
        predictedData.push_back("Predicted number of injuries: " + to_string(avgInjured));
        predictedData.push_back("Predicted number of deaths: " + to_string(avgDeath));

        return predictedData;
    }

    void heapSort(vector<weatherData> &stateData){
        int size = stateData.size();

        for(int i = (size / 2) - 1; i >= 0; i --){
            heapify(stateData, size, i);
        }

        for(int i = size - 1; i > 0; i--){
            swap(stateData.at(0), stateData.at(i));
            heapify(stateData, i, 0);
        }
    } //build in place a heap to sort state data

    void heapify(vector<weatherData> &stateData, int size, int i){
        int index = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if(left < size && stoi(stateData.at(left).getYear()) > stoi(stateData.at(index).getYear())){
            index = left;
        }

        if(right < size && stoi(stateData.at(right).getYear()) > stoi(stateData.at(index).getYear())){
            index = right;
        }

        if(index != i){
            swap(stateData.at(i),stateData.at(index));

            heapify(stateData, size, index);
        }
    } //heapify used for heap sort

};