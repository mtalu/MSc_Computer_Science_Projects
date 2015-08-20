/**
 * The field class represent a single field inside a record
 * @author Michael
 *
 */
public class Field {
	private String value;
	private DataType type;
	
	/**
	 * Constructs a new field with given value and automatically sets the type if supported
	 * @param value the value of the field
	 * @throws InvalidArgumentException if value more than 25 characters
	 * @throws UnsupportedDataTypeException if value does not match supported data type
	 */
	public Field(String value) throws InvalidArgumentException, UnsupportedDataTypeException {
		if(value.length() > 25) {
			throw new InvalidArgumentException("Field values must be less than or equal to twenty-five characters");
		}
		
		DataType valueType = DataTypes.getDataType(value);
		
		if(valueType == null) {
			throw new UnsupportedDataTypeException("No supported data type for provided value");
		}
		
		type = valueType;
		this.value = value;
	}
	
	/**
	 * Constructs a new field with given value and specified data type. Could be useful when value
	 * can be represented by several data types, i.e. integer values can fit in alphanumeric fields
	 * @param value the value of the field
	 * @param type the data type
	 * @throws InvalidArgumentException if value more than 25 characters
	 * @throws UnsupportedDataTypeException if data type mismatch
	 */
	public Field(String value, DataType fieldType) throws InvalidArgumentException, UnsupportedDataTypeException {
		if(value.length() > 25) {
			throw new InvalidArgumentException("Field values must be less than or equal to twenty-five characters");
		}
		
		DataType valueType = DataTypes.getDataType(value);
		if(!DataTypes.accepts(fieldType, valueType)) {
			throw new UnsupportedDataTypeException("Data type mismatch");
		}
		
		type = fieldType;
		this.value = value;
	}
	
	/**
	 * Return current value of the field
	 * @return the value
	 */
	public String getValue() {
		return value;
	}
	
	/**
	 * Sets the value of a field to the new value specified
	 * @param value the value to set the field to
	 * @throws InvalidArgumentException if value more than 25 characters
	 * @throws UnsupportedDataTypeException if attempting to add value with unsupported data type
	 */
	public void setValue(String value) throws InvalidArgumentException, UnsupportedDataTypeException {
		if(value.length() > 25) {
			throw new InvalidArgumentException("Field values must be less than or equal to twenty-five characters");
		}
		
		DataType valueType = DataTypes.getDataType(value);
		if(!DataTypes.accepts(type, valueType)) {
			throw new UnsupportedDataTypeException("Data type of value not supported by this field");
		}
		
		this.value = value;
	}
	
	/**
	 * Return current data type of field
	 * @return the data type
	 */
	public DataType getType() {
		return type;
	}
	
	/**
	 * Sets the data type of the field
	 * @param type the data type to set the field to
	 * @throws UnsupportedDataTypeException if new data type does not support current value type
	 */
	public void setType(DataType newType) throws UnsupportedDataTypeException {		
		DataType currentValueType = DataTypes.getDataType(value);
		
		if(!DataTypes.accepts(newType, currentValueType)) {
			throw new UnsupportedDataTypeException("New field type does not support current value type");
		}
		
		this.type = newType;
	}
}
