--- THIS FILE IS JUST A REPRESENTAITION OF SOME OF THE QUERIES I RAN TO TEST THE PROGRAM 


----- TO UPLOAD THE CSV INTO THE DB RUN THE FOLLOWING: --------------------------------------
\COPY users(username, password, fname, lname, ismanager) FROM '\The\path\to\the\CSV\file\in\your\computer.csv' WITH delimiter ',' CSV HEADER;
\COPY payments(username, payment_amount, payment_date) FROM '\The\path\to\the\CSV\file\in\your\computer.csv' WITH delimiter ',' CSV HEADER;
\COPY debt_details(username, debtamount) FROM '\The\path\to\the\CSV\file\in\your\computer.csv' WITH delimiter ',' CSV HEADER;

-----------#########################################################-----------------------------------------------


create table users(userID serial primary key, username varchar(18),
password varchar(18), fname varchar(18), lname varchar(18));
select * from users;
insert into users(username, password, fname, lname)
values('bobbyj', 'password', 'bob', 'jones' );
create table payments(username varchar(18), 
					  payment_amount integer, 
					  payment_date DATE NOT NULL DEFAULT CURRENT_DATE)
					 
select * from payments;

insert into payments(username, payment_amount)
values('bobbyj', 100);

---------- GET PAYMENTSSSS 
select payment_amount, payment_date 
from users u inner join payments p 
on p.username = u.username;

---------- debt table 
create table debt_details(username varchar(18), debtamount integer);

insert into debt_details(username, debtamount)
values('bobbyj', 2400);

select * from debt_details;
----GET DEBT
select debtamount from debt_details d where d.username = 'bobbyj';


---BEFORE CLOSING THE PROGRAM UPDATE AMOUNT OF DEBT

UPDATE debt_details
SET debtamount = (5000 - 700)
WHERE username = 'bobbyj';



----GET ALL THE MEMBERS

select fname, lname, debtamount from users u 
inner join debt_details d on u.username = d.username
where ismanager = false;


\copy payments(username, payment_amount, payment_date) from '/Users/bryanlopez/Desktop/debt_details.csv' with delimiter ',' csv header;