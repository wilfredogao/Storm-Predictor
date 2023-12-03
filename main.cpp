#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <map>
#include <vector>
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
                return temp = "$0";
            }
            else
                return temp = "$" + propertyDamage;
        }

        std::string getTorScale() //returns tor scale
        //https://www.ncei.noaa.gov/pub/data/swdi/stormevents/csvfiles/Storm-Data-Bulk-csv-Format.pdf 
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

void printMap(std::map<std::string, vector<weatherData>> weatherMap, string userState, string userYear, bool& foundState, bool& foundYear) //prints the multimap data
{
    bool firsttime = true;
    /*auto it = weatherMap.begin();    //test to print first element
    std::cout   << "State: " << it->first << "\n"
                << "Year: " << it->second.getYear() << "\n"
                << "Event: " << it->second.getEvent() << "\n"
                //<< "Direct Injuries: " << it->second.injuriesDirect << "\n"
                << "Resulting injuries: " << it->second.getInjuries() << "\n"
                //<< "Direct Deaths: " << it->second.deathsDirect << "\n"
                << "Resulting deaths: " << it->second.getDeaths() << "\n"
                << "Property Damage: " << it->second.getPropertyDMG() << "\n"
                << it->second.getTorScale() << "\n\n";
    */
    
    for(auto& data :weatherMap) //looks through all elements
    {
        if (userState == data.first) {
            foundState = true;
            for (int i = 0; i < data.second.size();i++) {
                if (userYear == data.second.at(i).getYear()) {
                    if(firsttime){
                        cout << "Thanks for waiting! Here's your weather report: " << endl;
                        firsttime = false;
                    }
                    foundYear = true;
                    std::cout << "State: " << data.first << "\n"
                              << "Year: " << data.second.at(i).getYear() << "\n"
                              << "Event: " << data.second.at(i).getEvent() << "\n"
                              //<< "Direct Injuries: " << it->second.injuriesDirect << "\n"
                              << "Resulting injuries: " << data.second.at(i).getInjuries() << "\n"
                              //<< "Direct Deaths: " << it->second.deathsDirect << "\n"
                              << "Resulting deaths: " << data.second.at(i).getDeaths() << "\n"
                              << "Property Damage: " << data.second.at(i).getPropertyDMG() << "\n"
                              << data.second.at(i).getTorScale() << "\n\n";
                }
            }
        }
    }
}

void runProgram() //runs the program, keeps main clean 
{
    string inputState = "", inputYear = "", again = "";
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
        cout << "\nThanks, one moment while we prepare this data for you. Hope your day is going great!\n\n";
        printMap(weatherMap, inputState, inputYear, stateFound, yearFound);

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
    runProgram();
    return 0;
}


/*
//basic merge sort function
unmultimap<string, weatherData> mergeSort(unmultimap<string, weatherData> map) {
    if (map.size() <= 1)
        return map;
    int mid = map.size()/2; //divide map in half and recursively sort each half
    unmultimap<string, weatherData> leftMap;
    unmultimap<string, weatherData> rightMap;
    unmultimap<string, weatherData>::iterator it;
    for (int i = 0; i < mid; i++) {
        leftMap.insert(map[i]);
    }
    for (int j = mid; j < map.size(); j++) {
        rightMap.insert(map[j]);
    }
    unmultimap<string, weatherData> left = mergeSort(leftMap); //recursively sort the left half of data
    unmultimap<string, weatherData> right = mergeSort(rightMap); //recursively sort the right half of data
    return merge(left, right);
}

//merge function for merge sort
unmultimap<string, weatherData> merge(unmultimap<string, weatherData> left, unmultimap<string, weatherData> right) {
    unmultimap<string, weatherData> result; //create empty map to store results
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
unmultimap<string, weatherData> quickSort (unmultimap<string, weatherData> map, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(map, low, high);
        quickSort(map, low, pivotIndex-1); //recursively sort the left section
        quickSort(map, pivotIndex+1, high); //recursively sort the right section
    }
    return map;
}

//partition function for quick sort
int partition (unmultimap<string, weatherData> map, int low, int high) {
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