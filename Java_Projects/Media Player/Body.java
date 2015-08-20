import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.BevelBorder;

import javazoom.jlgui.basicplayer.BasicPlayer;

import java.io.File;
import java.nio.file.Path;
import java.net.*;
import java.util.Arrays;
import java.util.ArrayList;

/**
 * Creates the media player body for a given frame.
 * @author Michael
 *
 */
public class Body {
	private static Body body;
	private My_Player player;
	private JFrame frame;
	private Container contentPane;
	private JPanel southPanel;
	private JPanel controllerPanel;
	private JPanel playlistPanel;
	private JPanel nowPlayingPanel;
	private JPanel displayPanel;
	private JSlider seekBar;
	private static final int SEEK_DIVIDER = 500;
	
	/*
	 * Adds controller panel to content pane of the frame
	 * @param frame the frame to add controller panel to
	 */
	private Body(JFrame frame) {
		assert(frame != null) : "Unitialized JFrame passed to constructor";
		
		this.frame = frame;
		contentPane = frame.getContentPane();
		contentPane.setLayout(new BorderLayout());
		
		player = My_Player.getInstance();
		
		setUpSouthPanel();
		setUpControllerPanel();
		setUpPlaylistPanel();
		setUpNowPlayingPanel();
		setUpDisplayPanel();
	}
	
	/**
	 * Initialize body when first called, return instance each successive call
	 * @param frame the frame on which to build body
	 * @return the body object
	 */
	public static Body getInstance(JFrame frame) {
		if(body == null) {
			body = new Body(frame);
		}
		
		return body;
	}
	
	/**
	 * Executes all UI actions including playing an actual track
	 */
	public void play() {
		player.play();
		/*Timer timer = new Timer(2000, new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(timer.getDelay() == 1000) {
					
				}
			}
		});*/
		refreshCurrentlyPlaying();
	}
	
	/**
	 * Executes all UI actions including stopping actual track
	 */
	public void stop() {
		player.stop();
		refreshTitleSelection();
		refreshCurrentlyPlaying();
	}
	
	/**
	 * Executes all UI actions including pausing actual track
	 */
	public void pause() {
		player.pause();
		removeCurrentlyPlaying();
		refreshCurrentlyPlaying();
	}
	
	/**
	 * Executes all UI actions including selecting previous track
	 */
	public void previous() {
		player.previous();
		refreshTitleSelection();
		refreshCurrentlyPlaying();
	}
	
	/**
	 * Executes all UI actions including selecting next track
	 */
	public void next() {
		player.next();
		refreshTitleSelection();
		refreshCurrentlyPlaying();
	}
	
	/**
	 * Creates label with file title and adds to playlist panel. 
	 * Also adds mouse listener for single click for visual indication of 
	 * file being selected and mouse listener for double click to play file
	 * directly
	 * @param file the file to be added to playlist panel
	 */
	public void addToPlaylistPanel(My_Track track) {
		JLabel title = track.getLabel();
		title.setToolTipText(track.getInformationString());
		setTitleSelectionColours(title);
		title.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				setTitleSelectionColours(title);
				player.open(track);
				refreshTitleSelection();
			}
		});
		title.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				if(e.getClickCount() == 2) {
					player.open(track);
					play();
				}
			}
		});

		playlistPanel.add(title);
		frame.pack();
		frame.validate();
	}
	
	/*
	 * Creates south panel area for controller and now playing information
	 */
	private void setUpSouthPanel() {
		southPanel = new JPanel();
		southPanel.setLayout(new BorderLayout());
		southPanel.setBorder(BorderFactory.createSoftBevelBorder(BevelBorder.RAISED));
		contentPane.add(southPanel, BorderLayout.SOUTH);
	}
	
	/*
	 * Creates controller panel and adds it to content pane
	 */
	private void setUpControllerPanel() {
		controllerPanel = new JPanel();
		southPanel.add(controllerPanel, BorderLayout.CENTER);
		
		JButton previousButton = addButton("images/backward_button.png", controllerPanel);
		previousButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				previous();
			}
		});
		
		JButton pauseButton = addButton("images/pause_button.png", controllerPanel);
		pauseButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				pause();
			}
		});
		
		JButton playButton = addButton("images/play_button.png", controllerPanel);
		playButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				play();
			}
		});
		
		JButton stopButton = addButton("images/stop_button.png", controllerPanel);
		stopButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				stop();
			}
		});
		
		JButton nextButton = addButton("images/forward_button.png", controllerPanel);
		nextButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				next();
			}
		});
	}
	
	/*
	 * Creates playlist panel area and adds it to content pane
	 */
	private void setUpPlaylistPanel() {	
		playlistPanel = new JPanel();
		playlistPanel.setLayout(new BoxLayout(playlistPanel, BoxLayout.Y_AXIS));
		playlistPanel.setBorder(BorderFactory.createTitledBorder("Playlist"));
		
		JLabel paddingLabel = new JLabel("                    ");
		playlistPanel.add(paddingLabel);
		
		JScrollPane scrollPane = new JScrollPane(playlistPanel);
		scrollPane.setBorder(BorderFactory.createSoftBevelBorder(BevelBorder.RAISED));
		scrollPane.setPreferredSize(new Dimension(150, 150));
		scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		
		contentPane.add(scrollPane, BorderLayout.WEST);
	}
	
	private void setUpNowPlayingPanel() {
		nowPlayingPanel = new JPanel();
		nowPlayingPanel.add(new JLabel(" "));
		southPanel.add(nowPlayingPanel, BorderLayout.NORTH);
	}
	
	private void setUpDisplayPanel() {
		displayPanel = new JPanel();
		displayPanel.setLayout(new BorderLayout());
		contentPane.add(displayPanel, BorderLayout.CENTER);
		
		seekBar = new JSlider(JSlider.HORIZONTAL, 0, SEEK_DIVIDER, 0);
		seekBar.setMinimum(5);
		seekBar.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				player.seek(seekBar.getValue(), SEEK_DIVIDER);
			}
		});
		displayPanel.add(seekBar, BorderLayout.SOUTH);
		
	}
	
	/*
	 * Wrapper for creating button with image icon and adding it to a JPanel
	 */
	private JButton addButton(String imgDirectory, JPanel panel) {
		JButton button = null;
		
		try {
			URL url = this.getClass().getResource(imgDirectory);
			ImageIcon icon = new ImageIcon(url);
			button = new JButton(icon);
			panel.add(button);
		}
		catch(NullPointerException e) {
			System.err.println("Invalid image directory");
			e.printStackTrace();
			System.exit(1);
		}
		
		return button;
	}
	
	/*
	 * Highlights title if not null and resets background and foreground colours of all 
	 * other titles in playlist panel.
	 */
	private void setTitleSelectionColours(JLabel title) {
		Component[] components = playlistPanel.getComponents();
		
		for(int i = 0; i < components.length; i++) {
			JLabel titleLabel = (JLabel) components[i];
			titleLabel.setOpaque(true);
			titleLabel.setForeground(Color.BLACK);
			titleLabel.setBackground(playlistPanel.getBackground());
		}
		
		if(title != null) {
			title.setOpaque(true);
			title.setForeground(Color.WHITE);
			title.setBackground(playlistPanel.getBackground().darker());
		}
	}
	
	/*
	 * Refreshes title selection colours. Can be called whenever user selects
	 * different track in MyPlayer or stops track.
	 */
	public void refreshTitleSelection() {
		My_Track currentTrack = player.getPlaylist().getCurrentTrack();
		
		if(currentTrack == null) {
			setTitleSelectionColours(null);
		}
		else {
			JLabel currentTitle = currentTrack.getLabel();
			setTitleSelectionColours(currentTitle);
		}
	}
	
	private void removeCurrentlyPlaying() {
		nowPlayingPanel.removeAll();
		nowPlayingPanel.add(new JLabel(" "));
		frame.revalidate();
		frame.repaint();
	}
	
	private void refreshCurrentlyPlaying() {
		nowPlayingPanel.removeAll();
		My_Track currentTrack = player.getPlaylist().getCurrentTrack();
		
		if(currentTrack != null) {
			if(player.getStatus() == BasicPlayer.PLAYING || player.getStatus() == BasicPlayer.PAUSED) {
				ImageIcon noteIcon = new ImageIcon("images/note.png");
				ImageIcon noteIcon2 = new ImageIcon("images/note.png");
				JLabel nowPlaying = new JLabel(currentTrack.getNowPlayingString(), noteIcon, JLabel.CENTER);
				JLabel noteLabel = new JLabel(noteIcon2);
				nowPlaying.setFont(new Font("Serif", Font.BOLD, 10));
				nowPlaying.setForeground(Color.BLACK);
				nowPlayingPanel.add(nowPlaying);
				nowPlayingPanel.add(noteLabel);
			}
			else {
				nowPlayingPanel.add(new JLabel(" "));
			}
		}
		else {
			nowPlayingPanel.add(new JLabel(" "));
		}
		
		frame.revalidate();
		frame.repaint();
	}
}
