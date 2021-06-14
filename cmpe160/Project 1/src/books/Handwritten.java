package books;

import interfaces.ReadInLibrary;
import librarymembers.LibraryMember;

/**
 * @author Muhammed
 *
 */
public class Handwritten extends Book implements ReadInLibrary {

	/**
	 * @param bookID
	 * It works when adding a new book
	 */
	public Handwritten(int bookID) {
		this.bookID = bookID;

	}

	/**
	 * This method works when readInLibrary method in Library class called.
	 * It changes the owner of book from null to a member and make that book taken
	 */
	@Override
	public void readBook(LibraryMember member) {
		this.isTaken = true;
		this.whoHas = member;
	}

	/**
	 * This method works when returnBook method in Library class called. 
	 * It changes the owner of book from a member to null and make that book untaken
	 */
	@Override
	public void returnBook(LibraryMember member) {
		this.isTaken = false;
		this.whoHas = null;
	}

}