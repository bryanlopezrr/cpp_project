create table users(userID serial primary key, username varchar(18),
password varchar(18), fname varchar(18), lname varchar(18));
select * from users;
insert into users(username, password, fname, lname)
values('bobbyj', 'password', 'bob', 'jones' );
create table payments(username varchar(18), 
					  payment_amount integer, 
					  payment_date DATE NOT NULL DEFAULT CURRENT_DATE)
					  

