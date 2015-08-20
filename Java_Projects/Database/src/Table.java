import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.io.File;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Element;

/**
 * The Table class represents a table that can store any number of records. It also
 * stores a list of columns, each of which contains the column name and supported data type
 * @author Michael
 *
 */
public class Table {
	private List<Column> columns;
	private List<Record> records;
	
	/**
	 * Create a new table with any number of columns. Columns have default alphanumeric data type.
	 * First column is always reserved to hold record key values.
	 * @param columns the names of the columns
	 */
	public Table(String... columns) throws InvalidArgumentException {
		if(!MyUtilities.elementsCorrectSize(columns)) {
			throw new InvalidArgumentException("Error: Column names must be less than or equal to 25 characters in length");
		}
		
		if(MyUtilities.containsRepeatedElement(Arrays.asList(columns))) {
			throw new InvalidArgumentException("Same column name used more than once");
		}

		
		this.columns = createColumns(columns);
		this.records = new ArrayList<Record>();
		this.columns.add(0, new Column("Key", DataType.KEY));
	}
	
	/**
	 * Get the name of a column at the specified index
	 * @param index the index of the column
	 * @return the name of the column or null if index out of bounds
	 */
	public String getColumnName(int index) {
		if(!(index >= 0 && index < columns.size())) {
			throw new IllegalArgumentException("Column at specified index does not exist");
		}
		
		String columnName = columns.get(index).getName();
		
		return columnName;
	}
	
	/**
	 * Change the name of a column
	 * @param index the index of the column
	 * @param newName the new name for the column
	 * @throws RepeatedColumnNameException
	 */
	public void editColumnName(int index, String newName) throws InvalidArgumentException {
		if(newName.length() > 25) {
			throw new InvalidArgumentException("Column names must be less than or equal to twenty-five characters in length");
		}
		
		if(duplicateColumnName(newName)) {
			throw new InvalidArgumentException("Duplicate column name");
		}
		
		if(!(index >= 0 && index < columns.size())) {
			throw new IllegalArgumentException("Column at specified index does not exist");
		}
		
		Column column = columns.get(index);
		column.setName(newName);
	}
	
	/**
	 * Gets current data type of column at specified index
	 * @param index the index of the column
	 * @return the data type of the column
	 */
	public DataType getColumnType(int index) {
		if(!(index >= 0 && index < columns.size())) {
			throw new IllegalArgumentException("Column at specified index does not exist");
		}
		
		Column column = columns.get(index);
		return column.getType();
	}
	
	/**
	 * Sets data type of a column to the specified type. Table cannot contain any
	 * records when using this function
	 * @param type a ColumnType variable
	 * @throws TableNotEmptyException if table contains data
	 */
	public void setColumnType(int index, DataType type) throws TableNotEmptyException {
		if(!records.isEmpty()) {
			throw new TableNotEmptyException("Table should not contain data when attempting to change a column type");
		}
		
		if(!(index >= 0 && index < columns.size())) {
			throw new IllegalArgumentException("Column at specified index does not exist");
		}
		
		if(index == 0) {
			throw new IllegalArgumentException("Data type for \"Keys\" column cannot be modified");
		}
		
		Column column = columns.get(index);
		column.setType(type);
	}
	
	/**
	 * Get the column at specified index
	 * @param index the index
	 * @return the column object
	 */
	public Column getColumn(int index) {
		if(!(index >= 0 && index < columns.size())) {
			throw new IllegalArgumentException("Column at specified index does not exist");
		}
		
		return columns.get(index);
	}
	
	/**
	 * Add a new record to a table. Checks to ensure that record key is unique.
	 * It must contain the same number of fields as there are columns in the table. 
	 * Fields must also be of the same type as corresponding column
	 * @param record the record to add
	 * @throws InvalidArgumentException if field/column mismatch
	 * @throws UnsupportedDataTypeException if field type/column type mismatch
	 */
	public void addRecord(Record record) throws InvalidArgumentException, UnsupportedDataTypeException {
		while(!isKeyUnique(record)) {
			record.setKey();
		}
		
		if(record.getNumberOfFields() != columns.size()) {
			throw new InvalidArgumentException("Field number mismatch");
		}
		
		if(dataTypeMismatch(record)) {
			throw new UnsupportedDataTypeException("Data type mismatch");
		}
			
		records.add(record);
	}
	
	/**
	 * Remove specified record from the table
	 * @param record the record to be removed
	 */
	public void deleteRecord(Record record) {
		if(!records.contains(record)) {
			throw new IllegalArgumentException("Record does not exist");
		}
			
		records.remove(record);
	}
	
	/**
	 * Get record at specified index
	 * @param index the index of the record
	 * @return the record or null if invalid
	 */
	public Record getRecord(int index) {
		if(!(index >= 0 && index < records.size())) {
			throw new IllegalArgumentException("Record at specified index does not exist");
		}
		
		Record record = records.get(index);
		
		return record;
	}
	
	/**
	 * Get a copy of all records in the table
	 * @return a copy of the records array list 
	 */
	public ArrayList<Record> getRecords() {
		ArrayList<Record> recordsCopy = new ArrayList<Record>(records);
		
		return recordsCopy;
	}

	
	/**
	 * Get the number of columns in the table
	 * @return number of columns in the table
	 */
	public int getNumberOfColumns() {
		return columns.size();
	}
	
	/**
	 * get the number of records in the table
	 * @return the number of rows in the table
	 */
	public int getNumberOfRecords() {
		return records.size();
	}
	
	/**
	 * Get all fields in a specified column
	 * @param column the column from which to fetch elements
	 * @return a list of the elements
	 */
	public ArrayList<Field> getAllInColumn(Column column) {
		int index = columns.indexOf(column);
		ArrayList<Field> columnData = new ArrayList<Field>();;
		
		for(Record record : records) {
			columnData.add(record.getField(index));
		}
		
		return columnData;
	}
	
	/**
	 * Write table contents to xml formatted file with .table extension
	 * @param filename the name of the file to be written to
	 */
	public void writeToFile(String fileName, String directory) {
		fileName += ".table";
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		DocumentBuilder db;
		
		try {
			db = dbf.newDocumentBuilder();
			Document doc = db.newDocument();
			Element rootElement = doc.createElement("Table");
			doc.appendChild(rootElement);
			
			headerToXML(doc, rootElement);
			bodyToXML(doc, rootElement);
			
			TransformerFactory tf = TransformerFactory.newInstance();
			Transformer transformer = tf.newTransformer();
			//transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			DOMSource source = new DOMSource(doc);
			StreamResult result;
			if(directory != null) {
				result = new StreamResult(new File(directory + "/" + fileName));
			}
			else {
				result = new StreamResult(new File(fileName));
			}
			transformer.transform(source, result);
			
		}
		catch(ParserConfigurationException e) {
			e.printStackTrace();
		}
		catch(TransformerException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Read in table contents from an xml formatted file. Note that any data
	 * that may already be stored in the table will be overwritten.
	 * @param fileName the name of file to read from including .table extension
	 */
	public void readFromFile(String fileName, String directory) {
		columns.clear();
		records.clear();
		
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder;
		
		try {
			builder = factory.newDocumentBuilder();
			Document doc;
			if(directory != null) {
				doc = builder.parse(directory + "/" + fileName);
			}
			else {
				doc = builder.parse(fileName);
			}
			
			XMLToHeader(doc);
			XMLToBody(doc);
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Print formatted table to the terminal, including the first column
	 * containing the unique keys of the record objects.
	 */
	public void printTable() {
		
		System.out.println();
		for(Column column : columns) {
			System.out.printf("%-30s", column.getName());
		}
		
		System.out.println();
		
		for(Record record : records) {
			System.out.println();
			for(int i = 0; i < record.getNumberOfFields(); i++) {
				System.out.printf("%-30s", record.getField(i).getValue());
			}
		}
		
		System.out.println("\n");
	}
		
	private void headerToXML(Document doc, Element rootElement) {
		Element header = doc.createElement("Header");
		rootElement.appendChild(header);
		
		for(Column column : columns) {
			Element colHeader = doc.createElement("Column");
			header.appendChild(colHeader);
			
			Element name = doc.createElement("Name");
			colHeader.appendChild(name);
			name.appendChild(doc.createTextNode(column.getName()));
			
			Element type = doc.createElement("Type");
			colHeader.appendChild(type);
			type.appendChild(doc.createTextNode(column.getType().toString()));
		}
	}
	
	private void bodyToXML(Document doc, Element rootElement) {
		Element body = doc.createElement("Body");
		rootElement.appendChild(body);
		
		for(Record record : records) {
			Element aRecord = doc.createElement("Record");
			body.appendChild(aRecord);
			for(int i = 0; i < columns.size(); i++) {
				Element field = doc.createElement("Field");
				aRecord.appendChild(field);
				
				Element value = doc.createElement("Value");
				field.appendChild(value);
				value.appendChild(doc.createTextNode(record.getField(i).getValue()));
				
				Element type = doc.createElement("Type");
				field.appendChild(type);
				type.appendChild(doc.createTextNode(record.getField(i).getType().toString()));
			}
		}
	}
	
	private void XMLToHeader(Document doc) {
		DataTypes types = new DataTypes();
		
		Node header = doc.getElementsByTagName("Header").item(0);
		NodeList children = header.getChildNodes();
		
		for(int i = 0; i < children.getLength(); i++) {
			Node column = children.item(i);
			String name = column.getFirstChild().getTextContent();
			String type = column.getFirstChild().getNextSibling().getTextContent();
			Column newColumn = new Column(name, types.getTypeForName(type));
			this.columns.add(newColumn);
		}
	}
	
	private void XMLToBody(Document doc) {
		DataTypes types = new DataTypes();
		
		Node body = doc.getElementsByTagName("Body").item(0);
		NodeList records = body.getChildNodes();
	
		try {
			for(int i = 0; i < records.getLength(); i++) {
				Node record = records.item(i);
				NodeList fields = record.getChildNodes();
				
				Record newRecord = new Record();
				String key = fields.item(0).getFirstChild().getTextContent();
				newRecord.setKey(key);
				
				for(int j = 1; j < fields.getLength(); j++) {
					Node field = fields.item(j);
					String value = field.getFirstChild().getTextContent();
					String type = field.getFirstChild().getNextSibling().getTextContent();
					Field newField = new Field(value, types.getTypeForName(type));
					newRecord.addField(newField);
				}
				
				this.records.add(newRecord);
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	private boolean isKeyUnique(Record record) {
		String key = record.getKey();
		
		for(Record currentRecord : records) {
			if(currentRecord.getKey().equals(key)) {
				return false;
			}
		}
		
		return true;
	}
	
	private ArrayList<Column> createColumns(String[] columnNames) {
		ArrayList<Column> columns = new ArrayList<Column>();
		
		for(int i = 0; i < columnNames.length; i++) {
			Column col = new Column(columnNames[i], DataType.ALPHANUMERIC);
			columns.add(col);
		}
		
		return columns;
	}
	
	private boolean dataTypeMismatch(Record record) {
		for(int i = 0; i < columns.size(); i++) {
			Column currentColumn = columns.get(i);
			Field currentField = record.getField(i);
			if(currentColumn.getType() != currentField.getType()) {
				return true;
			}
		}
		
		return false;
	}
	
	private boolean duplicateColumnName(String newName) {
		for(Column column : columns) {
			String name = column.getName();
			if(name.equals(newName)) {
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * Check if there is a record with specified key in the table
	 * @returns true if there is, otherwise false
	 */
	public boolean containsKey(String key) {
		for(Record record : records) {
			String currentKey = record.getKey();
			if(currentKey.equals(key)) {
				return true;
			}
		}
		
		return false;
	}
	
	public ArrayList<String> getForeignKeys() {
		ArrayList<String> foreignKeys = new ArrayList<String>();
		
		for(Column column : columns) {
			if(column.getType() == DataType.FOREIGN_KEY) {
				ArrayList<Field> fields = getAllInColumn(column);
				for(Field field : fields) {
					foreignKeys.add(field.getValue());
				}
			}
		}
		
		return foreignKeys;
	}
}
