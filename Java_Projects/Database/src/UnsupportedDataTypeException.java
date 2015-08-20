/**
 * The UnsupportedDataTypeException should be thrown when an attempt is made
 * to set a field value with a data type not supported by the field
 * @author Michael
 *
 */
public class UnsupportedDataTypeException extends Exception {
	String message;
	
	public UnsupportedDataTypeException(String message) {
		super(message);
		this.message = message;
	}
	
	public String getMessage() {
		return message;
	}
}
