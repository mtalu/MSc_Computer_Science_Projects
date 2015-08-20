import javax.swing.*;

import java.io.File;

/**
 * The MediaPlayer class creates a new media player and stores its principal components as 
 * private members.
 * @author Michael
 *
 */
public class MediaPlayer {

	/**
	 * Creates new media player
	 * @param args
	 */
	public static void main(String[] args) {
		MediaPlayer mediaPlayer = new MediaPlayer();
	}
	
	/**
	 * Creates the principal media player objects
	 */
	public MediaPlayer() {
		GUI gui = new GUI();
	}
}
