To run this web application without errors first you need to create database from scratch named dtBank.
You need to import sample data from csv files seperatly in datagrip. 
While importing these csv files you need to select separator as semicolon.
First row of the data represents field name so, you should right click on first row and select as header in datagrip.
After you import all csv files you run "all.sql" script to get our database design.
This script creates our table design for our database and makes some modifications on tables such as adding 
primary keys and foreign keys.

Now you are ready to run web application. you need to change your working directory to Cmpe321 folder.
If you are on Windows just run the command "python app.py" and it will run the application.
You can see the link for the application click it and browse it.
End point of application is localhost:5000
Now you will see two option for login. As user or as Database Manager. Choose one of them and continue. these links 
redirect you to login pages. After you enter your credentials you will see the functions that you can run.

Fill necessary fields of a function you select and press button. 
It will redirect you to a result page that print appropriate results.
