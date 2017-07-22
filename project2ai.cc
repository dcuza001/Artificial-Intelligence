#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <ctime>


using namespace std;


struct dataEntry{
    double classNum;
    vector<double> features;
    dataEntry(){
        classNum = 0;
        for(int i =0; i < 10; ++i){
            features.push_back(0);
        }
    }
    dataEntry(int length){
        classNum = 0;
        for(int i =0; i < length; ++i){
            features.push_back(0);
        }
    }
    void print(){
        cout << "Class: " << classNum << endl;
        for(int i = 0; i < features.size(); ++i){
            cout << "Feature " << i << ": " << features[i] << endl;
        }
    }
    
};

double distanceForward(vector<dataEntry> data, vector<int> currentSetOfFeatures, int featureToAdd){
    double accuracy = 0;
    double minDistance;
    
    for(int i = 0; i < data.size(); ++i){
        minDistance = 0;
        int closestEntryToI;
        int j;
        for(j = 0; j < data.size(); ++j){//loop through all entries checking distance to ith entry
            if(j != i){
                long double sumToSquareRoot = 0;
                long double addedFeature = 0;
                for(int k = 0; k < currentSetOfFeatures.size(); ++k){
                    sumToSquareRoot += pow((data[i].features[currentSetOfFeatures[k]] -data[j].features[currentSetOfFeatures[k]]), 2);
                }
                sumToSquareRoot += pow((data[i].features[featureToAdd]-data[j].features[featureToAdd]), 2);
                //sumToSquareRoot = pow(sumToSquareRoot, 2);
                double distance = sqrt(sumToSquareRoot);
                if((distance < minDistance) || minDistance == 0){
                    minDistance = distance;
                    closestEntryToI = j;
                }
            }
            
        }
        if(data[i].classNum == data[closestEntryToI].classNum){
            ++accuracy;
        }
        
    }
    return accuracy/data.size();
}

double minWrong = 10000000000;

double distanceForwardBetter(vector<dataEntry> data, vector<int> currentSetOfFeatures, int featureToAdd){
    double accuracy = 0;
    double minDistance;
    double wrong = 0;
    for(int i = 0; i < data.size(); ++i){
        minDistance = 0;
        int closestEntryToI;
        int j;
        for(j = 0; j < data.size(); ++j){//loop through all entries checking distance to ith entry
            if(j != i){
                long double sumToSquareRoot = 0;
                long double addedFeature = 0;
                for(int k = 0; k < currentSetOfFeatures.size(); ++k){
                    sumToSquareRoot += pow((data[i].features[currentSetOfFeatures[k]] -data[j].features[currentSetOfFeatures[k]]), 2);
                }
                sumToSquareRoot += pow((data[i].features[featureToAdd]-data[j].features[featureToAdd]), 2);
                //sumToSquareRoot = pow(sumToSquareRoot, 2);
                double distance = sqrt(sumToSquareRoot);
                if((distance < minDistance) || minDistance == 0){
                    minDistance = distance;
                    closestEntryToI = j;
                }
            }
            
        }
        if(data[i].classNum == data[closestEntryToI].classNum){
            ++accuracy;
        }
        else{//early exit, save time
            ++wrong;
            if(wrong > minWrong)
                return 0;
        }
        
    }
    if(wrong < minWrong)
        minWrong = wrong;
    
    return accuracy/data.size();
}


double distanceBackward(vector<dataEntry> data, vector<int> currentSetOfFeatures, int featureToRemove){
    double accuracy = 0;
    double minDistance = 0;
    vector<int> modifiedSetOfFeatures;
    for(int i = 0; i < currentSetOfFeatures.size(); ++i){
        if(currentSetOfFeatures[i] != featureToRemove)
            modifiedSetOfFeatures.push_back(currentSetOfFeatures[i]);
    }
    //cout << "Modified size: " << modifiedSetOfFeatures.size() << endl;
    for(int i = 0; i < data.size(); ++i){
        minDistance = 0;
        int closestEntryToI;
        int j;
        for(j = 0; j < data.size(); ++j){//loop through all entries checking distance to ith entry
            if(j != i){
                double sumToSquareRoot = 0;
                double addedFeature = 0;
                for(int k = 0; k < modifiedSetOfFeatures.size(); ++k){
                    sumToSquareRoot += pow((data[i].features[modifiedSetOfFeatures[k]] -data[j].features[modifiedSetOfFeatures[k]]), 2);
                }
                //sumToSquareRoot += pow((data[i].features[featureToAdd]-data[j].features[featureToAdd]), 2);
                //sumToSquareRoot = pow(sumToSquareRoot, 2);
                double distance = sqrt(sumToSquareRoot);
                if((distance < minDistance) || minDistance == 0){
                    minDistance = distance;
                    closestEntryToI = j;
                }
            }
            
        }
        if(data[i].classNum == data[closestEntryToI].classNum){
            ++accuracy;
        }
        
    }
    return (accuracy/data.size());
}




void featureSearchBackward(vector<dataEntry> data){
    
    //don't know if i need this...
    vector<int> currentSetOfFeatures;
    for(int i = 0; i < data[0].features.size(); ++i){
        currentSetOfFeatures.push_back(i);
    }
    vector<int> bestSetOfFeatures;
    //set<int> currentSetOfFeatures;
    double bestAccuracyEVER = 0;
    
    for(int i = 0; i < data[0].features.size(); ++i){//tree level
        
        cout << "On the " << i+1 << "th level of the search tree." << endl;
        int featureToRemoveThisLevel;
        double bestAccuracySoFar = 0;
        
        for(int j = 0; j < data[0].features.size(); ++j){//
            //if feature isn't already in set, consider adding
            bool found = false;
            for(int z = 0; z < currentSetOfFeatures.size(); ++z){
                if(currentSetOfFeatures[z] == j)
                    found = true;
            }
            if(found){
                
                cout << "Considering removing the " << j+1 << " feature..." << endl;
                double accuracy = distanceBackward(data, currentSetOfFeatures, j);
                //cout << "ACCURACY: " << accuracy << endl;
                //cout << "After LeaveONEOUT: " << j << endl;
                if (accuracy > bestAccuracySoFar){
                    //cout << "New Best Accuracy: " << accuracy << endl;
                    bestAccuracySoFar = accuracy;
                    featureToRemoveThisLevel = j;//should this be an array???
                }
            }
            
        }
        
        cout << "REMOVING FEATURE: " << featureToRemoveThisLevel+1 << " at level: " << i+1 << endl;
        cout << "Gives Accuracy: " << bestAccuracySoFar << endl;
        vector<int> modifiedSetOfFeatures;
        for(int i = 0; i < currentSetOfFeatures.size(); ++i){
            if(currentSetOfFeatures[i] != featureToRemoveThisLevel)
                modifiedSetOfFeatures.push_back(currentSetOfFeatures[i]);
        }
        currentSetOfFeatures = modifiedSetOfFeatures;
        if(currentSetOfFeatures.size() == 2){//best 2 combos are last 2 left because removing
            bestSetOfFeatures = currentSetOfFeatures;
            bestAccuracyEVER = bestAccuracySoFar;
        }
        if(bestAccuracySoFar > bestAccuracyEVER){
            bestSetOfFeatures = currentSetOfFeatures;
            bestAccuracyEVER = bestAccuracySoFar;
            
        }
        
    }
    cout << "Best Set(" << bestAccuracyEVER << "): ";
    for(int i = 0; i < bestSetOfFeatures.size(); ++i){
        cout << bestSetOfFeatures[i]+1 << ", ";
    }
    cout << endl;

}




void featureSearch(vector<dataEntry> data){
    
    //don't know if i need this...
    vector<int> currentSetOfFeatures;
    vector<int> bestSetOfFeatures;
    //set<int> currentSetOfFeatures;
    double bestAccuracyEVER = 0;
    
    for(int i = 0; i < data[0].features.size(); ++i){//tree level
        
        cout << "On the " << i+1 << "th level of the search tree." << endl;
        int featureToAddThisLevel;
        //set<int> featureToAddThisLevel;
        double bestAccuracySoFar = 0;
        
        for(int j = 0; j < data[0].features.size(); ++j){//
            //if feature isn't already in set, consider adding
            bool found = false;
            for(int z = 0; z < currentSetOfFeatures.size(); ++z){
                if(currentSetOfFeatures[z] == j)
                    found = true;
            }
            if(!found){
                
                cout << "Considering adding the " << j << " feature..." << endl;
                double accuracy = distanceForward(data, currentSetOfFeatures, j);
                //cout << "ACCURACY: " << accuracy << endl;
                //cout << "After LeaveONEOUT: " << j << endl;
                if (accuracy > bestAccuracySoFar){
                    //cout << "New Best Accuracy: " << accuracy << endl;
                    bestAccuracySoFar = accuracy;
                    featureToAddThisLevel = j;//should this be an array???
                }
            }
            
        }
        
        cout << "ADDING FEATURE: " << featureToAddThisLevel+1 << " at level: " << i+1 << endl;
        cout << "Gives Accuracy: " << bestAccuracySoFar << endl;
        currentSetOfFeatures.push_back(featureToAddThisLevel);
        if(bestAccuracySoFar > bestAccuracyEVER){
            bestSetOfFeatures = currentSetOfFeatures;
            bestAccuracyEVER = bestAccuracySoFar;
            //cout << "BEST ACC: " << bestAccuracyEVER << endl;
            
        }
        
    }
    cout << "Best Set(" << bestAccuracyEVER << "): ";
    for(int i = 0; i < bestSetOfFeatures.size(); ++i){
        cout << bestSetOfFeatures[i]+1 << ", ";
    }
    cout << endl;

}

void featureSearchBetter(vector<dataEntry> data){
    
    //don't know if i need this...
    vector<int> currentSetOfFeatures;
    vector<int> bestSetOfFeatures;
    //set<int> currentSetOfFeatures;
    double bestAccuracyEVER = 0;
    
    for(int i = 0; i < data[0].features.size(); ++i){//tree level
        
        cout << "On the " << i+1 << "th level of the search tree." << endl;
        int featureToAddThisLevel;
        //set<int> featureToAddThisLevel;
        double bestAccuracySoFar = 0;
        
        for(int j = 0; j < data[0].features.size(); ++j){//
            //if feature isn't already in set, consider adding
            bool found = false;
            for(int z = 0; z < currentSetOfFeatures.size(); ++z){
                if(currentSetOfFeatures[z] == j)
                    found = true;
            }
            if(!found){
                
                cout << "Considering adding the " << j << " feature..." << endl;
                double accuracy = distanceForwardBetter(data, currentSetOfFeatures, j);
                //cout << "ACCURACY: " << accuracy << endl;
                //cout << "After LeaveONEOUT: " << j << endl;
                if (accuracy > bestAccuracySoFar){
                    //cout << "New Best Accuracy: " << accuracy << endl;
                    bestAccuracySoFar = accuracy;
                    featureToAddThisLevel = j;//should this be an array???
                }
            }
            
        }
        
        cout << "ADDING FEATURE: " << featureToAddThisLevel+1 << " at level: " << i+1 << endl;
        cout << "Gives Accuracy: " << bestAccuracySoFar << endl;
        currentSetOfFeatures.push_back(featureToAddThisLevel);
        if(bestAccuracySoFar > bestAccuracyEVER){
            bestSetOfFeatures = currentSetOfFeatures;
            bestAccuracyEVER = bestAccuracySoFar;
            //cout << "BEST ACC: " << bestAccuracyEVER << endl;
            
        }
        
    }
    cout << "Best Set(" << bestAccuracyEVER << "): ";
    for(int i = 0; i < bestSetOfFeatures.size(); ++i){
        cout << bestSetOfFeatures[i]+1 << ", ";
    }
    cout << endl;

}

int main(){
    //time
    clock_t start;
    start = clock();
    double duration;
    
    int thing;
    double s1, s2, s3;
    ifstream input;
    
    //input >> s1 >> s2 >> s3;
    //cout << s1 << endl;
    //cout << s3 << endl;
    //cout << s2 << endl;
    
    dataEntry emptyEntryLarge(40);
    dataEntry emptyEntrySmall(10);
    
    //get the data
    string sizeText;
    string searchType;
    
    cout <<"Large or small?(l or s): ";
    cin >> sizeText;
    cout << endl;
    if(sizeText == "l"){
        input.open("cs_170_large80.txt", std::ifstream::in);
        if(input.is_open())
            cout << "OPEN!" << endl;
        vector<dataEntry> myDataLarge;
        for(int i = 0; i < 1000; ++i){
            myDataLarge.push_back(emptyEntryLarge);
        }
        for(int i = 0; i < 1000; ++i){
            for(int j = 0; j < 40; ++j){
                if(j == 0){
                    
                    input >> myDataLarge[i].classNum;
                    //cout << "class:" << myData[i].classNum << endl;
                    //input >> junk;
                }
                
                input >> myDataLarge[i].features[j];
                //cout << myData[i].features[j] << endl;
            }
        }
        //begin search!
        cout << "Forward, ForwardEarlyExit, Backward?(f, e, b): ";
        cin >> searchType;
        cout << endl;
        if(searchType == "f")
            featureSearch(myDataLarge);
        else if(searchType == "b")
            featureSearchBackward(myDataLarge);
        else
            featureSearchBetter(myDataLarge);
    }
    else{
        input.open("cs_170_small11.txt", std::ifstream::in);
        if(input.is_open())
            cout << "OPEN!" << endl;
        vector<dataEntry> myDataSmall;
        for(int i = 0; i < 100; ++i){
            myDataSmall.push_back(emptyEntrySmall);
        }
        for(int i = 0; i < 100; ++i){
            for(int j = 0; j < 10; ++j){
                if(j == 0){
                    
                    input >> myDataSmall[i].classNum;
                    //cout << "class:" << myData[i].classNum << endl;
                    //input >> junk;
                }
                
                input >> myDataSmall[i].features[j];
                //cout << myData[i].features[j] << endl;
            }
        }
        //begin search!
        cout << "Forward, ForwardEarlyExit, Backward?(f, e, b): ";
        cin >> searchType;
        cout << endl;
        if(searchType == "f")
            featureSearch(myDataSmall);
        else if(searchType == "b")
            featureSearchBackward(myDataSmall);
        else
            featureSearchBetter(myDataSmall);
    }
    duration = (clock() - start)/ (double) CLOCKS_PER_SEC;
    cout << "Execution time: " << duration << endl;
    
    return 0;
}
