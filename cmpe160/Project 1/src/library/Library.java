package library;

import java.util.Scanner;

import books.Book;
import books.Handwritten;
import books.Printed;
import librarymembers.Academic;
import librarymembers.LibraryMember;
import librarymembers.Student;

/**
 * @author Muhammed
 *
 */
public class Library {
	/**
	 * Scanner for reading input from file
	 */
	public Scanner scanner;
	/**
	 * Holds all books that library has
	 */
	private Book[] books;
	/**
	 * Holds all members (Students and Academics) that register to library
	 */
	private LibraryMember[] members;
	/**
	 * It refers the money that library gain from members that make late delivery
	 */
	protected int totalFee = 0;
	/**
	 * This is index of books array whenever a book registered it is incremented one
	 */
	private int place = 0;
	/**
	 * This is index of members array whenever a member registered it is incremented
	 * one
	 */
	private int order = 0;

	/**
	 * @return
	 */
	public Book[] getBooks() {
		return books;
	}

	/**
	 * @return
	 */
	public LibraryMember[] getMembers() {
		return members;
	}

	/**
	 * @param scanner It creates a Book array which has size of 1 million It creates
	 *                a LibraryMember array which has size of 1 million
	 */
	public Library(Scanner scanner) {
		books = new Book[1000000];
		members = new LibraryMember[1000000];
		this.scanner = scanner;
	}

	/**
	 * This method adds Books that are written in input file typ represents the type
	 * of book Printed(P) or Handwritten(H) First if works if the books' type is
	 * Printed Second if works if the books' type is Handwritten
	 */
	public void addBook() {
		String typ = scanner.next();
		place++;
		if (typ.equals("P")) {
			books[place] = new Printed(place);
			books[place].setBookType("P");
		}

		if (typ.equals("H")) {
			books[place] = new Handwritten(place);
			books[place].setBookType("H");
		}
	}

	/**
	 * This method adds Members that are written in input file person represents the
	 * type of Member Academic(A) or Student(S) First if works if the members' type
	 * is Academic Second if works if the members' type is Student
	 */
	public void addMember() {
		String person = scanner.next();
		order++;
		if (person.equals("A")) {
			members[order] = new Academic(order);
			members[order].setMemberType("A");
		}
		if (person.equals("S")) {
			members[order] = new Student(order);
			members[order].setMemberType("S");
		}
	}

	/**
	 * @param tick 
	 * id_of_book represents id of book 
	 * id_of_Library_Member represents id of member 
	 * First if checks whether given index of book array null or not 
	 * Second if checks whether given index of library member array null or not 
	 * Third if checks whether the given book already taken or not 
	 * Fourth if checks whether number of current books that a member has is smaller than the maximum number of books that a member has
	 * Fifth if checks whether given books' type is Printed or not
	 * Sixth if checks whether given member has a book that deadLine of book is already passed or not
	 * In the body of the last if statement first I attain the book to the member second I called borrowBook method from Printed class to change some variables
	 * In the third line I am adding the book to the members current books then I added to the History of the member
	 */
	public void borrowBook(int tick) {
		int id_of_book = scanner.nextInt();
		int id_of_Library_Member = scanner.nextInt();

		if (books[id_of_book] != null) {
			if (members[id_of_Library_Member] != null) {
				if (books[id_of_book].isTaken() != true) {
					if (members[id_of_Library_Member].currentBook.size() < members[id_of_Library_Member]
							.getMaxNumberOfBooks()) {
						if (books[id_of_book].getBookType().equals("P")) {
							if (members[id_of_Library_Member].isOverTime(tick)) {
								books[id_of_book].setWhoHas(members[id_of_Library_Member]);
								((Printed) books[id_of_book]).borrowBook(members[id_of_Library_Member], tick);
								members[id_of_Library_Member].currentBook.add(books[id_of_book]);
								members[id_of_Library_Member].getTheHistory().add(books[id_of_book]);
							}
						}
					}
				}
			}
		}
	}

	/**
	 * @param tick
	 * id_of_book represents id of book 
	 * id_of_Library_Member represents id of member 
	 * First if checks whether given index of book array null or not 
	 * Second if checks whether given index of library member array null or not 
	 * Third if checks whether the given book already taken or not 
	 * Fourth if checks whether given books' owner is given member or not
	 * Fifth if checks whether given books' type is Printed or not
	 * Sixth if checks whether the books deadLine is passed or not
	 * In the body of this if statement I calculate fee that member must pay
	 * Then I called returnBook method from Printed class to change some variables
	 * Then I remove the from members' current books
	 * And else if statement checks the books' type is Handwritten or not
	 * Then I called returnBook method from Printed class to change some variables
	 * Then I remove the from members' current books
	 */
	public void returnBook(int tick) {
		int id_of_book = scanner.nextInt();
		int id_of_Library_Member = scanner.nextInt();

		if (books[id_of_book] != null) {
			if (members[id_of_Library_Member] != null) {
				if (books[id_of_book].isTaken() != false) {
					if (books[id_of_book].getWhoHas() == members[id_of_Library_Member]) {
						if (books[id_of_book].getBookType().equals("P")) {
							if (tick > ((Printed) books[id_of_book]).getDeadLine()) {
								totalFee += tick - ((Printed) books[id_of_book]).getDeadLine();
							}
							((Printed) books[id_of_book]).returnBook(members[id_of_Library_Member]);
							members[id_of_Library_Member].currentBook.remove(books[id_of_book]);

						} else if (books[id_of_book].getBookType().equals("H")) {
							((Handwritten) books[id_of_book]).returnBook(members[id_of_Library_Member]);
							members[id_of_Library_Member].currentBook.remove(books[id_of_book]);
						}
					}
				}
			}
		}
	}

	/**
	 * @param tick
	 * id_of_book represents id of book 
	 * id_of_Library_Member represents id of member 
	 * First if checks whether given index of book array null or not 
	 * Second if checks whether given index of library member array null or not 
	 * Third if checks whether the given book already taken or not 
	 * Fourth if checks whether given books' owner is given member or not
	 * Fifth if checks whether given books' type is Printed or not
	 * Sixth if checks whether given books' deadline extended or not
	 * Seventh if checks whether given books deadline already passed or not
	 * In the body of last if statement I called extend method from Printed class to change some variables
	 */
	public void extendBook(int tick) {
		int id_of_book = scanner.nextInt();
		int id_of_Library_Member = scanner.nextInt();

		if (books[id_of_book] != null) {
			if (members[id_of_Library_Member] != null) {
				if (books[id_of_book].isTaken() != false) {
					if (books[id_of_book].getWhoHas().getId() == id_of_Library_Member) {
						if (books[id_of_book].getBookType().equals("P")) {
							if (((Printed) books[id_of_book]).isExtended()) {
								if (tick <= ((Printed) books[id_of_book]).getDeadLine()) {
									((Printed) books[id_of_book]).extend(members[id_of_Library_Member], tick);
								}
							}
						}
					}
				}
			}
		}
	}

	/**
	 * id_of_book represents id of book 
	 * id_of_Library_Member represents id of member 
	 * First if checks whether given index of book array null or not 
	 * Second if checks whether given index of library member array null or not 
	 * Third if checks whether the given book already taken or not 
	 * Fourth if checks whether the given books' type is Handwritten and at the same time given members type is Student or not
	 * In the body of this if statement there is no action
	 * And its else statement first I attain the book to the member
	 * Fifth if statement checks whether given books' type is Handwritten or not
	 * In the body of this if Statement first I called readBook method from Handwritten class to change some variables
	 * Second I add the book to the history of the member
	 * Lastly in the else part first I called readBook method from Handwritten class to change some variables
	 * Second I add the book to the history of the member
	 */
	public void readInLibrary() {
		int id_of_book = scanner.nextInt();
		int id_of_Library_Member = scanner.nextInt();

		if (books[id_of_book] != null) {
			if (members[id_of_Library_Member] != null) {
				if (books[id_of_book].isTaken() != true) {
					if (members[id_of_Library_Member].getMemberType().equals("S")
							&& books[id_of_book].getBookType().equals("H")) {
					} else {
						books[id_of_book].setWhoHas(members[id_of_Library_Member]);
						if (books[id_of_book].getBookType().equals("H")) {
							((Handwritten) books[id_of_book]).readBook(members[id_of_Library_Member]);
							members[id_of_Library_Member].getTheHistory().add(books[id_of_book]);

						} else {
							((Printed) books[id_of_book]).readBook(members[id_of_Library_Member]);
							members[id_of_Library_Member].getTheHistory().add(books[id_of_book]);
						}
					}
				}
			}
		}
	}

	/**
	 * @return the totalFee
	 * returns the total fee that library gains from members
	 */
	public int getTotalFee() {
		return totalFee;
	}

}
