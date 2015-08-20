import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * The GUI class creates a new media player GUI
 * @author Michael
 *
 */
public class GUI {
	private JFrame frame;
	private Menu menu;
	
	/**
	 * Creates a new frame and populates it with a new menu and body before
	 * displaying
	 */
	public GUI() {
		
		createFrame();
		menu = new Menu(frame);
		
		frame.pack();
		frame.setVisible(true);
	}
	
	/*
	 * Creates a new 640 x 480 frame that is centred on the screen
	 */
	private void createFrame() {
		frame = new JFrame("Media Player");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocationByPlatform(true);
	}
}
