package librarysimulator;

import java.util.Scanner;

import library.Library;
import librarysimulator.Action;

/**
 * 
 * Class that implements the logic for the simulator. In each timerTick call one
 * of the possible Actions happens.
 *
 */
public class LibrarySimulator {

	// The variable that represents the unit time
	private int tick = 0;

	Library library;
	Action action;

	public LibrarySimulator(Scanner scanner) {
		library = new Library(scanner);
	}

	public void timerTick(Action action) {

		tick++;

		this.action = action;

		if (action.getType() == Action.Type.addBook) {
			library.addBook();
		} else if (action.getType() == Action.Type.addMember) {
			library.addMember();
		} else if (action.getType() == Action.Type.borrowBook) {
			library.borrowBook(tick);
		} else if (action.getType() == Action.Type.returnBook) {
			library.returnBook(tick);
		} else if (action.getType() == Action.Type.extendBook) {
			library.extendBook(tick);
		} else if (action.getType() == Action.Type.readInLibrary) {
			library.readInLibrary();
		}

	}

	public int getTick() {
		return tick;
	}

}
