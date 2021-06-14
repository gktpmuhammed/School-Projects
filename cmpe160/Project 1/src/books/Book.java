package books;

import librarymembers.LibraryMember;

/**
 * @author Muhammed
 *
 */
public abstract class Book {

	/**
	 * It keeps bookID
	 */
	protected int bookID;
	/**
	 * It keeps whether it is handwritten or printed
	 */
	protected String bookType;
	/**
	 * It refers whether it is taken by someone or not
	 */
	protected boolean isTaken;
	/**
	 * ýt keeps the member that takes the book
	 */
	protected LibraryMember whoHas;

	/**
	 * @return the bookID
	 */
	public int getBookID() {
		return bookID;
	}

	/**
	 * @param bookID the bookID to set
	 */
	public void setBookID(int bookID) {
		this.bookID = bookID;
	}

	/**
	 * @return the bookType
	 */
	public String getBookType() {
		return bookType;
	}

	/**
	 * @param bookType the bookType to set
	 */
	
	public void setBookType(String bookType) {
		this.bookType = bookType;
	}

	/**
	 * @return the isTaken
	 */
	public boolean isTaken() {
		return isTaken;
	}

	/**
	 * @param isTaken the isTaken to set
	 */
	public void setTaken(boolean isTaken) {
		this.isTaken = isTaken;
	}

	/**
	 * @return the whoHas
	 */
	public LibraryMember getWhoHas() {
		return whoHas;
	}

	/**
	 * @param whoHas the whoHas to set
	 */
	public void setWhoHas(LibraryMember whoHas) {
		this.whoHas = whoHas;
	}

	/**
	 * Default constructor
	 */
	public Book() {

	}

	/**
	 * @param bookID
	 * @param bookType
	 */
	public Book(int bookID, String bookType) {
		this.bookID = bookID;
		this.bookType = bookType;
	}

	/**
	 * @param member
	 */
	public abstract void returnBook(LibraryMember member);

}