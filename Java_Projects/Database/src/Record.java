import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

/**
 * The Record class can store any number of fields.
 * @author Michael
 *
 */
public class Record {
	private ArrayList<Field> fields;
	
	/*
	 * Create a new record with given fields. The first field is initialized to a random 4 character key made up of a 
	 * mixture of numbers and letters. Keys should be checked by a table object to ensure they are unique 
	 * within that table
	 */
	public Record(Field... fields) {
		this.fields = new ArrayList<Field>(Arrays.asList(fields));
		setKey();
	}
	
	/**
	 * Adds field to record
	 * @param field
	 */
	public void addField(Field field) {
		fields.add(field);
	}
	
	/**
	 * Get the field at specified index
	 * @param index the index of the field
	 * @return the field as a string
	 * @throws IllegalArgumentException if index not valid
	 */
	public Field getField(int index) {
		
		if(!(index >= 0 && index < fields.size())) {
			throw new IllegalArgumentException("Field at specified index does not exist");
		}
		
		Field field = fields.get(index);
		return field;
	}
	
	/**
	 * Get the number of fields currently in the record
	 * @return the number of fields in the record
	 */
	public int getNumberOfFields() {
		return fields.size();
	}
	
	/**
	 * Sets the record's key to a random four character code consisting of numbers and lower
	 * case letters. The key is stored as the record's first element. Table objects must 
	 * ensure that the key generated is unique in the table.
	 */
	public void setKey() {
		Random random = new Random();
		
		char a, c; 
		int b, d;
		String key;
		
		a = (char) (random.nextInt(26) + 'a');
		c = (char) (random.nextInt(26) + 'a');
		b = random.nextInt(10);
		d = random.nextInt(10);
		
		key = "" + a + b + c + d;
		
		try {
			Field keyField = new Field(key);
			fields.add(0, keyField);
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Set key field to a custom key. Useful when reading key in from xml
	 * @param customKey the custom key
	 */
	public void setKey(String customKey) {
		if(DataTypes.getDataType(customKey) != DataType.KEY) {
			throw new IllegalArgumentException("Key does not appear to be valid");
		}
		
		try {
			Field keyField = new Field(customKey, DataType.KEY);
			fields.remove(0);
			fields.add(0, keyField);
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Returns the record's key
	 * @return the 4 character key as a string 
	 */
	public String getKey() {
		Field keyField = fields.get(0);
		String key = keyField.getValue();
		
		return key;
	}
}
