import java.io.File;
import javax.swing.*;
import com.mpatric.mp3agic.*;

/**
 * The My_Track class stores information about a single track 
 * @author Michael
 *
 */
public class My_Track {
	private File file;
	private JLabel label;
	private String title;
	private String album;
	private String artist;
	
	public My_Track(File file) {
		this.file = file;
		setTrackInformation();
		label = new JLabel(getLabelString());
	}
	
	/**
	 * Returns the File object associated with a track
	 * @return the File object
	 */
	public File getFile() {
		return file;
	}
	
	/**
	 * Returns the JLabel object associated with a track
	 * @return the JLabel object
	 */
	public JLabel getLabel() {
		return label;
	}
	
	/*
	 * Gets track ID3 title or filename
	 */
	public String getLabelString() {
		String labelString = "";
		
		if(title == null) {
			labelString += file.getName();
		}
		else {
			labelString = this.title;
		}
		
		return labelString;
	}
	
	/*
	 * Prepares track information string. Uses html tags for text wrapping as per: 
	 * http://stackoverflow.com/questions/868651/multi-line-tooltips-in-java
	 * @return
	*/
	public String getInformationString() {
		String infoString = "<html>";
		
		infoString += getLabelString();
		
		if(artist != null) {
			infoString += "<br><br><i>" + artist + "</i>";
		}
		
		if(album != null) {
			infoString += "<br><i>" + album + "</i>"; 
		}
		
		infoString += "</html>";
		
		return infoString;
	}
	
	public String getNowPlayingString() {
		String infoString = "<html>";
		
		infoString += "<center>" + getLabelString() + "</center>";
		
		if(artist != null) {
			infoString += "<br><i><center>" + artist + "</center></i>";
		}
		
		infoString += "</html>";
		
		return infoString;
	}
	
	public long getByteSize() {
		return file.length();
	}
	
	/*
	 * Fetches mp3 audio file ID3 information using mp3agic library
	 */
	private void setTrackInformation() {
		try {
			Mp3File mp3 = new Mp3File(file.getPath());
			
			if(mp3.hasId3v2Tag()) {
				title = mp3.getId3v2Tag().getTitle();
				album =  mp3.getId3v2Tag().getAlbum();
				artist = mp3.getId3v2Tag().getArtist();
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
}
