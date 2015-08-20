import java.util.HashMap;
import java.util.Set;
import java.io.File;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;

/**
 * The Database class represents a collection of related tables.
 * @author Michael
 *
 */
public class Database {
	private HashMap<String, Table> tables;
	
	public Database() {
		tables = new HashMap<String, Table>();
	}
	
	/**
	 * Adds a table to the database, ensuring that each record is unique and has a unique key in the database
	 * @param name the name of the table
	 * @param table the table object to be added
	 * @throws InvalidArgumentException if table does not have a unique name
	 * @throws IllegalArgumentException if table is null or database already contains a record in the table
	 * @throws ForeignKeyReferenceException if foreign key does not reference valid record in database
	 */
	public void addTable(String name, Table table) throws InvalidArgumentException, ForeignKeyReferenceException {
		if(containsDuplicate(name)) {
			throw new InvalidArgumentException("A table with the same name already exists in the database");
		}
		
		if(table == null) {
			throw new InvalidArgumentException("Table object has null value");
		}
		
		if(!allRecordsUnique(table)) {
			throw new InvalidArgumentException("Same record in more than one table");
		}
		
		if(foreignKeyReferenceError(table)) {
			throw new ForeignKeyReferenceException("One or several foreign keys in table do not reference valid records");
		}
		
		Record duplicateKey = duplicateKey(table);
		
		while(duplicateKey != null) {
			duplicateKey.setKey();
			duplicateKey = duplicateKey(table);
		}
		
		tables.put(name, table);
	}
	
	/**
	 * Deletes tables with the specified name from the database if it exists
	 * @param name the name of the table
	 * @throws InvalidArgumentException
	 */
	public void deleteTable(String name) throws InvalidArgumentException {
		if(!tables.containsKey(name)) {
			throw new InvalidArgumentException("No table with such a name in the database");
		}
		
		tables.remove(name);
	}
	
	/**
	 * Gets table with the specified name
	 * @param name the name of the table
	 * @return the table object
	 * @throws InvalidArgumentException if no such table in database
	 */
	public Table getTable(String name) throws InvalidArgumentException {
		if(!tables.containsKey(name)) {
			throw new InvalidArgumentException("No table with such a name in the database");
		}
		
		Table table = tables.get(name);
		
		return table; 
	}
	
	/**
	 * Returns a list of tables currently in the database
	 * @return list of tables in the database
	 */
	public List<Table> getTables() {
		List<Table> tables = new ArrayList<Table>(this.tables.values());
		
		return tables;
	}
	
	/**
	 * Returns the number of tables in the database
	 * @return number of tables in the database
	 */
	public int getNumberOfTables() {
		return tables.size();
	}
	
	/**
	 * Saves all tables in the database to a .db folder with the specified name in the working directory
	 * @param folderName the name of the .db folder
	 * @throws IOException if fails to create .db folder
	 */
	public void saveDatabase(String folderName) throws IOException {
		folderName += ".db";
		File dir = new File(folderName);
		boolean success = dir.mkdir();
		
		if(!success) {
			throw new IOException("Error: Failed to create directory");
		}
		
		for(String key : tables.keySet()) {
			Table table = tables.get(key);
			table.writeToFile(key, folderName);
		}
	}
	
	/**
	 * Loads all tables from a .db folder with the specified name into the database.
	 * Clears all data that may already be present in the database.
	 * @param folderName the name of the folder including the .db extension
	 */
	public void loadDatabase(String folderName) throws IOException {
		tables.clear();
		
		File folder = new File(folderName);
		File[] files = folder.listFiles(new FilenameFilter() {
			public boolean accept(File folder, String name) {
				return name.toLowerCase().endsWith(".table");
			}
		});
		
		if(files != null) {
			for(int i = 0; i < files.length; i++) {
				try{
					String fileName = files[i].getName();
					Table table = new Table();
					table.readFromFile(fileName, folderName);
					addTable(fileName, table);
				}
				catch(InvalidArgumentException e) {
					e.printStackTrace();
				}
				catch(ForeignKeyReferenceException e) {
					e.printStackTrace();
				}
			}
		}
		else{
			throw new IOException("Error: Specified directory does not exist");
		}
	}
	
	/*
	 * Checks database for possible duplicate table name
	 * @param name the table name
	 * @return true if it does, otherwise false
	 */
	private boolean containsDuplicate(String name) {
		Set<String> keys = tables.keySet();
		
		if(keys.contains(name)) {
			return true;
		}
		
		return false;
	}
	
	/**
	 * Checks all records in new table to ensure they have a unique key.
	 * @param table the table to be added
	 * @return the record with a duplicate key, or null if none
	 */
	private Record duplicateKey(Table table) {
		
		for(Record record : table.getRecords()) {
			String newKey = record.getKey();
			if(containsKey(newKey)) {
				return record;
			}
		}
		
		return null;
	}
	
	private boolean allRecordsUnique(Table table) {
		for(Record record : table.getRecords()) {
			if(containsRecord(record)) {
				return false;
			}
		}
		
		return true;
	}
	
	/**
	 * Checks if database contains specified record
	 * @param record the record
	 * @return true if it does, false otherwise
	 */
	private boolean containsRecord(Record record) {
		for(Table table : tables.values()) {
			for(Record currentRecord : table.getRecords()) {
				if(record == currentRecord) {
					return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * Check database for specified key
	 * @param key the key
	 * @return true if database contains key, false otherwise
	 */
	private boolean containsKey(String key) {
		for(Table table : tables.values()) {
			for(Record record : table.getRecords()) {
				String databaseKey = record.getKey();
				if(key.equals(databaseKey)) {
					return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * Check all foreign keys in list for possible reference error, i.e. the record referenced does not exist
	 * @param table the table being added to database
	 * @return true if reference error, otherwise false
	 */
	private boolean foreignKeyReferenceError(Table table) {
		ArrayList<String> foreignKeys = table.getForeignKeys();
		
		for(String foreignKey : foreignKeys) {
			if(!validForeignKey(foreignKey)) {
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * Checks if database contains record with corresponding key for given foreign key
	 * @param foreignKey the foreign key
	 * @return true if it references valid record, otherwise false
	 */
	private boolean validForeignKey(String foreignKey) {
		if(!DataTypes.is(foreignKey, DataType.FOREIGN_KEY)) {
			throw new IllegalArgumentException("False foreign key passed to function");
		}
		
		String key = foreignKey.substring(3, foreignKey.length());
		
		if(containsKey(key)) {
			return true; 
		}
		
		return false;
	}
}
