package main;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import librarysimulator.*;
import librarysimulator.Action.Type;

/*
 * 
 * INPUT FORMAT:
 * a number n represents the number of events
 * in the next n lines the events will be given in their own format
 * 
 * Shortcuts for Events
 * 1 => addBook
 * 2 => addMember
 * 3 => borrowBook
 * 4 => returnBook
 * 5 => extendBook
 * 6 => readInLibrary
 * 
 * Adding A new Book:
 * 1 P
 * 1 H
 * 
 * The id of a book is a six digit number, and the letters at the end stand for,
 * P => printed, H => handwritten
 *
 * Adding a new member:
 * 2 S
 * 2 A
 * 
 * the id of a library member is a six digit number,
 * S => Student, A => Academic
 * 
 * 
 * Borrowing A book:
 * 3 Id_Of_Book Id_Of_Library_Member
 * 
 * Returning A book:
 * 4 Id_Of_Book Id_Of_Library_Member
 * 
 * Extending A book:
 * 5 Id_Of_Book Id_Of_Library_Member
 * 
 * Read In Library:
 * 6 Id_Of_Book Id_Of_Library_Member
 *
 */

public class Main {

	public static void main(String[] args) throws FileNotFoundException {

		// The name of the input file is the only argument of the program
		File inputFile = new File(args[0]);

		Scanner scanner = new Scanner(inputFile);

		LibrarySimulator simulation = new LibrarySimulator(scanner);
		int numberOfEvents = scanner.nextInt();

		for (int i = 0; i < numberOfEvents; i++) {

			int type = scanner.nextInt();

			Action action;

			if (type == 1) {
				action = new Action(Type.addBook);
				simulation.timerTick(action);
			} else if (type == 2) {
				action = new Action(Type.addMember);
				simulation.timerTick(action);
			} else if (type == 3) {
				action = new Action(Type.borrowBook);
				simulation.timerTick(action);
			} else if (type == 4) {
				action = new Action(Type.returnBook);
				simulation.timerTick(action);
			} else if (type == 5) {
				action = new Action(Type.extendBook);
				simulation.timerTick(action);
			} else if (type == 6) {
				action = new Action(Type.readInLibrary);
				simulation.timerTick(action);
			}

		}
		scanner.close();
	}
}
