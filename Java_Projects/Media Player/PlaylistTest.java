import static org.junit.Assert.*;
import java.io.File;

import org.junit.Test;


public class PlaylistTest {

	@Test
	public void testAdd() {
		My_Track track1 = new My_Track(new File("Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("Canon.mp3"));
		My_Track track3 = new My_Track(new File("Handel - Entrance of the Queen of Sheba.mp3"));
		
		Playlist playlist = new Playlist();
		playlist.add(track1);
		playlist.add(track2);
		playlist.add(track3);
		
		assertTrue(playlist.numberOfTracks() == 3);
		assertTrue(playlist.getCurrentTrack() == track3);
	}
	
	@Test
	public void testDelete() {
		My_Track track1 = new My_Track(new File("Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("Canon.mp3"));
		My_Track track3 = new My_Track(new File("Handel - Entrance of the Queen of Sheba.mp3"));
		
		Playlist playlist = new Playlist();
		playlist.add(track1);
		playlist.add(track2);
		playlist.add(track3);
		
		playlist.delete(2);
		
		assertTrue(playlist.numberOfTracks() == 2);
		assertTrue(playlist.getCurrentTrack() == track2);
		
		playlist.add(track3);
		playlist.delete(0);
		
		assertTrue(playlist.numberOfTracks() == 2);
		assertTrue(playlist.getCurrentTrack() == track2);
		
		playlist.add(track1);
		playlist.delete(1);
		assertTrue(playlist.getCurrentTrack() == track1);
	}
	
	@Test
	public void testSetCurrentTrack() {
		My_Track track1 = new My_Track(new File("Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("Canon.mp3"));
		My_Track track3 = new My_Track(new File("Handel - Entrance of the Queen of Sheba.mp3"));
		
		Playlist playlist = new Playlist();
		playlist.add(track1);
		playlist.add(track2);
		playlist.add(track3);
		
		playlist.setCurrentTrack(track1);
		assertTrue(playlist.getCurrentTrack() == track1);
		playlist.setCurrentTrack(track2);
		assertTrue(playlist.getCurrentTrack() == track2);
		playlist.setCurrentTrack(track3);
		assertTrue(playlist.getCurrentTrack() == track3);
	}
	
	@Test
	public void testSetFirstandSetLastTrack() {
		My_Track track1 = new My_Track(new File("Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("Canon.mp3"));
		My_Track track3 = new My_Track(new File("Handel - Entrance of the Queen of Sheba.mp3"));
		
		Playlist playlist = new Playlist();
		playlist.add(track1);
		playlist.add(track2);
		playlist.add(track3);
		
		playlist.setFirstTrack();
		assertTrue(playlist.getCurrentTrack() == track1);
		playlist.setLastTrack();
		assertTrue(playlist.getCurrentTrack() == track3);
	}
	
	@Test
	public void testSetNextAndSetPrevious() {
		My_Track track1 = new My_Track(new File("Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("Canon.mp3"));
		My_Track track3 = new My_Track(new File("Handel - Entrance of the Queen of Sheba.mp3"));
		
		Playlist playlist = new Playlist();
		playlist.add(track1);
		playlist.add(track2);
		playlist.add(track3);
		
		playlist.setPrevious();
		assertTrue(playlist.getCurrentTrack() == track2);
		playlist.setPrevious();
		assertTrue(playlist.getCurrentTrack() == track1);
		playlist.setNext();
		assertTrue(playlist.getCurrentTrack() == track2);
		playlist.setNext();
		assertTrue(playlist.getCurrentTrack() == track3);
	}
	
	

}
