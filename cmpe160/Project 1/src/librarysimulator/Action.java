package librarysimulator;
/**
 * class representing possible actions in LibrarySimulator.
 */
public class Action {
	/**
	 * enum representing possible action types. 
	 */
	public enum Type{
		addBook,
		addMember,
		borrowBook,
		returnBook,
		extendBook,
		readInLibrary
	}
	
	private final Type type;	
	
	public Action(Type type) {
		this.type = type;
	}
	
	public Type getType() {
		return type;
	}
	
	
	
	
	

}