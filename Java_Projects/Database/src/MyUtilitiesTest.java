import static org.junit.Assert.*;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;

public class MyUtilitiesTest {

	@Test
	public void testContainsRepeatedElement() {
		ArrayList<String> list = new ArrayList<String>(Arrays.asList("A string", "A different string"));
		assertFalse(MyUtilities.containsRepeatedElement(list));
		
		ArrayList<String> list2 = new ArrayList<String>(Arrays.asList("A string", "A string"));
		assertTrue(MyUtilities.containsRepeatedElement(list2));
	}
	
	@Test
	public void testElementsCorrectSize() {
		String[] array = {"Testing", "Testing", "Testing"};
		assertTrue(MyUtilities.elementsCorrectSize(array));
		
		array[2] = "This string is greater than twenty-five characters";
		assertFalse(MyUtilities.elementsCorrectSize(array));
	}

}
