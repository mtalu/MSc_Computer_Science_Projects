import static org.junit.Assert.*;

import org.junit.Test;

import java.io.File;



public class DatabaseTest {

	@Test
	/**
	 * Tests adding a table to the database
	 */
	public void testAddTable() {
		try {
			Database db = new Database();
			db.addTable("Table1", new Table());
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	/**
	 * Checks that exception is thrown when adding two tables with the same name
	 */
	public void testAddTableException() {
		try {
			Database db = new Database();
			db.addTable("Table1", new Table());
			db.addTable("Table1", new Table());
			fail("Should throw exception");
		}
		catch(InvalidArgumentException e) {
			assertEquals(e.getMessage(), "A table with the same name already exists in the database");
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	/**
	 * Checks that exception is thrown when adding a table with null value
	 */
	public void testAddTableException2() {
		try {
			Database db = new Database();
			db.addTable("Table1", null);
			fail("Should throw exception");
		}
		catch(Exception e) {
			assertTrue(e instanceof InvalidArgumentException);
		}
	}
	
	@Test
	/**
	 * Tests deleting an existing table from database
	 */
	public void testDeleteValidTable() {
		try {
			Database db = new Database();
			db.addTable("Table1", new Table());
			db.deleteTable("Table1");
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	/**
	 * Checks that exception is thrown when attempting to delete invalid table
	 */
	public void testDeleteInvalidTable() {
		try {
			Database db = new Database();
			db.deleteTable("Table1");
			fail("Should throw exception");
		}
		catch(Exception e) {
			assertTrue(e instanceof InvalidArgumentException);
		}
	}
	
	@Test
	/**
	 * Saves a database with three tables to a temporary folder and then
	 * checks that the folder contains three files.
	 */
	public void testSaveDatabase() {
		try {
			Database db = new Database();
			db.addTable("Table1", new Table());
			db.addTable("Table2", new Table());
			db.addTable("Table3", new Table());
			db.saveDatabase("TempDatabaseTestFolder");
			
			File tempFolder = new File("TempDatabaseTestFolder.db");
			assertSame(tempFolder.listFiles().length, 3);
			MyUtilities.deleteDirectory(tempFolder);
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	/**
	 * Saves a database with three tables to a temporary folder and then
	 * checks that the folder contains three files.
	 */
	public void testLoadDatabase() {
		try {
			Database db = new Database();
			db.addTable("Table1", new Table());
			db.addTable("Table2", new Table());
			db.addTable("Table3", new Table());
			db.saveDatabase("TempDatabaseTestFolder");
			
			Database db2 = new Database();
			db2.loadDatabase("TempDatabaseTestFolder.db");
			assertTrue(db2.getNumberOfTables() == 3);
			
			MyUtilities.deleteDirectory(new File("TempDatabaseTestFolder.db"));
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testAddValidForeignKey() {
		try {
			Database db = new Database();
			
			Table table = new Table("Name", "Age", "City");
			table.setColumnType(2, DataType.INTEGER);
			Record record1 = new Record(new Field("John"), new Field("24"), new Field("Bristol"));
			table.addRecord(record1);
			db.addTable("Table1", table);
			
			Table table2 = new Table("Foreign Key");
			table2.setColumnType(1, DataType.FOREIGN_KEY);
			Record record2 = new Record(new Field("fk-" + record1.getKey()));
			table2.addRecord(record2);
			db.addTable("Table2", table2);
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testAddInvalidForeignKey() {
		try {
			Database db = new Database();
			
			Table table = new Table("Name", "Age", "City");
			table.setColumnType(2, DataType.INTEGER);
			Record record1 = new Record(new Field("John"), new Field("24"), new Field("Bristol"));
			table.addRecord(record1);
			db.addTable("Table1", table);
			
			Table table2 = new Table("Foreign Key");
			table2.setColumnType(1, DataType.FOREIGN_KEY);
			Record record2 = new Record(new Field("fk-t3s7"));
			table2.addRecord(record2);
			db.addTable("Table2", table2);
			
			fail("Should throw exception");
		}
		catch(Exception e) {
			assertTrue(e instanceof ForeignKeyReferenceException);
		}
	}

}
