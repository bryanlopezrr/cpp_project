#include <fstream>
#include <iostream> 
#include <vector>
#include <string>  
#include <sstream>

using namespace std;

struct familyMember {
    string name; 
    vector<int> contributions; 
};

void instructions();
void addMember();
vector<familyMember> members; 

int main(){

    instructions();
    
    bool keepGoing = true;
    char choice;

    ifstream fileWMembers; 

    //Reading from text file 
    fileWMembers.open("familyfile.txt");
    if(!fileWMembers){
        cout << "\nError opening the file\n";
        return 1; 
    }

    string name;
    string line; 
    int value; 
    // vector<int> money;

    while(getline(fileWMembers, line)){
        familyMember oneperson = {};        
        stringstream ss(line); 
        ss >> name; 
        oneperson.name = name; 

        while(ss >> value){
            // money.push_back(value);
            oneperson.contributions.push_back(value);            
        }
        members.push_back(oneperson);
    }

    // for(int i = 0; i < members.size(); i++){
    //     for(int j = 0; j < members[i].contributions.size(); j++){
    //         cout << members[i].contributions[j] << endl;
    //     }
    // }


    while(keepGoing){
        cin >> choice; 
        
            switch(toupper(choice)){
                case 'A':{
                    addMember();
                    break;
                }
                case 'C':{
                    cout << "C\n";
                    break;
                }
                case 'R':{
                    cout << "R\n";
                    break;
                }
                case 'X':{
                    keepGoing = false; 
                    break;
                }                
                default:{
                    cout << "Invalid input, try again\n";
                    break;
                }        
            }

            ofstream OUTfamilyfile;
            OUTfamilyfile.open("familyfile.txt");

            if(!OUTfamilyfile){
                cout << "Error writing to text file\n";
                return 1; 
            }

            for (int i = 0; i < members.size(); i++)
            {
                OUTfamilyfile << members[i].name << endl;
            }

    }


    return 0; 
}

void instructions(){
    cout << "\n=======================================\n";
    cout << "IRS TAX DEBT: ACCOUNTABILITY PROGRAM\n";
    cout << "=======================================\n\n";
    cout << "Please select an option from below:\n";
    cout << "| A - Add member| | C - check balance | | R - Remove member | | X - Exit program |\n";
}

void addMember(){
    familyMember newMember; 
    string name; 

    cout << "What is your name?\n";
    // getline(cin, name);
    cin >> name;
    newMember.name = name;
    cin.ignore(1000, '\n');

    members.push_back(newMember);
    
}
