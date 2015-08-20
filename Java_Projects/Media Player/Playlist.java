import java.util.ArrayList;

/**
 * The Playlist class can store any number of My_Track objects
 * @author Michael
 *
 */
public class Playlist {
	private ArrayList<My_Track> playlist;
	private My_Track currentTrack;
	
	public Playlist() {
		playlist = new ArrayList<>();
	}
	
	/**
	 * Adds My_Track object to the end of the playlist
	 * @param track the track object to add
	 */
	public void add(My_Track track) {
		playlist.add(track);
		currentTrack = track;
	}
	
	/**
	 * Deletes the My_Track object at the specified index if valid and resets current track
	 * @param index the index of track
	 * @throws IndexOutOfBoundsException if index not valid
	 */
	public void delete(int index) {
		if(index < 0 || index > playlist.size()) {
			throw new IndexOutOfBoundsException("Error: No track at given index");
		}
		
		My_Track track = playlist.get(index);
		playlist.remove(track);
		
		if(!isEmpty()) {
			if(index >= 0 && index < playlist.size()) {
				currentTrack = playlist.get(index);
			}
			else {
				currentTrack = playlist.get(index - 1);
			}
		}
		else {
			currentTrack = null;
		}
	}
	
	/**
	 * Gets My_Track object at specified index
	 * @param index the index
	 * @return the My_Track object
	 * @throws IndexOutOfBoundsException if index not valid
	 */
	public My_Track get(int index) {
		if(index < 0 || index > playlist.size()) {
			throw new IndexOutOfBoundsException("Error: No track at given index");
		}
		
		My_Track track = playlist.get(index);
		
		return track;
	}
	
	/**
	 * Returns the current My_Track object
	 * @return
	 */
	public My_Track getCurrentTrack() {
		return currentTrack;
	}
	
	/**
	 * Sets the current My_Track object
	 * @param track a My_Track object that is present in the playlist or null
	 * @throws IllegalArgumentException if My_Track object not in playlist
	 */
	public void setCurrentTrack(My_Track track) {
		if(!playlist.contains(track) && !(track == null)) {
			throw new IllegalArgumentException("My_Track object not present in playlist");
		}
		
		currentTrack = track;
	}
	
	/**
	 * Set current track to first track in playlist if present
	 */
	public void setFirstTrack() {
		if(!playlist.isEmpty()) {
			currentTrack = playlist.get(0);
		}
	}
	
	/**
	 * Set current track to last track in playlist if present
	 */
	public void setLastTrack() {
		if(!playlist.isEmpty()) {
			currentTrack = playlist.get(playlist.size() - 1);
		}
	}
	
	/**
	 * Checks if playlist has next track
	 * @return true if yes, otherwise false. Also returns false if current file is
	 * null, i.e. no file is open
	 */
	public boolean hasNext() {
		if(currentTrack == null) {
			return false;
		}
		
		int nextFileIndex = playlist.indexOf(currentTrack) + 1;
		
		if(nextFileIndex >= playlist.size()) {
			return false;
		}
		
		return true;
	}
	
	/**
	 * Checks if playlist has previous track
	 * @return true if yes, otherwise false
	 */
	public boolean hasPrevious() {
		if(currentTrack == null) {
			return false;
		}
		
		int previousFileIndex = playlist.indexOf(currentTrack) - 1;
		
		if(previousFileIndex < 0) {
			return false;
		}
		
		return true;
	}
	
	/**
	 * Set next file in playlist
	 */
	public void setNext() {
		if(hasNext()) {
			int currentFileIndex = playlist.indexOf(currentTrack);
			currentTrack = playlist.get(currentFileIndex + 1);
		}
	}
	
	/**
	 * Set previous file in playlist
	 */
	public void setPrevious() {
		if(hasPrevious()) {
			int currentFileIndex = playlist.indexOf(currentTrack);
			currentTrack = playlist.get(currentFileIndex - 1);
		}
	}
	
	/**
	 * Checks if playlist is empty
	 * @return true if empty, otherwise false
	 */
	public boolean isEmpty() {
		return playlist.isEmpty();
	}
	
	/**
	 * Wrapper for checking number of tracks in playlist
	 * @return
	 */
	public int numberOfTracks() {
		return playlist.size();
	}
	
	/**
	 * Wrapper to check if playlist contains track
	 * @param track
	 * @return
	 */
	public boolean contains(My_Track track) {
		return playlist.contains(track);
	}
}
