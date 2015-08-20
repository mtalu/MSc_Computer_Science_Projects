/**
 * The invalid command exception class is a checked exception thrown by the game when 
 * the user enters an invalid command
 */
public class InvalidCommandException extends Exception {
	private String message;
	
	/**
	 * Constructor to create new checked exception with provided message
	 * @param message
	 */
	public InvalidCommandException(String message) {
		super(message);
		this.message = message;
	}

	public String toString() {
		return message;
	}
}
