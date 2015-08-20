import static org.junit.Assert.*;
import java.io.File;

import org.junit.Test;


public class My_TrackTest {

	@Test
	public void testFile() {
		File file = new File("test/Canon.mp3");
		My_Track track = new My_Track(file);
		assertTrue(track.getFile().getName().equals(file.getName()));
	}
	
	@Test
	public void testJLabel() {
		File file = new File("test/Canon.mp3");
		My_Track track = new My_Track(file);
		assertTrue(track.getLabel().getText().equals(file.getName()));
	}

}
