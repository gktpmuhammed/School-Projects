
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Random;
import java.util.Scanner;

public class MG2017400162 {
	public static int count = 0;// This variable stores the number of computer wins.
	public static int count1 = 0;// This variable stores the number of player wins.
	public static int markCount = 0;// This variable stores the number of 'X' when checking impossible board.
	public static int markCount1 = 0;// This variable stores the number of 'O' when checking impossible board.

	public static void main(String[] args) throws FileNotFoundException {
		Scanner console = new Scanner(System.in);
		System.out.println("Welcome to the XOX Game. ");
		System.out.print("Would you like to load the board from file or create a new one? (L or C) ");
		String choice = console.next();// This variable stores the player's choice whether loading from a file or not.
		choice = choice.toUpperCase();// To prevent errors this statement convert letter to upper case.
		while(!choice.equals("L") && !choice.equals("C")) {// If player enters anything except L or C this method ask again player's choice.
			System.out.print("Wrong input! Try again: ");
			choice = console.next();
			choice = choice.toUpperCase();
		}
		if (choice.equals("L")) {// If choice is L loadFile method start to execute its statements.
			loadFile(console);
		}
		else if (choice.equals("C")) {// If choice is C create method start to execute its statements.
			create(console);
		}	
		System.out.println("Thanks for playing.");
	}
	
	public static void loadFile(Scanner console) throws FileNotFoundException {//if player decides to load file this method will run
		Random rand = new Random();
		int r = 0;// It represents row
		int c = 0;// It represents column
		int k = 0;// Just in case of control for win conditions.
		String again= "Y";//this represent the choice of whether player wants to play again or not
		String newTable ="";//this holds the context of file
		String table ;//this holds the board
		int n = 0;//just for running while loop for once.
		while(n==0) {
			System.out.print("Please enter the file name: ");
			String file = console.next();//file name
			File fileBoard = new File(file);
			while(!fileBoard.exists()) {//checks whether this file exists or not
				System.out.println("Such a file does not exist! ");//ýf file does not exist this statement executes.
				System.out.print("Please enter the file name: ");
				file = console.next();//ýt will read file name again
				fileBoard = new File(file);
			}
			while(!fileBoard.isFile()) {//it checks whether this is a file or not
				System.out.println("Such a file does not exist! ");//ýf file does not exist this statement executes.
				System.out.print("Please enter the file name: ");
				file = console.next();//ýt will read file name again
				fileBoard = new File(file);
			}
			Scanner fileScan = new Scanner(fileBoard);//this scanner for reading file context
			newTable ="";//empty string for table
			while(fileScan.hasNextLine()) {//if file has next line this loop continues to return
				for(int m=0;m<3;m++) {//my file consist of 4 lines so this loop return 3 times and then read last line
					newTable+=fileScan.nextLine();//this statement reads each line and add to new table
					newTable+="\n";
				}
				newTable+=fileScan.nextLine();
			}
			table = newTable;
			if(!impossibleBoard(table)) {//it checks whether this board possible or not
				System.out.println("Impossible board configuration! ");
				continue;
			}
			else {
				if(!checkForWin(table)) {//it checks whether there is a win or not in loaded file
					System.out.println(table);
					System.out.println("Game has already ended!");
					create(console);//then it create an empty board to play again
					n=1;
				}
				else {//if  there is no problem such as impossible board or any win condition program start to continue properly
					System.out.println("Load successful. ");
					System.out.print("Enter your symbol: (X or O) ");//it asks player's mark
					String mark = console.next();
					mark = mark.toUpperCase();
					String mark1 = "";//computer's mark
					while(!mark.equals("X") && !mark.equals("O")) {//if player enters anything except X and O program give an error and asks again
						System.out.print("Wrong input! Try again: ");
						mark = console.next();
						mark = mark.toUpperCase();
					}
					if(mark.equals("X")) {//if mark equals X computer's mark become O
						mark1 = "O";
						System.out.print("You are player " + mark + " and the computer is player "+ mark1 + ". \n");
					}
					else if (mark.equals("O")){//if mark equals O computer's mark become X
						mark1 = "X";
						System.out.print("You are player " + mark + " and the computer is player " +mark1 + ". \n");
					}
					if(markCount==markCount1+1) {//if the number of X's one greater than O's and player's mark equals X computers starts the game
						if(mark.equals("X")) {
							System.out.println("Computer will start: ");
							table=computer(r,c,rand,table,mark1,mark);//this statement calls computer method and computer plays here
							System.out.println(table);
						}
						else {//if the number of X's one greater than O's and player's mark equals O player starts the game
							System.out.println("You will start: ");
							System.out.println(table);
						}
					}
					else if(markCount1==markCount+1) {//if the number of O's one greater than X's and player's mark equals O computers starts the game
						if(mark.equals("O")) {
							System.out.println("Computer will start: ");
							table=computer(r,c,rand,table,mark1,mark);//this statement calls computer method and computer plays here
							System.out.println(table);
						}
						else {//if the number of O's one greater than X's and player's mark equals X computers starts the game
							System.out.println("You will start: ");
							System.out.println(table);
						}
					}
					else if(markCount1==markCount){//if the number of O's equal to number of X's than the beginner who starts the game decided randomly
						int starter = rand.nextInt(2);//it gives randomly two number 0 and 1
						if (starter==1) {//if the number equals to 1 then computer starts
							System.out.println("Computer will start: ");
							table=computer(r,c,rand,table,mark1,mark);//this statement calls computer method and computer plays here
							System.out.println(table);//it prints table
						}
						else {//if number is zero than player starts
							System.out.println("You will start: ");
							System.out.println(table);
						}
					}
					if (checkForTie(table)) {//after the player move program check again whether there is a tie or not
						System.out.print("There has a tie!!! Do you want to play again? (Y or N) ");//this statement ask player wants to play again or not
						again = console.next();//it holds player choice about playing again Y represents yes N represents No
						again = again.toUpperCase();
						if(again.equals("N")) {//if choice equals N program stops
							System.out.println("You: " + count1 + " Computer: " + count);//then print scoreboard
						}
						else if(again.equals("Y")) {//if choice equals Y program creates an empty board and continues
							create(console);
						}
						break;
					}
					if(!checkForWin(table)) {//after the player move program check again whether there is a win or not
						k=1;//just handle if statements easily
						if (k==1) {
							System.out.print("Computer wins! Do you want to play again? (Y or N) ");//this statement ask player wants to play again or not
							count++;// this holds the number of computer wins
						}
						again = console.next();//it holds player choice about playing again Y represents yes N represents No
						again = again.toUpperCase();
						if(again.equals("N")) {//if choice equals N program stops
							System.out.println("You: " + count1 + " Computer: " + count);//then print scoreboard
						}
						else if(again.equals("Y")) {//if choice equals Y program creates an empty board and continues
							create(console);
						}
					}
					else {//if there is no tie or win than this statement start to execute its statements
						System.out.print("Enter coordinates: ");//asks player to enter coordinates for a new move
						while(true) {//if player enters a string or anything other than integer this loop prevent a runtime error and prints wrong input 
							try {
								r = console.nextInt();//takes input from player for row coordinate
								c = console.nextInt();//takes input from player for column coordinate
								break;
							}catch(Exception ex) {//if there is a exception these statements executes
								System.out.println("Wrong input! Try again: ");
								console.nextLine();
								continue;
							}
						}
						
						while(checkForWin(table)) {//while there is no win program continues to running
							k = 0;
							if(((r<1 || r>4) || (c<1 || c>4)) || table.charAt((18*(r-1)+(4*c-2)))!='E'){//if player enters invalid input such as if r and c 
								//coordinates range is out of 1-4 and if there is anything than E program give wrong input and asks again 
								System.out.print("Wrong input! Try again: ");
								r = console.nextInt();//takes input from player for row coordinate
								c = console.nextInt();//takes input from player for column coordinate
								continue;
							}
							table = table.substring(0,18*(r-1)+(4*c-2))+mark+ table.substring(18*(r-1)+(4*c-1));//player's move executes here and table configuration
							//changes here
							if (checkForTie(table)) {//after the player move program check again whether there is a tie or not
								System.out.print("There has a tie!!! Do you want to play again? (Y or N)");//this statement ask player wants to play again or not
								k=-1;//just handle if statements easily
								break;
							}
							if(checkForWin(table)) {//after the player move program check again whether there is a win or not
								table=computer(r,c,rand,table,mark1,mark);//this statement calls computer method and computer plays here
								k =1;//just handle if statements easily
							}
							if (checkForTie(table)) {//after the player move program check again whether there is a tie or not
								System.out.println(table);
								System.out.print("There has a tie!!! Do you want to play again? (Y or N)");//this statement ask player wants to play again or not
								k=-1;//just handle if statements easily
								break;
							}
							System.out.println(table);
							if (checkForWin(table)) {//after the computer move program check again whether there is a win or no
								System.out.print("Enter coordinates: ");
								while(true) {//if player enters a string or anything other than integer this loop prevent a runtime error and prints wrong input 
									try {
										r = console.nextInt();//takes input from player for row coordinate
										c = console.nextInt();//takes input from player for column coordinate
										break;
									}catch(Exception ex) {//if there is a exception these statements executes
										System.out.println("Wrong input! Try again: ");
										console.nextLine();
										continue;
									}
								}
							}
							else {
								break;//if there is no tie or winner program returns the beginning of while loop and executes its statement again
							}
						}
						if (k==1) {//just handle if statements easily
							System.out.print("Computer wins! Do you want to play again? (Y or N) ");//this statement ask player wants to play again or not
							count++;// this holds the number of computer wins
						}else if(k==-1) {//just handle if statements easily
							
						}
						else {
							System.out.print("You win! Do you want to play again? (Y or N) ");//this statement ask player wants to play again or not
							count1++;// this holds the number of player's wins
						}
						again = console.next();//it holds player choice about playing again Y represents yes N represents No
						again = again.toUpperCase();
						if(again.equals("N")) {//if choice equals N program stops
							System.out.println("You: " + count1 + " Computer: " + count);//then print scoreboard
						}
						else if(again.equals("Y")) {//if choice equals Y program creates an empty board and continues
							create(console);
						}
						n=1;//just handle while loop to run just once
					}
				}
			}
		}
	}
	
	public static void create(Scanner console) {//this method creates empty table
		String again= "Y";//just for running while loop in the beginning
		while(again.equals("Y")) {//if player's choice equals Y this loop starts execute its statements
			System.out.print("Enter your symbol: (X or O) ");//it asks player's mark
			String mark = console.next();
			mark = mark.toUpperCase();
			String mark1 = "";//computer's mark
			while(!mark.equals("X") && !mark.equals("O")) {//if player enters anything except X and O program give an error and asks again
				System.out.print("Wrong input! Try again: ");
				mark = console.next();
				mark = mark.toUpperCase();
			}
			if(mark.equals("X")) {//if mark equals X computer's mark become O
				mark1 = "O";
				System.out.print("You are player " + mark + " and the computer is player "+ mark1 + ". \n");
			}
			else if (mark.equals("O")){//if mark equals O computer's mark become X
				mark1 = "X";
				System.out.print("You are player " + mark + " and the computer is player " +mark1 + ". \n");
			}
			int r=0,c=0;
			String table = "| E | E | E | E |\n| E | E | E | E |\n| E | E | E | E |\n| E | E | E | E |";//empty table
			Random rand = new Random();
			int starter = rand.nextInt(2);//it gives randomly two number 0 and 1
			if (starter==1) {//if the number equals to 1 then computer starts
				System.out.println("Computer will start: ");
				table=computer(r,c,rand,table,mark1,mark);//this statement calls computer method and computer plays here
				System.out.println(table);//it prints table
			}
			else {//if number is zero than player starts
				System.out.println("You will start: ");
				System.out.println(table);
			}
			System.out.print("Enter coordinates: ");//asks player new coordinates 
			while(true) {//if player enters a string or anything other than integer this loop prevent a runtime error and prints wrong input 
				try {
					r = console.nextInt();//takes input from player for row coordinate
					c = console.nextInt();//takes input from player for column coordinate
					break;
				}catch(Exception ex) {//if there is a exception these statements executes
					System.out.println("Wrong input! Try again: ");
					console.nextLine();
					continue;
				}
			}
			int k = 0;
			while(checkForWin(table)) {//while there is no win program continues to running
				k = 0;
				if(((r<1 || r>4) || (c<1 || c>4)) || table.charAt((18*(r-1)+(4*c-2)))!='E'){//if player enters invalid input such as if r and c 
					//coordinates range is out of 1-4 and if there is anything than E program give wrong input and asks again 
					System.out.print("Wrong input! Try again: ");
					r = console.nextInt();//takes input from player for row coordinate
					c = console.nextInt();//takes input from player for column coordinate
					continue;
				}
				table = table.substring(0,18*(r-1)+(4*c-2))+mark+ table.substring(18*(r-1)+(4*c-1));//player's move executes here and table configuration changes here
				if (checkForTie(table)) {//after the player move program check again whether there is a tie or not
					System.out.print("There has a tie!!! Do you want to play again? (Y or N)");//this statement ask player wants to play again or not
					break;
				}
				if(checkForWin(table)) {//after the player move program check again whether there is a win or not
					table=computer(r,c,rand,table,mark1,mark);//this statement calls computer method and computer plays here
					if (checkForTie(table)) {//after the computer's move program check again whether there is a tie or not
						System.out.println("There has a tie!!!");
						break;
					}
					else {
						k =1;
					}
				}
				System.out.println(table);
				if (checkForWin(table)) {//after the computer move program check again whether there is a win or not
					System.out.print("Enter coordinates: ");//asks player new coordinates 
					while(true) {//if player enters a string or anything other than integer this loop prevent a runtime error and prints wrong input 
						try {
							r = console.nextInt();//takes input from player for row coordinate
							c = console.nextInt();//takes input from player for column coordinate
							break;
						}catch(Exception ex) {//if there is a exception these statements executes
							System.out.println("Wrong input! Try again: ");
							console.nextLine();
							continue;
						}
					}
				}
				else {
					break;//if there is no tie or winner program returns the beginning of while loop and executes its statement again
				}
			}
			if (k==1) {//just handle if statements easily
				System.out.print("Computer wins! Do you want to play again? (Y or N) ");//this statement ask player wants to play again or not
				count++;// this holds the number of computer wins
			}
			else {
				System.out.print("You win! Do you want to play again? (Y or N) ");//this statement ask player wants to play again or not
				count1++;// this holds the number of player's wins
			}
			again = console.next();//it holds player choice about playing again Y represents yes N represents No
			again = again.toUpperCase();
			if(again.equals("N")) {//if choice equals N program stops
				System.out.println("You: " + count1 + " Computer: " + count);//then print scoreboard
			}
		}
	}
	
	public static String computer(int r, int c, Random rand, String table, String mark1,String mark) {//computer makes its whole move and work here
		r = rand.nextInt(4)+1;//random row coordinate its range is 1-4
		c = rand.nextInt(4)+1;//random column coordinate its range is 1-4
		while(table.charAt(18*(r-1)+(4*c-2))!='E') {//if randomly chosen coordinate's place includes other than E this loop select new coordinate 
			r = rand.nextInt(4)+1;//random row coordinate its range is 1-4
			c = rand.nextInt(4)+1;//random column coordinate its range is 1-4
		}
		table = table.substring(0,18*(r-1)+(4*c-2))+mark1+ table.substring(18*(r-1)+(4*c-1));//table changes here computer makes its move here
		return table;//returns table
	}
	
	public static boolean checkForWin(String table) {//control function for win conditions
		int sum=0;//if there is any win situation this variable changes
		for(int i = 0; i<4;i++) {//this for loop returns 4 times (4 represents total row number)
			if((table.charAt(18*i+2)!='E' && table.charAt(18*i+2)==table.charAt(18*i+6) && table.charAt(18*i+6)==table.charAt(18*i+10)) || 
					(table.charAt(18*i+6)!='E' && table.charAt(18*i+6)==table.charAt(18*i+10) && table.charAt(18*i+10)==table.charAt(18*i+14))) {
				//this if statement checks just horizontal wins
				sum +=1;//if there is any win situation sum rises one
			}
		}
		for(int i=0; i<2;i++) {
			for(int k=2;k<15;k+=4) {
				if(table.charAt(18*i+k)!='E' && table.charAt(18*i+k)==table.charAt(18*(i+1)+k) && table.charAt(18*(i+1)+k)==table.charAt(18*(i+2)+k)){
					//this if statement checks just vertical wins
					sum +=1;//if there is any win situation sum rises one
				}
			}	
		}
		for(int i=0; i<2;i++) {
			if((table.charAt(18*i+2)!='E' && table.charAt(18*i+2)==table.charAt(18*(i+1)+6) && table.charAt(18*(i+1)+6)==table.charAt(18*(i+2)+10)) || 
					(table.charAt(18*i+6)!='E' && table.charAt(18*i+6)==table.charAt(18*(i+1)+10) && table.charAt(18*(i+1)+10)==table.charAt(18*(i+2)+14))) {
				//this if statement checks just diagonal wins
				sum +=1;//if there is any win situation sum rises one
			}
		}
		for(int i=0;i<2;i++) {
			if((table.charAt(18*i+14)!='E' && table.charAt(18*i+14)==table.charAt(18*(i+1)+10) && table.charAt(18*(i+1)+10)==table.charAt(18*(i+2)+6)) || 
					(table.charAt(18*i+10)!='E' && table.charAt(18*i+10)==table.charAt(18*(i+1)+6) && table.charAt(18*(i+1)+6)==table.charAt(18*(i+2)+2))) {
				//this if statement checks just reverse diagonal wins
				sum +=1;//if there is any win situation sum rises one
			}
		}
		if(sum==0) {//if there is no win method returns true
			return true;
		}
		else {//if there is a win method returns false
			return false;
		}
	}
	
	public static boolean impossibleBoard(String table) {//this method checks whether loaded file is possible valid board or not
		int sum=0;
		markCount = 0;// This variable stores the number of 'X' when checking impossible board.
		markCount1 = 0;// This variable stores the number of 'O' when checking impossible board.
		int diff = 0;//the difference between the number of X's and the number of O's
		for(int i=0;i<4;i++) {//this for loop returns 4 times (4 represents total row number)
			for(int k=2;k<=16;k+=4) {//for moving in rows
				if(table.charAt(18*i+k)!='X' && table.charAt(18*i+k)!='O' && table.charAt(18*i+k)!='E') {//if there are any other character except X O E sum rises once
					sum++;
				}
			}
			for(int l=0;l<=16;l+=4) {//for moving in rows
				if(table.charAt(18*i+l)!='|') {//if there are anything other than | sum rises once
					sum++;
				}
			}
			for(int n=1;n<=16;n+=2) {//for moving in rows
				if(table.charAt(18*i+n)!=' ') {//if there are anything other than ' '(space) sum rises once
					sum++;
				}
			}
			for(int k=2;k<=16;k+=4) {//for moving in rows
				if(table.charAt(18*i+k)=='X') {//this count number of X's
					markCount++;
				}
				if(table.charAt(18*i+k)=='O') {//this count number of O's
					markCount1++;
				}
			}
		}
		diff = markCount-markCount1;
		if(Math.abs(diff)>1) {//if difference greater than 1 this table is impossible and returns false
			return false;
		}
		else if(Math.abs(diff)<=1) {//if difference smaller than or equal to 1 this table is possible and returns true
			return true;
		}
		if(sum==0) {
			return true;//if sum equals to 0 this table is possible and returns true
		}
		else {
			return false;//if sum greater than 0 this table is impossible and returns false
		}
	}
	
	public static boolean checkForTie(String table) {//this method checks tie situation
		int sum = 0;
		for(int i=0;i<4;i++) {//this for loop returns 4 times (4 represents total row number)
			for(int k=2;k<16;k+=4) {//for moving in rows
				if(table.charAt(18*i+k)!='E') {//if empty cells are not E sum rises once
					sum++;
				}
			}
		}
		if(sum==16 && checkForWin(table)) {//if all cells not equal to E and there is no win situation there is tie and returns true
			return true;
		}
		else {
			return false;//returns false
		}
	}
}