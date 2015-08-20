import static org.junit.Assert.*;

import org.junit.Test;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.File;

public class TableTest {

	@Test
	public void testValidTable() {
		try {
			Table table = new Table("column1", "column2");
			assertNotNull(table);
		}
		catch(InvalidArgumentException e) {
			fail("Should not throw RepeatedColumnNameException");
		}
	}
	
	@Test
	public void testInvalidTable() {
		try {
			Table table = new Table("column 1", "column 1");
			fail("Should throw a RepeatedColumnNameException");
		}
		catch(Exception e) {
			assertTrue(e instanceof InvalidArgumentException);
		}
	}
	
	@Test
	public void testGetColumnName() {
		Table table = null;
		
		try {
			table = new Table("column1", "column2");
			
		}
		catch(Exception e) {
			fail("Should not throw an exception");
		}
		
		assertEquals(table.getColumnName(1), "column1");
	}
	
	@Test
	public void testEditColumnNameValidNewName() {
		Table table = null;
		
		try {
			table = new Table("column1", "column2");
			table.editColumnName(0, "a new name");
			assertEquals(table.getColumnName(0), "a new name");
		}
		catch(Exception e) {
			fail("Should not throw an exception");
		}
	}
	
	@Test
	public void testEditColumnNameInvalidNewName() {
		Table table = null;
		
		try {
			table = new Table("column1", "column2");
			table.editColumnName(0, "column2");
			fail("Should throw RepeatedColumnNameException");
		}
		catch(Exception e) {
			assertTrue(e instanceof InvalidArgumentException);
		}
	}
	
	@Test
	public void testAddValidRecord() {
		try {
			Table table = new Table("column1", "column2", "column3");
			Record record = new Record(new Field("dataField1"), new Field("dataField2"), new Field("dataField3"));
			table.addRecord(record);
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
		
	@Test
	public void testAddInvalidRecord() {
		try {
			Table table = new Table("column1", "column2", "column3");
			Record record = new Record(new Field("dataField1"), new Field("dataField2"));
			table.addRecord(record);
			fail("Should throw InvalidArgumentException");
		}
		catch(Exception e) {
			assertTrue(e instanceof InvalidArgumentException);
		}
		
		try {
			Table table = new Table("column1", "column2", "column3");
			Record record = new Record(new Field("dataField1", DataType.INTEGER), new Field("dataField2"), new Field("dataField2"));
			table.addRecord(record);
			fail("Should throw UnsupportedDataTypeException");
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
	}
	
	@Test
	public void testDeleteRecord() {
		Table table = null;
		
		try {
			table = new Table();
			Record record = new Record();
			table.addRecord(record);
			assertTrue(table.getNumberOfRecords() == 1);
			table.deleteRecord(record);
			assertTrue(table.getNumberOfRecords() == 0);
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	@Test
	public void testGetAllInColumn() {
		Table table = null;
		
		try {
			table = new Table("column1");
			Record record = new Record(new Field("row1Col1"));
			Record record2 = new Record(new Field("row2Col1"));
			Record record3 = new Record(new Field("row3Col1"));
			table.addRecord(record);
			table.addRecord(record2);
			table.addRecord(record3);
			
			ArrayList<Field> expectedList = new ArrayList<Field>(Arrays.asList(new Field("row1Col1"), new Field("row2Col1"), new Field("row3Col1")));
			ArrayList<Field> actualList = table.getAllInColumn(table.getColumn(1));
			
			for(int i = 1; i < expectedList.size(); i++) {
				Field expectedField = expectedList.get(i);
				Field actualField = actualList.get(i);
				assertEquals(expectedField.getValue(), actualField.getValue());
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	@Test
	public void testXML() {
		try {
			File tempFolder = new File("TempTableFolder");
			tempFolder.mkdir();
						
			Table table = new Table("Column1", "Column2", "Column 3");
			Record recordTable1 = new Record(new Field("FieldCol1"), new Field("FieldCol2"), new Field("FieldCol3"));
			table.addRecord(recordTable1);
			table.writeToFile("table", "TempTableFolder");
						
			Table table2 = new Table();
			table2.readFromFile("table.table", "TempTableFolder");
			Record recordTable2 = table2.getRecord(0);
					
			for(int i = 0; i < recordTable1.getNumberOfFields(); i++) {
				Field t1Field = recordTable1.getField(i);
				Field t2Field = recordTable2.getField(i);
				assertTrue(t1Field.getValue().equals(t2Field.getValue()) /*&& t1Field.getType().equals(t2Field.getType())*/);
			}
			
			MyUtilities.deleteDirectory(tempFolder);
		}
		catch(Exception e) {
			e.printStackTrace();
			fail("Should not throw exception");
		}
	}
}
