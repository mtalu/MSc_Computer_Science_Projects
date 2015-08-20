import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

/**
 * The DataTypes class contains a set of methods for dealing with supported database
 * data types
 * @author Michael
 *
 */
public class DataTypes {
	
	private HashMap<String, DataType> dataTypes;
	
	public DataTypes() {
		dataTypes = new HashMap<String, DataType>();
		List<DataType> dataTypes = Arrays.asList(DataType.values());
		
		for(DataType dataType : dataTypes) {
			this.dataTypes.put(dataType.toString(), dataType);
		}
	}
	
	/**
	 * Returns the data type for a particular string representation of a data type.
	 * Particularly useful when reading data types in from xml
	 * @param name the string representation of data type
	 * @return the data type
	 */
	public DataType getTypeForName(String name) {
		if(dataTypes.containsKey(name)) {
			return dataTypes.get(name);
		}
		
		return null;
	}
	
	/**
	 * Returns the data type of the value
	 * @param value the value in question
	 * @return the data type or null if unsupported
	 */
	public static DataType getDataType(String value) {
		if(value.matches("^[0-9]*$")) {
			return DataType.INTEGER;
		}
		else if(value.matches("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6}$")) {
			return DataType.EMAIL;
		}
		else if(value.matches("[a-z][0-9][a-z][0-9]")) {
			return DataType.KEY;
		}
		else if(value.matches("fk-[a-z][0-9][a-z][0-9]")) {
			return DataType.FOREIGN_KEY;
		}
		else if(value.matches("^[a-zA-Z0-9]*$")) {
			return DataType.ALPHANUMERIC;
		}
		else {
			return null;
		}
	}
	
	/**
	 * This method is needed since some data types like integer can be placed in an alphanumeric field
	 * @param accepting a data type
	 * @param accepted the other data type in question
	 * @return true if accepting data type accepts data type in questions, otherwise false
	 */
	public static boolean accepts(DataType accepting, DataType accepted) {
		switch(accepting) {
		case ALPHANUMERIC :
			if(accepted == DataType.ALPHANUMERIC || accepted == DataType.INTEGER 
				|| accepted == DataType.KEY || accepted == DataType.FOREIGN_KEY) {
				return true;
			}
			break;
		case INTEGER :
			if(accepted == DataType.INTEGER) {
				return true;
			}
			break;
		case EMAIL :
			if(accepted == DataType.EMAIL) {
				return true;
			}
			break;
		case KEY :
			if(accepted == DataType.KEY) {
				return true;
			}
			break;
		case FOREIGN_KEY :
			if(accepted == DataType.FOREIGN_KEY) {
				return true;
			}
			break;
		}
		
		return false;
	}
	
	/**
	 * A convenience method for checking if value is of specified data type
	 * (or is accepted by it).
	 * @param value the value in question
	 * @return true if value is specified data type, otherwise false
	 */
	public static boolean is(String value, DataType thisType) {
		DataType valueType = getDataType(value);
		if(DataTypes.accepts(thisType, valueType)) {
			return true;
		}
		
		return false;
	}
}
