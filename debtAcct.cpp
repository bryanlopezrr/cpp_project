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


    This program assumes proper input from the user - at least for the testing of it 
*/

#include <fstream>
#include <iostream> 
#include <vector>
#include <string>  
#include <sstream>
#include <iomanip>
#include <termios.h>
#include <unistd.h>
#include <pqxx/pqxx>
#include "termcolor.hpp"

using namespace std;

void welcomeView();
void optionsForUsers();
void managerOptions();
void clearScreen();
int getch(void);
string passwordFunction(char* password);

int main(){

    while(1){

            //Connection string to local database - Postgres
            string connection_string = "host=127.0.0.1 port=5432 dbname=postgres user=postgres password=password";
            pqxx::connection con(connection_string.c_str());
            pqxx::work wrk(con);  
  

            //--- PROOF OF CONCEPT ----
            // pqxx::result res = wrk.exec("SELECT * FROM debt_details");
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
        welcomeView();
        char response = ' ';
        string username = " ", password = " "; 
        cin >> response;

        char password_ch[32];
        if( toupper(response) == 'Q'){break;}
        else if( toupper(response) == 'Y'){
            while(1){

                bool isManager = false; 
                cout << "Login in with your credentials: \n";
                cout << "Username: ";
                cin >> username; 
                cin.ignore();

                cout << "Password: ";
                // cin >> password; 
                // cin.ignore();
                password = passwordFunction(password_ch);    

                pqxx::result res = wrk.exec("SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "'");
                if(res.size() == 1){
                    char selection = ' ';
                    clearScreen();
                    cout << "\n -- WELCOME BACK -- \t\t[ " + username +  " ]\n";
                    cout << "What would you like to do?:\n";            
                    
                    string t = res[0][5].c_str();
                    if(t == "t"){
                        managerOptions(); 
                        isManager = true; 
                    }else{
                        optionsForUsers();
                    }

                        int amountPaid = 0;
                        int debtTotal = 0; 
                        pqxx::result res = wrk.exec("SELECT debtamount FROM debt_details d WHERE d.username = '"+ username + "';");
                        debtTotal = stoi((res[0][0]).c_str());

                        res = wrk.exec("SELECT payment_amount, payment_date FROM payments WHERE username = '" + username + "';");
                        for(int i = 0; i < res.size(); i++){
                            amountPaid += stoi((res[i][0].c_str()));
                        }

                    while(1){
                        //Connection string to local database - Postgres ---- RECONNECTING HERE because of commit to DB
                        string connection_string = "host=127.0.0.1 port=5432 dbname=postgres user=postgres password=password";
                        pqxx::connection con(connection_string.c_str());
                        pqxx::work wrk(con);  

                        if(!isManager){

                            cin >> selection;
                            if(toupper(selection) == 'X'){ cout << "Goodbye" << termcolor::cyan << "!!!" << "\n" << termcolor::reset; break;}
                            switch (toupper(selection))
                            {
                                case 'V':{
                                            pqxx::result res = wrk.exec("SELECT debtamount FROM debt_details d WHERE d.username = '"+ username + "';");
                                            cout << "Balance: \n";
                                            cout << termcolor::green << "$ "<< termcolor::reset << res[0][0] << endl << endl;

                                            debtTotal = stoi((res[0][0]).c_str());
                                        }
                                    break;
                                case '$':{
                                            pqxx::result res = wrk.exec("SELECT payment_amount, payment_date, paymentID FROM payments WHERE username = '" + username + "';");
                                            cout << "Payment" << "\t\t" << "Date" <<"\t\t" << "PaymentID" << endl;
                                            for(int i = 0; i < res.size(); i++){
                                                cout << termcolor::green << "$ " << termcolor::reset << res[i][0] << "\t\t" << res[i][1] << "\t\t "<<  res[i][2]  <<endl;
                                            }

                                        }
                                    break;
                                case 'P':{
                                            cout << "Amount Paid\n";
                                            cout << termcolor::green << "$ " << termcolor::reset << amountPaid << endl;
                                        }   
                                    break;
                                case 'O':{
                                            optionsForUsers();  
                                        }   
                                    break;                                
                                case 'M':{
                                            int payment = 0; 
                                            cout << "Enter amount to pay: \n";
                                            cin >> payment;
                                            cin.ignore(1000, '\n');
                                            amountPaid += payment;

                                            pqxx::result res = wrk.exec("INSERT INTO payments(username, payment_amount) VALUES('" + username + "', " + to_string(payment) +");"); 
                                            res = wrk.exec("UPDATE debt_details SET debtamount = (" + to_string(debtTotal - payment) + ") WHERE username = '" + username + "';");
                                            wrk.commit();
                                        }   
                                    break;    
                                default:
                                    cout << "Incorrect input\n\n";
                                    break;
                            }

                        }
                        else{
                                //Manager menu: 
                            string userToDelete = "";    
                            cin >> selection;
                            cin.ignore(1000, '\n');
                            if(toupper(selection) == 'X'){ cout << "Goodbye" << termcolor::cyan << "!!!" << "\n" << termcolor::reset; break;}
                            switch (toupper(selection))
                            {
                                case 'V':{
                                         pqxx::result res = wrk.exec("SELECT fname, lname, debtamount FROM users u INNER JOIN debt_details d ON u.username = d.username WHERE ismanager IS NOT true;"); 
                                         cout  << "First Name \t" << setw(10) << "Last Name " << setw(25) << "Debt Amount\n";
                                         for(int i = 0; i < res.size(); i++){
                                             cout << setw(10) << res[i][0] << "\t" << setw(10) << res[i][1] << "\t" << setw(10) << termcolor::green << "$ " <<  termcolor::reset <<res[i][2] << endl;
                                         }
                                    }
                                    break;
                                case 'D':{
                                        cout << termcolor::red << "Which user do you want to delete? "<< termcolor::reset;
                                        cin >> userToDelete;
                                        cin.ignore(1000, '\n');
                                        
                                        pqxx::result res = wrk.exec("SELECT * FROM users WHERE username = '" + userToDelete + "';");
                                        if(res.size() == 1)
                                        {
                                            pqxx::result res = wrk.exec("DELETE FROM users WHERE username = '" + userToDelete + "';");
                                            res = wrk.exec("DELETE FROM payments WHERE username = '" + userToDelete + "';");
                                            res = wrk.exec("DELETE FROM debt_details WHERE username = '" + userToDelete + "';");

                                            cout << "Are you sure? (Y/N)\n";
                                            cin >> response; 
                                            cin.ignore(1000, '\n');
                                            if(toupper(response) == 'Y'){
                                                wrk.commit();
                                            }else{
                                                break;   
                                            }
                                            
                                        }
                                        else{
                                            cout << "User does not exist\n"; 
                                        }

                                    }
                                    break;
                                case 'R':{
                                        cout << "Top 10 Most Recent Payments:\n";
                                        pqxx::result res = wrk.exec("SELECT * FROM payments ORDER BY payment_date DESC LIMIT 10;"); 
                                        for(int i = 0; i < res.size(); i++){
                                            cout << setw(15) << res[i][0] << "\t" << setw(10) << termcolor::green <<  "$ " <<  termcolor::reset << res[i][1] << "\t"  << res[i][2] << endl;
                                        }
                                    }
                                    break;
                                case 'O':{
                                        managerOptions(); 
                                    }
                                    break;
                                case 'A':{
                                    //changes DB
                                        cout << termcolor::yellow<< "Account help -- UNDER CONSTRUCTION\n\n";                                        
                                        cout << "                              \\`.    T" << endl;
                                        cout << "    .--------------.___________) \\   |    T" << endl;
                                        cout << "    |//////////////|___________[ ]   !  T |" << endl;
                                        cout << "    `--------------'           ) (      | !" << endl;
                                        cout << "                               '-'      !" << endl <<termcolor::reset;
                                    }
                                    break;                                    
                                case 'M':{
                                        int modPayment =0, amountEntered = 0;
                                        cout << "What payment ID do you want to modify: ";
                                        cin >> modPayment; 
                                        cin.ignore(1000, '\n');
                                        cout << "What amount needs to be entered: ";
                                        cin >> amountEntered;
                                        cin.ignore(1000, '\n');

                                        pqxx::result res = wrk.exec("UPDATE payments SET payment_amount = (" + to_string(amountEntered) + ") WHERE paymentID = " + to_string(modPayment) + ";");
                                        wrk.commit();

                                    }
                                
                                    break; 
                                case 'U':{
                                        string userUpg ="";
                                        cout << "What user do you want to upgrade -> manager: ";
                                        cin >> userUpg; 
                                        cin.ignore(1000, '\n');

                                        pqxx::result res = wrk.exec("UPDATE users SET ismanager = (true) WHERE username = '" + userUpg + "';");
                                        wrk.commit();

                                    }
                                
                                    break;                                                                                                                               
                                default:
                                    cout << "Incorrect input\n\n";
                                    break;
                            }

                        }    
                    }

                }
                else{
                    clearScreen();
                    cout << "\n -- NOT FOUND | WRONG UNAME & PASS -- \n";
                    break;
                }
                cout << "Logging out [ " + username << " ]" << endl;
                sleep(2);
                clearScreen();
                break;
            }
        }  
        else{
            clearScreen();
            cin.ignore(1000, '\n');

            string fullName ="";
            /*
            cout << "Goodbye \n";
            break;*/
            cout <<termcolor::on_grey << "\n=======================================\n" 
                 << "          CREATE AN ACCOUNT:            \n"
                 <<"=======================================\n\n" << termcolor::reset; 
                                
            cout << "Enter first name, last name \n";
            getline(cin, fullName);

            vector<string> tokenizedName;
            string name;
            istringstream is(fullName);
                while (getline(is, name, ' ')){
                    tokenizedName.push_back(name);
                }
                
            // cin.ignore(1000, '\n');
            cout << "Choose a username: ";
            cin >> username; 
            cin.ignore(1000, '\n');
            //Logic to check if user name exists
            while(1){
                pqxx::result res = wrk.exec("SELECT * FROM users WHERE username = '" + username + "'");
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
            string debt ="";
            cout << "\nHow much is your debt: ";
            cin >> debt; 
            pqxx::result res = wrk.exec("INSERT INTO users(username, password, fname, lname) VALUES('" + username + "', '" + password + "', '" + tokenizedName[0] + "', '" + tokenizedName[1] + "')");
            res = wrk.exec("INSERT INTO debt_details(username, debtamount) VALUES('" + username + "', " + debt + ");");
            wrk.commit();

            if(res.size() == 0){
                cout << termcolor::green << "\n\n * * * ACCOUNT CREATED ^_^ * * *\n" << termcolor::reset;
            }
            else{
                cout << termcolor::red << "\nAn error occurred. Try again\n" << termcolor::reset;
            }
            sleep(2);
            clearScreen();
            tokenizedName.clear();
        // ............................................................
        } 
    }
    
    return 0; 
}

void welcomeView(){
    cout << termcolor::on_grey; 
    cout << "\n=======================================\n";
    cout << "      DEBT: ACCOUNTABILITY PROGRAM\n";
    cout << "=======================================\n\n";
    cout << "Do you have an account with us already? (Y/N): \t\t Type Q - to quit the program\n"<< termcolor::reset;
}


void optionsForUsers(){
    cout << termcolor::on_grey << "\n====================================================\n";
    cout << "| V - View balance | P - Paid amount | X - Exit    |\n";
    cout << "| ---------------- | --------------- | ----------- |\n";
    cout << "| $ - All payments | M - Make payment| O - Options |\n";
    cout << "====================================================\n" << termcolor::reset;
}

void managerOptions(){
    cout << termcolor::on_blue<<"* * * MANAGER * * *";
    cout << "\n==========================================================\n";
    cout << "| V - View members    | D - Delete members | X - Exit    |\n";
    cout << "| ------------------- | ------------------ | ----------- |\n";
    cout << "| R - Recent payments | A - Account help   | O - Options |\n";
    cout << "| ------------------- | ------------------ | ----------- |\n";
    cout << "| M - Modify payments | U - Upgrade acct   |             |\n";
    cout << "==========================================================\n"<< termcolor::reset;
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

