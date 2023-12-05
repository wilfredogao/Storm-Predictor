#include "weatherData.h"
//this file includes all the functions that interact with weatherData.h

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
                auto begin = high_resolution_clock::now();
                //https://www.geeksforgeeks.org/measure-execution-time-function-cpp
                sorted = data.second.at(0).quickSort(data.second, 0, sorted.size() - 1);
                auto end = high_resolution_clock::now();
                auto sortTime = duration_cast<microseconds>(end - begin);
                cout << "Quick Sort execution: " << (double)sortTime.count() / 1000 << " milliseconds\n";
            }
            else if (sortChoice == "1"){
                sorted = data.second;
                auto begin = high_resolution_clock::now();
                //https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
                data.second.at(0).heapSort(sorted);
                auto end = high_resolution_clock::now();
                auto sortTime = duration_cast<microseconds>(end - begin);
                cout << "Heap Sort execution: " << (double)sortTime.count() / 1000 << " milliseconds\n";
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
    cout << "Hi! Welcome to the weather predictor!\n\n";
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
        cout << "\nWhich sorting algorithm would you like to use to sort through the data? Enter 0 for Quick Sort or 1 for Heap Sort\n";
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