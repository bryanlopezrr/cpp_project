# Debt Console Application - C++

This is a basic console application that will allow to keep track of payments a group makes 
to an entity. 
<br>IE: There is an outstanding balance of $5000. A group of people decide to partner to pay this debt together by
    submitting $200 payments every month. The program will keep track of this group as a whole adding all the payments
    but also making sure each indivuals contributions are recorded</br>

Time spent: **?** hours spent in total

## User Stories

The following **required** functionality is completed:

- [ ] User can add a new member to the program. 
- [ ] Create a group - join a group - have a one man team 
- [x] The program will read in members from a database and their corresponding payments
- [ ] User can remove a member from the program
- [ ] The program will save members and their corresponding files into a database  
- [ ] The program calculates total amount paid from all members 
- [ ] User can validate all the money left to pay 

## Table Implementation - Draft 

### Members Table
 | ID~ | Name | DOB | GroupID | Payments | 
 | --- | ---- | --- | ------- | -------- | 

### Groups Table 
 | GroupID~ | GroupName | DebtID | 
 | ------- | --------- | ------ |

### Debt Table 
 | DebtID~| DebtAmount | 
 | ------ | ---------- |

### Users Table
 | username | passwd | userID | 
 | -------- | ------ | ------ |


The logic here ^ is that a Member belongs to a Group that has a certain Amount of Debt from the Debt table
When a Member joins a group, they can start making payments to that Groups debt, view their personal payments. 
Groups of one individual are allowed, however you do need to have a GroupName to distinguish 
the Group from a regular Member

~ Represents PK 

### Screenshots of DB
<img src = "./img/postgresLH.png" title="postgres" width="490" height="400"/>
<img src = "./img/debtImage.png" title="debtTable" width="200" height="130"/>
<img src = "./img/memberImage.png" title="memberTable" width="380" height="90"/>
<img src = "./img/usersImage.png" title="usersTable" width="365" height="130"/>

## Instructions to run the program

- Navigate to file location
- Compile the program using : $ g++ -std=c++17 'nameOfFile.cpp'  -lpqxx -lpq -o './nameOfEXE'
- Run the program by executing ./nameOfEXE 

The following **bonus** features are implemented:


## Video Walkthrough

Here's a walkthrough of implemented user stories:
<img src='http://g.recordit.co/sxhqXk1Ppr.gif' title='Video Walkthrough' width='' alt='Video Walkthrough' />

<img src='http://g.recordit.co/R0Uia1UCLl.gif' title='Video Walkthrough' width='' alt='Video Walkthrough' />
