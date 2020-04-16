#include <fstream>
#include <iostream> 
#include <vector>
#include <string>  
#include <sstream>
#include <fstream>
#include <pqxx/pqxx>

using namespace std;

void instructions();

int main(){

    instructions();

    string connection_string = "host=localhost port=5432 dbname=postgres user=postgres password=password";
    pqxx::connection con(connection_string.c_str());
    pqxx::work wrk(con);

    // pqxx::result res = wrk.exec("SELECT * FROM debtDetails");

    if(res.size() < 1){
        cout << "An error has occurred\n";
        return 1;
    }

    // cout << "Debt ID: \t" << "Debt Amount: \n";
    // for (int i = 0; i < res.size(); i++)
    // {
    //     cout << res[i][0] << "\t\t" << res[i][1] << endl;
    // }


    //ADDING SWITCH STAMENT FOR THE OPTIONS: 

    return 0; 
}

void instructions(){
    cout << "\n=======================================\n";
    cout << "      DEBT: ACCOUNTABILITY PROGRAM\n";
    cout << "=======================================\n\n";
    cout << "Please select an option below:\n\n";
}

