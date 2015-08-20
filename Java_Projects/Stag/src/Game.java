import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.io.*;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import java.io.File;
import java.awt.*;
import java.awt.Event.*;
import java.io.*;
import javax.swing.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.w3c.dom.CharacterData;

/**
 * The Game class creates all the objects necessary to run the game and contains the
 * main game loop
 */
public class Game {
	
	private Player player;
	private Parser parser;
	private Room currentRoom;
	private String validCommands[];
	
	/**
	 * Creates main game objects
	 */
	public Game() {
		player = new Player("Anon");
		parser = new Parser();
		createRooms();
		validCommands = new String[] {"go", "look", "take", "drop", "use", "help", "quit"};
	}
	
	/**
	 * Starts game and contains main game loop which continually scans in player commands
	 */
	public void startGame() {
		//printIntro();
		System.out.println("\nYour commands are:\n" + validCommands());
		System.out.println(currentRoom.getDescription());
		
		boolean quit = false;
		while(!quit) {
			try{
				Command command = parser.getCommand();
				quit = processCommand(command);
			}
			catch(InvalidCommandException e) {
				System.out.println(e);
			}
		}
	}
	
	private boolean processCommand(Command command) {
		boolean quit = false;
		String commandWord = command.getCommandWord();
		
		switch(commandWord) {
		case "go":
			goCommand(command);
			break;
		case "help":
			helpCommand(command);
			break;
		case "quit": 
			quit = quitCommand(command);
			break;
		case "look":
			lookCommand(command);
			break;
		case "take":
			takeCommand(command);
			break;
		case "drop":
			dropCommand(command);
			break;
		case "use":
			useCommand(command);
			break;
		default:
			commandNotRecognizedError();
		}
		
		return quit;
	}
	
	private void goCommand(Command command) {
		if(!command.hasSecondWord()) {
			System.out.println("Go where?");
			return;
		}
		
		String direction = command.getSecondWord();
		Room nextRoom = currentRoom.getExit(direction);
		
		if(nextRoom == null) {
			System.out.println("Can't go there");
			return;
		}
		
		if(nextRoom.isLocked()) {
			System.out.println(nextRoom.getLockedString());
			return;
		}
		
		currentRoom = nextRoom;
		System.out.println(currentRoom.getDescription());
		
	}
	
	private void takeCommand(Command command) {
		if(!command.hasSecondWord()) {
			commandNotRecognizedError();
			return;
		}
		
		String itemName = command.getSecondWord();
		
		if(currentRoom.hasItem(itemName)) {
			Item item = currentRoom.getItem(itemName);
			player.takeItem(item);
			currentRoom.removeItem(item);
			System.out.println("You take the " + itemName);
		}
		else {
			System.out.println("There is no " + itemName + " to take");
		}
	}
	
	private void dropCommand(Command command) {
		if(!command.hasSecondWord()) {
			commandNotRecognizedError();
			return;
		}
		
		String itemName = command.getSecondWord();
		
		if(player.isHolding(itemName)) {
			Item item = player.getItem(itemName);
			player.dropItem(itemName);
			currentRoom.addItem(item);
			System.out.println("You drop the " + itemName);
		}
		else {
			System.out.println("You have no " + itemName + " to drop");
		}
	}
	
	private void useCommand(Command command) {
		if(!command.hasSecondWord()) {
			commandNotRecognizedError();
			return;
		}
		
		String itemName = command.getSecondWord();
		
		if(player.isHolding(itemName)) {
			Item item = player.getItem(itemName);
			currentRoom.useItem(item, currentRoom);
		}
		else {
			System.out.println("You don't have this item to use");
		}
	}
	
	private void helpCommand(Command command) {
		if(command.hasSecondWord()) {
			commandNotRecognizedError();
			return;
		}
		
		System.out.println("Commands:\n" + validCommands());
	}
	
	private boolean quitCommand(Command command) {
		if(command.hasSecondWord()) {
			commandNotRecognizedError();
			return false;
		}
		else {
			return true;
		}
	}
	
	private void lookCommand(Command command) {
		if(command.hasSecondWord()) {
			commandNotRecognizedError();
			return;
		}
		
		System.out.println(currentRoom.getLookString());
	}
	
	private String validCommands() {
		String validCommandString = "";
		for(int i = 0; i < validCommands.length; i++) {
			validCommandString += validCommands[i] + "\n";
		}
		
		return validCommandString;
	}
	
	private void commandNotRecognizedError() {
		System.out.println("Command not recognized");
	}
	
	/**
	 * Read introduction from text file and print to terminal
	 */
	private void printIntro() {
		try(BufferedReader reader = new BufferedReader(new FileReader("the_riad_intro"))) {
			String currentLine;
			
			while((currentLine = reader.readLine()) != null ) {
				System.out.println(currentLine);
				currentLine = reader.readLine();
				sleep(Integer.parseInt(currentLine));
			}
		}
		catch(IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
		catch(NumberFormatException e) {
			System.err.println("Error: Every other line of intro file should be a number");
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	private void sleep(int seconds) {
		try {
			TimeUnit.SECONDS.sleep(seconds);
		}
		catch (InterruptedException e) {
			//Do nothing
		}
	}
	
	/**
	 * Read in room objects and room elements from XML file
	 */
	private void createRooms() {
		ArrayList<Room> gameRooms = new ArrayList<Room>();
		
		try {
			File file = new File("rooms.xml");
			DocumentBuilderFactory dfb = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dfb.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();
			Node root = doc.getDocumentElement();
			System.out.println("Root element " + doc.getDocumentElement().getNodeName());
			NodeList nodeList = root.getChildNodes();//doc.getElementsByTagName("room");
			
			for(int i = 0, count = 0; i < nodeList.getLength(); i++) {
				Node room = nodeList.item(i);
				if(room.getNodeType() == Node.ELEMENT_NODE) {
					Element roomEl = (Element) room;
					String roomName = roomEl.getElementsByTagName("name").item(count).getTextContent();
					String roomDescr = roomEl.getElementsByTagName("description").item(count++).getTextContent();
					System.out.println(roomName + " " + roomDescr);
					Room newRoom = new Room(roomName, roomDescr);
					gameRooms.add(newRoom);
					System.out.println("A room");
					NodeList roomElements = room.getChildNodes();
					for(int j = 0, count2 = 0; j < roomElements.getLength(); j++) {
						Node roomElement = roomElements.item(j);
						if(roomElement.getNodeType() == Node.ELEMENT_NODE) {
							Element roomChild = (Element) roomElement;
							switch(roomElement.getNodeName()) {
							case "surroundings":
								Text textNode = (Text) roomChild.getFirstChild();
								newRoom.setSurroundings(textNode.getData());
								break;
							case "item":
								NodeList itemElements = roomElement.getChildNodes();
								//newRoom.addItem(new Item(itemName.getNodeValue()));
							}
						}
					}
					
				}
			}
			
			currentRoom = gameRooms.get(0);
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		
		
	}
}
