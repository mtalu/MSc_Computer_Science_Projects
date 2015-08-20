import static org.junit.Assert.*;

import org.junit.Test;


public class RecordTest {
	
	@Test
	public void testAddAndGetField() {
		try {
			Field field = new Field("Testing");
			Record testRecord = new Record(field);
			assertTrue(field == testRecord.getField(1));
		}
		catch(Exception e) {
			fail("Should not throw exception");
		}
	}
	
	@Test
	public void testKey() {
		Record testRecord = new Record();
		String key = testRecord.getKey();
		assertTrue(DataTypes.getDataType(key) == DataType.KEY);
	}
}
