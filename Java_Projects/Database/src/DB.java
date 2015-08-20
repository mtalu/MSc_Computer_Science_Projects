import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

import java.util.ArrayList;
import java.util.Arrays;
import java.io.File;

public class DB {

	public static void main(String[] args) {
		DB.runAllTests();
		//DB.runDemo();
		
		
	}
	
	public static void runAllTests() {
		Result result = JUnitCore.runClasses(TestSuite.class);
		
		for (Failure fail : result.getFailures()) {
				System.out.println(fail.toString());
		}
		
		if (result.wasSuccessful()) {
			System.out.println(result.getRunCount() + " tests finished successfully");
		}
	}
	
	public static void runDemo() {
		//Create a new database object
		Database db = new Database();
		
		try {
			//Create a new table object with the specified columns. Columns will be set to "alphanumeric" by default
			Table table1 = new Table("Name", "Age", "Email");
			//Set table columns to appropriate types. Indeces start at 1, since the column at index 0 is reserved for
			//record keys and cannot be modified.
			table1.setColumnType(2, DataType.INTEGER);
			table1.setColumnType(3, DataType.EMAIL);
			
			//Create new record objects with the specified fields. Field types are automatically set to the most likely data type, but
			//it is also possible to set it to a custom type, e.g. if you wanted to make the value "25" alphanumeric for some reason.
			Record record1 = new Record(new Field("Michael"), new Field("25"), new Field("michaeltalu@gmail.com"));
			Record record2 = new Record(new Field("0944737", DataType.ALPHANUMERIC), new Field("32"), new Field("0944737@gmail.com"));
			
			Record record3 = new Record(new Field(record1.getKey()));
			
			//Add new records to the table. Records must have the same number of fields as the table has columns and data types must match.
			table1.addRecord(record1);
			table1.addRecord(record2);
			
			//Print table to the terminal with nice formatting
			table1.printTable();
			
			//Save table to temporary .table file in working directory
			table1.writeToFile("table_example", null);
			
			//Create new table file and load data from .table file into it, then print to terminal
			Table table2 = new Table();
			table2.readFromFile("table_example.table", null);
			System.out.println("\n");
			table2.printTable();
			File file = new File("table_example.table");
			file.delete();
			
			//Add the two tables to database. Tables must have unique names
			db.addTable("Table1", table1);
			db.addTable("Table2", table2);
			
			//Save the database to a temporary .db folder
			db.saveDatabase("Database_Example");
			
			//Create new database, load data from .db folder into it and then print all tables to the terminal
			Database db2 = new Database();
			db2.loadDatabase("Database_Example.db");
			for(Table table : db2.getTables()) {
				System.out.println("\n");
				table.printTable();
			}
			
			MyUtilities.deleteDirectory(new File("Database_Example.db"));
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
}
