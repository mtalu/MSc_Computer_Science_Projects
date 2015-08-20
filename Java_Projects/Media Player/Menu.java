import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import java.io.File;

import javax.swing.filechooser.FileNameExtensionFilter;

/**
 * Creates a new menu bar for a given frame and populates it with
 * menus and menu items
 * @author Michael
 *
 */
public class Menu {
	private JFrame frame;
	private JMenuBar menuBar;
	private Body body;
	private My_Player player;
	private Skins skin;
	
	/**
	 * Creates a new media player menu
	 * @param frame the frame to add menu to
	 */
	public Menu(JFrame frame) {
		assert(frame != null) : "Unitialized JFrame passed to constructor";
		
		this.frame = frame;
		createMenuBar();
		body = Body.getInstance(frame);
		player = My_Player.getInstance();
		skin = new Skins(frame);
	}
	
	/*
	 * Creates new menu bar object and Skins object with a default media player
	 * skin
	 */
	private void createMenuBar() {
		menuBar = new JMenuBar();
		frame.setJMenuBar(menuBar);
		
		createMenus();
	}
	
	/*
	 * Creates all main media player menus and adds them to menu bar
	 */
	private void createMenus() {
		JMenu mediaPlayerMenu = new JMenu("Media Player");
		menuBar.add(mediaPlayerMenu);
		addMediaPlayerMenuItems(mediaPlayerMenu);
		
		JMenu fileMenu = new JMenu("File");
		menuBar.add(fileMenu);
		addFileMenuItems(fileMenu);
		
		JMenu optionsMenu = new JMenu("Playback");
		menuBar.add(optionsMenu);
		addOptionsMenuItems(optionsMenu);
	}
	
	/*
	 * Creates media player menu items and adds them to media player menu
	 */
	private void addMediaPlayerMenuItems(JMenu mediaPlayerMenu) {
		assert(mediaPlayerMenu != null);
		
		JMenuItem aboutMenuItem = new JMenuItem("About");
		aboutMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				displayAboutDialog();
			}
		});
		mediaPlayerMenu.add(aboutMenuItem);
		
		JMenu skinsMenu = new JMenu("Skins");
		mediaPlayerMenu.add(skinsMenu);
		JMenuItem defaultSkinMenuItem = new JMenuItem("Default");
		skinsMenu.add(defaultSkinMenuItem);
		defaultSkinMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				skin.setDefaultSkin();
			}
		});
		JMenuItem deepPurpleSkinMenuItem = new JMenuItem("Deep Purple");
		skinsMenu.add(deepPurpleSkinMenuItem);
		deepPurpleSkinMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				skin.setDeepPurpleSkin();
			}
		});
		JMenuItem poisonPinkMenuItem = new JMenuItem("Poison Pink");
		skinsMenu.add(poisonPinkMenuItem);
		poisonPinkMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				skin.setPinkPoisonSkin();
			}
		});
		
		JMenuItem exitMenuItem = new JMenuItem("Exit");
		exitMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		mediaPlayerMenu.add(exitMenuItem);
	}
	
	/*
	 * Creates file menu items and adds them to file menu
	 */
	private void addFileMenuItems(JMenu fileMenu) {
		assert(fileMenu != null);
		
		JMenuItem openMenuItem = new JMenuItem("Open");
		openMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				openFileChooser();
			}
		});
		fileMenu.add(openMenuItem);
	}
	
	/*
	 * Creates options menu items and adds them to options menu
	 */
	private void addOptionsMenuItems(JMenu optionsMenu) {
		assert(optionsMenu != null);
		
		JMenuItem playMenuItem = new JMenuItem("Play");
		playMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				body.play();
			}
		});
		optionsMenu.add(playMenuItem);
		
		JMenuItem stopMenuItem = new JMenuItem("Stop");
		stopMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				body.stop();
			}
		});
		optionsMenu.add(stopMenuItem);
		
		JMenuItem pauseMenuItem = new JMenuItem("Pause");
		pauseMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				body.pause();
			}
		});
		optionsMenu.add(pauseMenuItem);
		
		JMenuItem previousMenuItem = new JMenuItem("Previous");
		previousMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				body.previous();
			}
		});
		optionsMenu.add(previousMenuItem);
		
		JMenuItem nextMenuItem = new JMenuItem("Next");
		nextMenuItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				body.next();
			}
		});
		optionsMenu.add(nextMenuItem);
		
	}
	
	/*
	 * Displays file chooser and opens file in player if one
	 * is selected by user. Files currently restricted to mp3 format
	 */
	private void openFileChooser() {
		JFileChooser fileChooser = new JFileChooser();
		FileNameExtensionFilter filter = new FileNameExtensionFilter("mp3 audio", "mp3");
		fileChooser.setFileFilter(filter);
		fileChooser.setMultiSelectionEnabled(true);
		fileChooser.showOpenDialog(frame);
		
		for(File file : fileChooser.getSelectedFiles()) {
			My_Track track = new My_Track(file);
			player.open(track);
			body.addToPlaylistPanel(track);
		}
	}
	
	private void displayAboutDialog() {
		JOptionPane.showMessageDialog(frame, "Media Player version 1.0\n\nCopyright (c) Michael Talu");
	}
}
