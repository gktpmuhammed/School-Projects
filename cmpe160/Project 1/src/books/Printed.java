package books;

import interfaces.Borrow;
import interfaces.ReadInLibrary;
import librarymembers.LibraryMember;

/**
 * @author Muhammed
 *
 */
public class Printed extends Book implements ReadInLibrary, Borrow {

	/**
	 * It keeps the delivery date of the books
	 */
	protected int deadLine = 0;
	/**
	 * this variable holds whether there is an extension of the delivery date or not 
	 */
	protected boolean isExtended;

	/**
	 * @return the deadLine
	 */
	public int getDeadLine() {
		return deadLine;
	}

	/**
	 * @param deadLine the deadLine to set
	 */
	public void setDeadLine(int deadLine) {
		this.deadLine = deadLine;
	}

	/**
	 * @return the isExtended
	 */
	public boolean isExtended() {
		return isExtended;
	}

	/**
	 * @param bookID
	 * This works when adding a new printed book
	 */
	public Printed(int bookID) {
		this.bookID = bookID;
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

	/**
	 * This method works when borrowBook method in Library class called.
	 * It changes the owner of book from null to a member and make that book taken	 
	 * It updates the deadLine according to memberType Academic or Student
	 */
	@Override
	public void borrowBook(LibraryMember member, int tick) {
		this.isTaken = true;
		this.whoHas = member;
		deadLine = tick + member.getTimeLimit();

	}

	/**
	 * This method works when extendBook method in Library class called.
	 * It updates the deadLine according to memberType Academic or Student and make it longer
	 */
	@Override
	public void extend(LibraryMember member, int tick) {
		this.isExtended = true;
		this.whoHas = member;
		deadLine += member.getTimeLimit();
	}

	/**
	 *  This method works when readInLibrary method in Library class called.
	 * It changes the owner of book from null to a member and make that book taken	 
	 * It updates the deadLine very high to prevent penalty fee in readInLibrary method
	 */
	@Override
	public void readBook(LibraryMember member) {
		this.isTaken = true;
		this.whoHas = member;
		this.deadLine = (int) Math.pow(2, 32)-1;
	}

}