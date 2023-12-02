#include <iostream>
#include <string>
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}


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
}
