import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.BevelBorder;

import java.net.URL;
import java.util.List;
import java.util.ArrayList;

/**
 * This class contains the functions to set various skins for the media player
 * @author Michael
 *
 */
public class Skins {
	private JFrame frame;
	private Container contentPane;
	private JMenuBar menuBar;
	
	/**
	 * Creates a new default skin
	 * @param contentPane the content pane for which to set skin
	 * @param menuBar the menu bar for which to set skin
	 */
	public Skins(JFrame frame) {
		this.frame = frame;
		this.contentPane = frame.getContentPane();
		this.menuBar = frame.getJMenuBar();
		
		menuBar.setBorder(BorderFactory.createRaisedBevelBorder());
		
		setDefaultSkin();
		
		//Set custom cursor
		Toolkit toolkit = Toolkit.getDefaultToolkit();
		URL url = this.getClass().getResource("images/cursor.png");
		Cursor cursor = toolkit.createCustomCursor(toolkit.getImage(url), new Point(0, 0), "Golden hand");
		frame.setCursor(cursor);
	}
	
	/**
	 * Sets default media player skin
	 */
	public void setDefaultSkin() {
		Color lightGray = new Color(218, 218, 218);
		Color darkGray = new Color(181,181,181);
		
		setSkin(lightGray, darkGray);
	}
	
	
	/**
	 * Sets deep purple media player skin
	 */
	public void setDeepPurpleSkin() {
		Color lightPurple = new Color(130,132,238);
		Color darkPurple = new Color(52,7,118);
		
		setSkin(lightPurple, darkPurple);
	}
	
	/**
	 * Sets pink poison media player skin
	 */
	public void setPinkPoisonSkin() {
		Color lightPink = new Color(235, 18, 213);
		Color darkPink = new Color(236, 133, 224);
		
		setSkin(lightPink, darkPink);
	}
	
	/*
	 * Change all relevant components to given colours
	 */
	private void setSkin(Color lightColour, Color darkColour) {
		List<Component> components = getAllComponents(frame);
		
		for(Component component : components) {
			if(component instanceof JMenu) {
				component.setBackground(lightColour);
				component.setForeground(darkColour);
			}
			else if(component instanceof JMenuItem) {
				component.setBackground(lightColour);
				component.setForeground(darkColour);
			}
			else if(component instanceof JPanel) {
				component.setBackground(lightColour);
			}
			else if(component instanceof JLabel) {
				component.setBackground(lightColour);
				component.setForeground(Color.BLACK);
			}
		}
		
		menuBar.setBackground(lightColour);
		contentPane.setBackground(darkColour);
	
		frame.revalidate();
		frame.repaint();
	}
	
	/* Get all components in given container by calling method recursively
	 * http://stackoverflow.com/questions/6495769/how-to-get-all-elements-inside-a-jframe
	 */
	private List<Component> getAllComponents(final Container c) {
		Component[] comps = c.getComponents();
		List<Component> compList = new ArrayList<Component>();
		for (Component comp : comps) {
			compList.add(comp);
			if (comp instanceof Container) {
				compList.addAll(getAllComponents((Container) comp));
			}
		}
		
		return compList;
	}
}
