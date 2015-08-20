/**
 * The TableNotEmptyException should be thrown when attempting to manipulate
 * a table that should not contain any data, but does.
 * @author Michael
 *
 */
public class TableNotEmptyException extends Exception {
	String message;
	
	public TableNotEmptyException(String message) {
		super(message);
		this.message = message;
	}
	
	public String getMessage() {
		return message;
	}
}
