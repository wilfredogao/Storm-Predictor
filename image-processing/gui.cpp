#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include "CImg.h"
#include <Magick++.h>
using namespace std;
using namespace cimg_library;

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
                return temp = propertyDamage;
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
        vector<weatherData> mergeSort(vector<weatherData> stateData) {
            if (stateData.size() <= 1)
                return stateData;
            int mid = stateData.size()/2; //divide map in half and recursively sort each half
            int midYear = stoi(stateData.at(mid).getYear());
            vector<weatherData> leftMap;
            vector<weatherData> rightMap;
            for (int i = 0; i < stateData.size(); i++) {
                if (i < midYear) {
                    leftMap.push_back(stateData[i]);
                }
                else {
                    rightMap.push_back(stateData[i]);
                }
            }
            vector<weatherData> left = mergeSort(leftMap); //recursively sort the left half of data
            vector<weatherData> right = mergeSort(rightMap); //recursively sort the right half of data
            return merge(left, right);
        }

        //merge function for merge sort
        vector<weatherData> merge(vector<weatherData> left, vector<weatherData> right) {
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
        }


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
            unordered_map<string, int> events;
            vector<weatherData> chosen;
            for (int i = 0; i < stateData.size();i++) {
                if ((stoi(year) - 10) < stoi(stateData.at(i).getYear()) && stoi(stateData.at(i).getYear()) <= stoi(year)) {
                    foundYear = true;
                    events[stateData.at(i).getEvent()]++;
                    chosen.push_back(stateData.at(i));
                }
            }
            int max = 0;
            string mostEvent;
            for(auto& data : events) //looks through all elements
            {
                if(data.second > max){
                    max = data.second;
                    mostEvent = data.first;
                }
            }
            float avgDamage = 0.0;
            float avgInjured = 0.0;
            float avgDeath = 0.0;
            float total = 0.0; // total number of events
            for(int i = 0; i < chosen.size(); i++){
                if(chosen.at(i).getEvent() == mostEvent){
                    avgDamage += stof(chosen.at(i).getPropertyDMG());
                    avgInjured += stof(chosen.at(i).getInjuries());
                    avgDeath += stof(chosen.at(i).getDeaths());
                    total++;
                }
            }
            avgDamage = (avgDamage / total);
            avgDeath = (avgDeath / total);
            avgInjured = (avgInjured / total);

            vector<string> predictedData;
            predictedData.push_back("Most Likely Event: " + mostEvent);
            predictedData.push_back("Most likely Damage: " + to_string(avgDamage));
            predictedData.push_back("Most Likely Injured: " + to_string(avgInjured));
            predictedData.push_back("Most Likely Dead: " + to_string(avgDeath));

            return predictedData;

        }

};

std::map<std::string, vector<weatherData>> populateMap(string csvFile) //read the file and allocate data to a multimap
{
    std::map<std::string, vector<weatherData>> tempMap;
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

            if((injuryD != "0.0" && injuryD != "-100") && (dmg != "0.0" && dmg != "-100")) {
                weatherData data(year, event, injuryD, injuryInd, deathDirect, deathIndirect, dmg,tor); //create a dara object
                tempMap[state].push_back(data); //append the data object to the end of the states data vector
            }
        }
    }
    return tempMap;
}


void printMap(std::map<std::string, vector<weatherData>> weatherMap, string userState, string userYear, bool& foundState, bool& foundYear, string sortChoice) //prints the multimap data
{
    vector<weatherData> sorted;
    vector<string> prediciton;
    bool validData = false;
    for(auto& data :weatherMap) //looks through all elements
    {
        if (userState == data.first) {
            foundState = true;
            if(sortChoice == "0"){
                sorted = data.second.at(0).quickSort(data.second, 0, sorted.size() - 1);
            }
            else if (sortChoice == "1"){
                sorted = data.second.at(0).mergeSort(data.second);
            }
            prediciton = data.second.at(0).getPrediction(sorted, userYear, foundYear);
            for(int i = 0; i < prediciton.size(); i ++){
                cout << prediciton.at(i) << endl;
            }
        }
    }
}

void runProgram() //runs the program, keeps main clean 
{
    string inputState = "", inputYear = "", again = "", sort = "";
    bool run = true;
    bool stateFound = false;
    bool yearFound = false;
    std::map<std::string, vector<weatherData>> weatherMap = populateMap("filteredWeatherData.csv");
    do //run until user doesn't want to
    {
        stateFound = false;
        yearFound = false;
        cout << "Hi! Which state would you like weather data for?\n";
        cin >> inputState;
        transform(inputState.begin(), inputState.end(), inputState.begin(), ::toupper);
        //https://stackoverflow.com/questions/23418390/how-to-convert-a-c-string-to-uppercase
        cout << "\nGreat thanks for the information! For " + inputState + ", which year would you like weather data for?\n";
        cin >> inputYear;
        inputYear = inputYear+ ".0";
        cout << "\nWhich sorting algorihtm would you like to use? Enter 0 for Quick Sort or 1 for Merge Sort\n";
        cin >> sort;
        cout << "\nThanks, one moment while we prepare this data for you. Hope your day is going great!\n\n";
        printMap(weatherMap, inputState, inputYear, stateFound, yearFound, sort);

        if(!stateFound)
        {
            cout << "Thanks for waiting! Unfortunately, there is no data for that state.\n\n";
        }
        else if (!yearFound)
        {
            cout << "Thanks for waiting! Unfortunately, for " + inputState + " there is no data for that year.\n\n";
        }

        cout << "Would you like to enter another state and year to try again? (Y/N)\n";
        cin >> again;
        cout << "\n";
        transform(again.begin(), again.end(), again.begin(), ::toupper);
        if(again != "Y")
        {
            run = false;
        }

    }
    while(run);
}

int main()
{
    //display map image
    CImg<unsigned char> image("US-Blank-map.jpg");
    CImgDisplay main_disp(image,"Click a point");

    const unsigned char blue[] = {0, 0, 255};

    //show point on mouse
    while (!main_disp.is_closed()) {
        int xmouse = main_disp.mouse_x();
        int ymouse = main_disp.mouse_y();

        image.draw_circle(xmouse, ymouse, 15, blue);

        main_disp.display(image);
        main_disp.wait();
    }

    runProgram();
    return 0;
}
