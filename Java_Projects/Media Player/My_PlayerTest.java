import static org.junit.Assert.*;

import java.io.File;

import javazoom.jlgui.basicplayer.BasicPlayer;

import org.junit.Test;


public class My_PlayerTest {
	 
	@Test
	public void testOpen() {
		My_Player player = My_Player.getInstance();
		
		My_Track track1 = new My_Track(new File("test/Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("test/Canon.mp3"));
		My_Track track3 = new My_Track(new File("test/Handel - Entrance of the Queen of Sheba.mp3"));
		
		player.open(track1);
		assertTrue(player.getPlaylist().getCurrentTrack() == track1);
		assertTrue(player.getPlaylist().numberOfTracks() == 1);
		
		player.open(track2);
		assertTrue(player.getPlaylist().getCurrentTrack() == track2);
		assertTrue(player.getPlaylist().numberOfTracks() == 2);
		
		player.open(track3);
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		assertTrue(player.getPlaylist().numberOfTracks() == 3);
		
		player.resetPlayer();
	}
	
	@Test
	public void testBasicControls() {
		My_Player player = My_Player.getInstance();
		
		My_Track track1 = new My_Track(new File("test/Allegro from Duet in C Major.mp3"));
		player.open(track1);
		
		player.play();
		assertTrue(player.getStatus() == BasicPlayer.PLAYING);
		player.pause();
		assertTrue(player.getStatus() == BasicPlayer.PAUSED);
		player.play();
		assertTrue(player.getStatus() == BasicPlayer.PLAYING);
		player.stop();
		assertTrue(player.getStatus() == BasicPlayer.STOPPED);
		player.play();
		assertTrue(player.getStatus() == BasicPlayer.STOPPED);
		
		player.resetPlayer();
	}
	
	@Test
	public void testPreviousControl() {
		My_Player player = My_Player.getInstance();
		
		My_Track track1 = new My_Track(new File("test/Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("test/Canon.mp3"));
		My_Track track3 = new My_Track(new File("test/Handel - Entrance of the Queen of Sheba.mp3"));
		
		player.open(track1);
		player.open(track2);
		player.open(track3);
		
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		player.play();
		assertTrue(player.getStatus() == BasicPlayer.PLAYING);
		player.previous();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		player.play();
		player.pause();
		assertTrue(player.getStatus() == BasicPlayer.PAUSED);
		player.previous();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		player.previous();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track2);
		player.previous();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track1);
		player.previous();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		
		player.resetPlayer();
	}
	
	@Test
	public void testNextControl() {
		My_Player player = My_Player.getInstance();
		
		My_Track track1 = new My_Track(new File("test/Allegro from Duet in C Major.mp3"));
		My_Track track2 = new My_Track(new File("test/Canon.mp3"));
		My_Track track3 = new My_Track(new File("test/Handel - Entrance of the Queen of Sheba.mp3"));
		
		player.open(track1);
		player.open(track2);
		player.open(track3);
		
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		player.next();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track1);
		player.next();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track2);
		player.next();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track3);
		player.play();
		assertTrue(player.getStatus() == BasicPlayer.PLAYING);
		player.next();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track1);
		player.play();
		player.pause();
		assertTrue(player.getStatus() == BasicPlayer.PAUSED);
		player.next();
		assertTrue(player.getStatus() == BasicPlayer.OPENED);
		assertTrue(player.getPlaylist().getCurrentTrack() == track2);
		
		player.resetPlayer();
	}
}