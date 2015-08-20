import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;

/**
 * The Room class represents any location in the game and stores all of its data, such
 * as name, description of surroundings, items, etc.
 */
public class Room implements java.io.Serializable {
	private String name;
	private String description;
	private String surroundings;
	private String lockedString;
	private boolean isLocked;
	private HashMap<String, Room> exits;
	private ArrayList<Item> items;
	private ArrayList<String> usableItems;
	
	/**
	 * Constructor to be used to create a normal unlocked room
	 * @param name the room name
	 */
	public Room (String name, String description) {
		this.name = name;
		this.description = description;
		surroundings = "Nothing else to see here...";
		isLocked = false;
		lockedString = null;
		exits = new HashMap<String, Room>();
		items = new ArrayList<Item>();
		usableItems = new ArrayList<String>();
	}
	
	/**
	 * Constructor to be used to create a locked room
	 * @param name the room name
	 * @param isLocked true for locked room
	 * @param lockedString string displayed when user tries to enter locked room without first unlocking
	 */
	public Room(String name, boolean isLocked, String lockedString) {
		this.name = name;
		this.isLocked = isLocked;
		this.lockedString = lockedString;
		surroundings = "Nothing else to see here...";
		exits = new HashMap<String,Room>();
		items = new ArrayList<Item>();
		usableItems = new ArrayList<String>();
	}
	
	/**
	 * Set a room's exit
	 * @param direction a string representing any direction
	 * @param nextRoom the room it leads to
	 */
	public void setExit(String direction, Room nextRoom) {
		exits.put(direction, nextRoom);
	}
	
	/**
	 * Get the room object in the given direction
	 * @param direction the direction
	 * @return the room object in this direction
	 */
	public Room getExit(String direction) {
		return exits.get(direction);
	}
	
	/**
	 * Returns the room's name
	 * @return room name
	 */
	public String getName() {
		return name;
	}
	
	/**
	 * Returns the room's main description displayed when first entering a room
	 * @return description
	 */
	public String getDescription() {
		return "You are in " + description + "\n" + getExitString();
	}
	
	/**
	 * Returns a string representation of all the room's possible exits
	 * @return exit string
	 */
	public String getExitString() {
		String exitString = "Exits:";
		Set<String> keys = exits.keySet();
		for(String key: keys) {
			exitString += "\n" + key;
		}
		
		return exitString;
	}
	
	/**
	 * Returns whether or not the room is locked
	 * @return true if locked
	 */
	public boolean isLocked() {
		return isLocked;
	}
	
	/**
	 * Sets the locked status of the room
	 * @param true for locked
	 */
	public void setLocked(boolean locked) {
		isLocked = locked;
	}
	
	/**
	 * returns the room's locked string
	 * @return
	 */
	public String getLockedString() {
		return lockedString;
	}
	
	/**
	 * Sets the room's long description displayed by the look command
	 * @param surroundings
	 */
	public void setSurroundings(String surroundings) {
		this.surroundings = surroundings;
	}
	
	/**
	 * Returns the room's long description
	 * @return
	 */
	public String getLookString() {
		return surroundings;
	}
	
	/**
	 * Adds a new item to a room
	 * @param the item to be added
	 */
	public void addItem(Item item) {
		items.add(item);
	}
	
	/**
	 * Checks if there is an item with same name as parameter in the room
	 * @param itemName
	 * @return true if there is
	 */
	public boolean hasItem(String itemName) {
		for(Item item : items) {
			if(itemName.equals(item.getName()));
			return true;
		}
		
		return false;
	}
	
	/**
	 * gets the item object with the specified name
	 * @param the name of the item
	 * @return the corresponding item object
	 */
	public Item getItem(String itemName) {
		for(Item item : items) {
			if(itemName.equals(item.getName())) {
				return item;
			}
		}
		
		return null;
	}
	
	/**
	 * Removes the specified item from the room's item lists
	 * @param the item object to be removed
	 */
	public void removeItem(Item item) {
		for(Iterator<Item> it = items.iterator(); it.hasNext();) {
			Item currentItem = it.next();
			if(item == currentItem) {
				it.remove();
			}
		}
	}
	
	/**
	 * Adds an item to the room's usable item list, items only 
	 * have some sort of effect if they are in this list
	 * @param item
	 */
	public void addUsableItem(String item) {
		usableItems.add(item);
	}
	
	/**
	 * Uses an item in the room if it is usable and unlocks the next room if it is also a key
	 * @param item
	 * @param currentRoom
	 */
	public void useItem(Item item, Room currentRoom) {
		if(isUsable(item)) {
			System.out.println(item.getUsedString());
			if(item.isKey()) {
				unlockNextRoom(item, currentRoom);
			}
		}
		else{
			System.out.println("Can't use that here");
		}
	}
	
	/**
	 * Checks if item can be used in the room
	 * @param item the Item object
	 * @return true if it can be used
	 */
	public boolean isUsable(Item item) {
		return usableItems.contains(item.getName());
	}
	
	/**
	 * Sets the next room's locked status to false. The next room must always be a room
	 * accessed by going in the "forward" direction, which is probably a big limitation that
	 * needs fixing.
	 * @param item
	 * @param currentRoom
	 */
	private void unlockNextRoom(Item item, Room currentRoom) {
		if(!item.hasBeenUsed()) {
			Room nextRoom = currentRoom.getExit("forward");
			nextRoom.setLocked(false);
			item.setUsed(true);
		}
		else {
			System.out.println("You have already usedddd the " + item.getName());
		}
	}
}
