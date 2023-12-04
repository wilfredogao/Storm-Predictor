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
using namespace std;

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
                    temp = to_string(stoi(propertyDamage.substr(0, propertyDamage.size() - 1)) * 1000);
                }
                else if(propertyDamage.at(propertyDamage.size() - 1) == 'M'){
                    temp = to_string(stoi(propertyDamage.substr(0, propertyDamage.size() - 1)) * 1000000);
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
        vector<weatherData> mergeSort(vector<weatherData> stateData) {
            if (stateData.size() <= 1)
                return stateData;
            int mid = floor(stateData.size()/2); //divide map in half and recursively sort each half
            int midYear = std::stoi(stateData.at(mid).getYear());
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
                for (int i = 0; i < stateData.size(); i++) {
                    if (std::stoi(stateData[i].getYear()) <= midYear) {
                        leftMap.push_back(stateData[i]);
                    } else {
                        rightMap.push_back(stateData[i]);
                    }
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

            if((injuryD != "0.0" && injuryD != "-100") && (dmg != "0K" && dmg != "-100")) {
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
            string year = userYear.substr(0, 4);
            if(foundYear){
                cout << "Here is the predicted data for " + userState + " in " + year + ": " << endl;
                for(int i = 0; i < prediciton.size(); i ++) {
                    cout << prediciton.at(i) << endl;
                }
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
    cout << "Hello! Welcome to the weather predictor!\n\n";
    do //run until user doesn't want to
    {
        stateFound = false;
        yearFound = false;
        cout << "Which state would you like see data for?\n";
        cin >> inputState;
        transform(inputState.begin(), inputState.end(), inputState.begin(), ::toupper);
        //https://stackoverflow.com/questions/23418390/how-to-convert-a-c-string-to-uppercase
        cout << "\nGreat thanks for the information!\nFor " + inputState + ", which year (from 1950 to 1999) would you like to get the predicted data for?\n";
        cin >> inputYear;
        inputYear = inputYear+ ".0";
        cout << "\nWhich sorting algorithm would you like to use to sort through the data? Enter 0 for Quick Sort or 1 for Merge Sort\n";
        cin >> sort;
        cout << "\nThanks, one moment while we prepare this data for you. Hope your day is going great!\n\n";

        printMap(weatherMap, inputState, inputYear, stateFound, yearFound, sort);

        if(!stateFound)
        {
            cout << "Thanks for waiting! Unfortunately, there is no data for that state.\n";
        }
        else if (!yearFound)
        {
            cout << "Thanks for waiting! Unfortunately, for " + inputState + " there is no data for that year.\n";
        }

        cout << "\nWould you like to enter another state and year to try again? (Y/N)\n";
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
    runProgram();
    return 0;
}
