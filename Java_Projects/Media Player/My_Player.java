import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.io.File;

import java.net.MalformedURLException;
import java.net.URL;
//import javazoom.jlgui.basicplayer.BasicPlayer;
//import javazoom.jlgui.basicplayer.BasicPlayerException;
import javazoom.jl.player.*;

import java.util.ArrayList;

import java.io.FileInputStream;


/**
 * The MyPlayer class encapsulates the functionality of the media player application. It
 * uses the Javazoom basic player library.
 * @author Michael
 *
 */
public class My_Player {
	private static My_Player myPlayer;
	//private BasicPlayer player;
	private Player player;
	private Playlist playlist;
	
	/**
	 * Creates a new basic player and playlist object. Uses singleton design pattern to
	 * enable access to the same player instance from different GUI classes, i.e. Menu
	 * and Body.
	 */
	private My_Player() {
		try {
			//player = new BasicPlayer();
			playlist = new Playlist();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Initializes MyPlayer instance when first called, returns instance each
	 * successive call
	 * @return same instance of MyPlayer
	 */
	public static My_Player getInstance() {
		if(myPlayer == null) {
			myPlayer = new My_Player();
		}
		
		return myPlayer;
	}
	
	/**
	 * Encapsulates basic player's open functionality and adds file
	 * to playlist
	 * @param file the file to be opened
	 */
	public void open(My_Track track) {
		try {
			if(!playlist.contains(track)) {
				playlist.add(track);
			}
			else {
				playlist.setCurrentTrack(track);
			}
			
			//player.open(track.getFile());
			FileInputStream fis = new FileInputStream(track.getFile());
			player = new Player(fis);
		}
		catch(Exception e) {
			System.err.println("Failed to open file");
			e.printStackTrace();
		}
	}
	
	/**
	 * Encapsulates basic player's play functionality 
	 */
	public void play() {
		try {
			if(player.getStatus() == player.PAUSED) {
				//player.resume();
				player.p
			}
			else if(player.getStatus() == player.OPENED) {
				player.play();
			}
			else {
				System.err.println("No file");
			}
		}
		catch(Exception e) {
			System.err.println("Failed to start file playback");
			e.printStackTrace();
		}
	}
	
	/**
	 * Encapsulates basic player's stop functionality
	 */
	public void stop() {
		try {
			player.stop();
			playlist.setCurrentTrack(null);
		}
		catch(Exception e) {
			System.err.println("Failed to stop file playback");
			e.printStackTrace();
		}
	}
	
	/**
	 * Encapsulates basic player's pause functionality
	 */
	public void pause() {
		try {
			player.pause();
		}
		catch(Exception e) {
			System.err.println("Failed to pause file playback");
			e.printStackTrace();
		}
	}
	
	/**
	 * Returns to start of currently open file if playing or paused, to the previous
	 * file in playlist if present or to the last file in playlist.
	 */
	public void previous() {
		try {
			if(playlist.isEmpty()) {
				System.err.println("No file in playlist");
				return;
			}
			
			if(player.getStatus() == BasicPlayer.PLAYING || player.getStatus() == BasicPlayer.PAUSED) {
				//Go back to beginning of current file
				My_Track currentTrack = playlist.getCurrentTrack();
				player.open(currentTrack.getFile());
			}
			else if(player.getStatus() == BasicPlayer.STOPPED || !playlist.hasPrevious()) {
				//Go to last file in playlist
				playlist.setLastTrack();
				My_Track lastTrack = playlist.getCurrentTrack();
				player.open(lastTrack.getFile());
			}
			else if(playlist.hasPrevious()) {
				//Open the previous file in playlist
				playlist.setPrevious();
				My_Track previousTrack = playlist.getCurrentTrack();
				player.open(previousTrack.getFile());
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Proceeds to next file in playlist if present or returns back to the start start
	 */
	public void next() {
		if(playlist.isEmpty()) {
			System.err.println("No file in playlist");
			return;
		}
		
		try {
			if(playlist.hasNext()) {
				playlist.setNext();
			}
			else {
				playlist.setFirstTrack();
			}
			
			My_Track nextTrack = playlist.getCurrentTrack();
			player.open(nextTrack.getFile());
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 *
	 */
	public void seek(int sliderPosition, int seekDivider) {
		try {
			My_Track currentTrack = playlist.getCurrentTrack();
			
			if(currentTrack != null) {
				player.seek(sliderPosition * (currentTrack.getByteSize() / seekDivider));
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Returns playlist
	 * @return playlist
	 */
	public Playlist getPlaylist() {
		return playlist;
	}
	
	/**
	 * Wrapper for returning status of player
	 * @return the current status of the player
	 */
	public int getStatus() {
		return player.getStatus();
	}
	
	/**
	 * Resets player to null so that new instance is created when getInstance called.
	 * Useful so that testing does not interfere with normal functionality
	 */
	public void resetPlayer() {
		myPlayer = null;
	}
}