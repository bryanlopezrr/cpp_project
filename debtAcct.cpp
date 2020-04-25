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
#include <termios.h>
#include <unistd.h>
#include <pqxx/pqxx>

using namespace std;

void instructions();
void optionsForUsers();
void clearScreen();
int getch(void);
string passwordFunction(char* password);

int main(){


    while(1){
            //Connection string to local database - Postgres
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
            


        //ADDING SWITCH STAMENT FOR THE OPTIONS: 
        // clearScreen();
        instructions();
        char response = ' ';
        string username = " ", password = " "; 
        cin >> response;

        char password_ch[32];
        if( toupper(response) == 'Y'){
            while(1){
                cout << "Login in with your credentials: \n";
                cout << "Username: ";
                cin >> username; 
                cin.ignore();

                cout << "Password: ";
                // cin >> password; 
                // cin.ignore();
                password = passwordFunction(password_ch);    

                pqxx::result res = wrk.exec("SELECT * FROM userInfo WHERE username = '" + username + "' AND passwd = '" + password + "'");
                if(res.size() == 1){
                    char selection = ' ';
                    clearScreen();
                    cout << "\n -- WELCOME BACK -- \n";
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
                            case '$':
                                cout << "MONEYYYYYY\n";
                                break;
                            default:
                                cout << "Incorrect input\n\n";
                                break;
                        }
                        
                    }
                }
                else{
                    clearScreen();
                    cout << "\n -- NOT FOUND -- \n";
                    break;
                }
                cout << "Logging out " + username << endl;
                sleep(3);
                clearScreen();
                break;
            }
        }  
        else{
            clearScreen();
            string fullName =" ";
            /*
            cout << "Goodbye \n";
            break;*/
            cout << "\n=======================================\n" 
                 << "          CREATE AN ACCOUNT:            \n"
                 <<"=======================================\n"; 
                 
            cout << "Enter first name, last name \n";
            getline(cin, fullName);
            cin.ignore(1000, '\n');
    
            cout << "Choose a username: ";
            cin >> username; 
            cin.ignore(1000, '\n');
            //Logic to check if user name exists
            while(1){
                pqxx::result res = wrk.exec("SELECT * FROM userInfo WHERE username = '" + username + "'");
                if(res.size() == 1)
                {
                    cout << "User already exists. Try a different one\n";
                    cin >> username;
                    cin.ignore(1000, '\n');
                }
                else{
                    break;
                }
            }
            cout << "Type password: ";
            password = passwordFunction(password_ch); 
            // cin >> password; 
            // cin.ignore(1000, '\n');

            //if query is succesful == return Account succesful and go back to the top 
            //else some error occurred exit program 
            
            pqxx::result res = wrk.exec("INSERT INTO userinfo(username, passwd, \"userID\") VALUES('" + username + "', '" + password + "', 7372)");
            wrk.commit();

            if(res.size() == 0){
                cout << "\n *** ACCOUNT CREATED ^_^ ***\n";
            }
            else{
                cout << "\nAn error occurred. Try again\n";
            }
            sleep(3);
            clearScreen();
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
    cout << "| ---------------- | --------------- | -------- |\n";
    cout << "| $ - All payments |                 |          |\n";
    cout << "=================================================\n";
}

void clearScreen(){
    //not secure to use system() but for demo purposes, its cleaner
    system("clear");
}

string passwordFunction(char* password){
        char temp =' '; 
        string strPassword = "";

        for(int i=0;;){
            temp = getch();
            strPassword.push_back(temp);
            if((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9')){
                password[i] = temp;
                        ++i; 
                        cout << "*";
            }
            else if (temp == '\n'){
                break;
            }
        }
        strPassword.pop_back();
        return strPassword;
}

int getch(void){
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

