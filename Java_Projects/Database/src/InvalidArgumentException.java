/**
 * The InvalidArgumentException is a checked exception that can be thrown whenever
 * there is a problem with an argument passed by a user.
 * @author Michael
 *
 */
public class InvalidArgumentException extends Exception {
	String message;
	
	public InvalidArgumentException(String message) {
		super(message);
		this.message = message;
	}
	
	public String getMessage() {
		return message;
	}
}
