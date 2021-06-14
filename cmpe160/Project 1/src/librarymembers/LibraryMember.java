package librarymembers;

import java.util.ArrayList;

import books.Book;
import books.Printed;

/**
 * @author Muhammed
 *
 */
public abstract class LibraryMember {

	/**
	 * It keeps id
	 */
	protected int id;

	/**
	 * It keeps the number of books that a member can have at the same time
	 */
	protected int maxNumberOfBooks;

	/**
	 * Ýt determines the days that one member can hold a book 
	 */
	protected int timeLimit;

	/**
	 * It keeps the memberType 
	 * for Academics it is A
	 * for Students it is S
	 */
	private String memberType;

	/**
	 * It keeps the history of a member 
	 * Every book that a member borrow or read in library
	 */
	protected ArrayList<Book> theHistory = new ArrayList<Book>();

	/**
	 * It keeps the books that a member has at the same time
	 */
	public ArrayList<Book> currentBook = new ArrayList<Book>();

	/**
	 * @return
	 */
	public int getId() {
		return id;
	}

	/**
	 * @return
	 */
	public int getMaxNumberOfBooks() {
		return maxNumberOfBooks;
	}

	/**
	 * @return
	 */
	public int getTimeLimit() {
		return timeLimit;
	}

	/**
	 * @return
	 */
	public String getMemberType() {
		return memberType;
	}

	/**
	 * @param memberType
	 */
	public void setMemberType(String memberType) {
		this.memberType = memberType;
	}

	/**
	 * @return
	 */
	public abstract ArrayList<Book> getTheHistory();

	/**
	 * @param theHistory
	 */
	public abstract void setTheHistory(ArrayList<Book> theHistory);

	/**
	 * @param tick
	 * @return
	 * 
	 * It is a method for determining if there is any book that should already returned to the library
	 */
	public boolean isOverTime(int tick) {
		for (Book i : currentBook) {
			if (((Printed) i).getDeadLine() < tick) {
				return false;
			}
		}
		return true;
	}
}