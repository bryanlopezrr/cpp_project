/*
                MILESTONE #2 DATABASE PROJECT

    This is a debt tracker for a group of people that can 
    add unlimited members per group to make payments to 
    a fixed amount of debt. 

    The program connects to postgres server db by using the pqxx library
    You can download it here :
        https://github.com/jtv/libpqxx
        or 
        http://pqxx.org -- official documentation

    GROUP MEMBERS: 
    Thinhn Phung - Bryan Lopez - Nam Pham

*/

#include <fstream>
#include <iostream> 
#include <vector>
#include <string>  
#include <sstream>
#include <fstream>
#include <pqxx/pqxx>

using namespace std;

void instructions();
void optionsForUsers();

int main(){

    string connection_string = "host=localhost port=5432 dbname=postgres user=postgres password=password";
    pqxx::connection con(connection_string.c_str());
    pqxx::work wrk(con);

            //--- PROOF OF CONCEPT ----
            // pqxx::result res = wrk.exec("SELECT * FROM debtDetails");
            // if(res.size() < 1){
            //     cout << "An error has occurred\n";
            //     return 1;
            // }

            // cout << "Debt ID: \t" << "Debt Amount: \n";
            // for (int i = 0; i < res.size(); i++)
            // {
            //     cout << res[i][0] << "\t\t" << res[i][1] << endl;
            // }
            
    while(1){
        //ADDING SWITCH STAMENT FOR THE OPTIONS: 
        instructions();
        char response = ' ';
        string username = " ", password = " "; 
        cin >> response;

        if( toupper(response) == 'Y'){
            while(1){
                cout << "Login in with your credentials: \n";
                cout << "Username: ";
                cin >> username; 
                cin.ignore();

                char x[18];
                cout << "Password: ";
                cin >> password; 
                cin.ignore();

                pqxx::result res = wrk.exec("SELECT * FROM userInfo WHERE username = '" + username + "' AND passwd = '" + password + "'");
                if(res.size() == 1){
                    char selection = ' ';
                    cout << "\n - WELCOME BACK - \n";
                    cout << "What would you like to do?:\n";            
                    optionsForUsers();

                        // This query returns the user that is logged in  
                        pqxx::result userLoggedIn = wrk.exec("SELECT * FROM userInfo WHERE username = '" + username + "';");
                        pqxx::row::reference uID = userLoggedIn[0][2];

                        string userID = uID.c_str();
                    while(1){
                        cin >> selection;
                        if(toupper(selection) == 'X'){ cout << "Goodbye\n"; break;}
                        switch (toupper(selection))
                        {
                            case 'V':{
                                        pqxx::result res = wrk.exec("SELECT debtamount, debtdetails.debtID FROM debtdetails INNER JOIN members ON debtdetails.debtID =" + userID + "LIMIT 1;");
                                        cout << "Balance: \n";
                                        for (int i = 0; i < res.size(); i++){
                                            cout << "$" << res[i][0] << endl << endl;
                                        }
                                     }
                                break;
                            case 'P':
                                cout << "PAID $--- HARD CODED FOR NOW \n\n";
                                break;
                            default:
                                cout << "Incorrect input\n\n";
                                break;
                        }
                        
                    }
                }
                else{
                    cout << "\n - NOT FOUND - \n";
                    break;
                }
                cout << "Logging out " + username << endl;
                break;
            }
        }  
        else{
            cout << "Goodbye \n";
            break;
        //------SECTION TO ADD A USER INTO THE DB --- IN PROGRESS ....
        //Only the group will need a login account but a the users can share this account ---->
            // cout << "Lets create you an account:\n";
            // cout << "Which group do you want to join?\n";

            //     while (true)
            //     {
            //         cout << "Press the name of the group\n";
            //         string groupName;
            //         getline(cin, groupName);

            //         if (groupName == "Win") // we will use database to get the Name of group in data by using hash table or other methods from SQL
            //         {
            //             cout << "You are added to a new group....\n";
            //             break;
            //         }
            //         else
            //         {
            //             cout << "The name of the group does not exist\n Please choose another name or you want to exit the program!\n";
            //             cout << "Press 'yes' to choose another name, press 'no' to exit the program\n";
            //             string check;
            //             getline(cin, check);
            //             cover(check);
            //             while (check != "yes" && check != "no")
            //             {
            //                 cout << "Please press 'yes' to agree, and 'no' to exit the program\n";
            //                 getline(cin, str);
            //                 cover(check);
            //             }
            //             if (check == "no")
            //             {
            //                 cout << "Exit the program\n";
            //                 return 0;
            //             }
            //             else
            //             {
            //                 continue;
            //             }
            //         }
            //     }
        // ............................................................
        } 
    }
    
    return 0; 
}

void instructions(){
    cout << "\n=======================================\n";
    cout << "      DEBT: ACCOUNTABILITY PROGRAM\n";
    cout << "=======================================\n\n";
    cout << "Do you have an account with us already? (Y/N):\n";
}


void optionsForUsers(){
    cout << "\n================================================\n";
    cout << "| V - View balance | P - Paid amount | X - Exit |\n";
    cout << "=================================================\n";
}
