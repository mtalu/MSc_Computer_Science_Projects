/**
 * The ForeignKeyReferenceException should be thrown when attempting to add a table
 * containing a foreign key that does not reference a valid record. 
 * @author Michael
 *
 */
public class ForeignKeyReferenceException extends Exception {
	String message;
	
	public ForeignKeyReferenceException(String message) {
		super(message);
		this.message = message;
	}
	
	public String getMessage() {
		return message;
	}
}
