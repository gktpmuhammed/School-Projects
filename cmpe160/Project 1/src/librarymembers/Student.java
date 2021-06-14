package librarymembers;

import java.util.ArrayList;

import books.Book;

/**
 * @author Muhammed
 *
 */
public class Student extends LibraryMember {

	/**
	 * @param id
	 * It works when adding a new Student
	 */
	public Student(int id) {
		this.id = id;
		this.maxNumberOfBooks = 10;
		this.timeLimit = 20;
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