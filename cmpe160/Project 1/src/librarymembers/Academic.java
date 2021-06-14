package librarymembers;

import java.util.ArrayList;

import books.Book;

/**
 * @author Muhammed
 *
 */
public class Academic extends LibraryMember {

	/**
	 * @param id
	 * It works when adding a new Academic
	 */
	public Academic(int id) {
		this.id = id;
		this.maxNumberOfBooks = 20;
		this.timeLimit = 50;
	}

	/**
	 * 
	 */
	@Override
	public ArrayList<Book> getTheHistory() {

		return theHistory;
	}

	/**
	 * 
	 */
	@Override
	public void setTheHistory(ArrayList<Book> theHistory) {

		this.theHistory = theHistory;
	}

}