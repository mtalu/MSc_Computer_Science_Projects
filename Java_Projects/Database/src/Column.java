/**
 * The Column class represent a single column in a table
 * @author Michael
 *
 */
public class Column {
	private String name;
	private DataType type;
	
	/**
	 * Constructs a column object with a name and a type
	 * @param name the column name
	 * @param type the column type
	 */
	public Column(String name, DataType type) {
		this.name = name;
		this.type = type;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public DataType getType() {
		return type;
	}
	
	public void setType(DataType type) {
		this.type = type;
	}
}
