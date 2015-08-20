import static org.junit.Assert.*;

import org.junit.Test;


public class FieldTest {

	@Test
	public void testValidAlphanumField() {
		try {
			Field field = new Field("Testing20");
			Field field2 = new Field ("Testing20", DataType.ALPHANUMERIC);
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testInvalidAlphanumField() {
		try {
			Field field = new Field("&%^Testing*&^");
			fail("Should throw exception");
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
		
		try {
			Field field = new Field("&%^Testing*&^", DataType.ALPHANUMERIC);
			fail("Should throw exception");
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
	}
	
	@Test
	public void testValidIntegerField() {
		try {
			Field field = new Field("295", DataType.INTEGER);
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testInvalidIntegerField() {
		try {
			Field field = new Field("295.89", DataType.INTEGER);
			fail("Should throw exception");
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
	}
	
	@Test
	public void testSetValueValid() {
		try {
			Field field = new Field("Testing20");
			field.setValue("Testing1256");
			field.setValue("254466");
			Field field2 = new Field("254", DataType.INTEGER);
			field2.setValue(("3452"));
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testSetValueInvalid() {
		try {
			Field field = new Field("Testing20");
			field.setValue("&%^Testing*&^");
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
		
		try {
			Field field2 = new Field("254", DataType.INTEGER);
			field2.setValue(("Testing20"));
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
	}
	
	@Test
	public void testSetTypeValid() {
		try {
			Field field = new Field("23546", DataType.ALPHANUMERIC);
			field.setType(DataType.INTEGER);
		}
		catch(Exception e) {
			e.printStackTrace();
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testSetTypeInvalid() {
		try {
			Field field = new Field("Testing23546", DataType.ALPHANUMERIC);
			field.setType(DataType.INTEGER);
		}
		catch(Exception e) {
			assertTrue(e instanceof UnsupportedDataTypeException);
		}
	}

}
