import static org.junit.Assert.*;

import org.junit.Test;


public class DataTypesTest {

	@Test
	public void testGetDataType() {
		assertTrue(DataTypes.is("Testing12798", DataType.ALPHANUMERIC));
		assertTrue(DataTypes.is("12798", DataType.INTEGER));
		assertTrue(DataTypes.getDataType("12798&^%£jskjfnjs") == null);
	}
	
	@Test
	public void testAccepts() {
		assertTrue(DataTypes.accepts(DataType.ALPHANUMERIC, DataType.ALPHANUMERIC));
		assertTrue(DataTypes.accepts(DataType.ALPHANUMERIC, DataType.INTEGER));
		assertTrue(DataTypes.accepts(DataType.INTEGER, DataType.INTEGER));
		
		assertFalse(DataTypes.accepts(DataType.INTEGER, DataType.ALPHANUMERIC));
	}

}
