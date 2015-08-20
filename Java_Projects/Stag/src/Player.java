/**
 * The Player class represents the user playing the game. It's main purpose
 * is to hold information about what items the player is carrying.
 */

import java.util.ArrayList;
import java.util.Iterator;

public class Player {
	private String name;
	private ArrayList<Item> items;
	
	/**
	 * Creates new player
	 * @param name
	 */
	public Player(String name) {
		this.name = name;
		items = new ArrayList<Item>();
	}
	
	public String getName() {
		return name;
	}
	
	/**
	 * Adds item to the player's inventory
	 * @param item
	 */
	public void takeItem(Item item) {
		items.add(item);
	}
	
	/**
	 * Removes item from the player's inventory
	 * @param item
	 */
	public void dropItem(String item) {
		
		for(Iterator<Item> it = items.iterator(); it.hasNext();) {
			String currentItem = it.next().getName();
			if(item.equals(currentItem));
			it.remove();
		}
	}
	
	/**
	 * Checks if item is currently in the player's inventory
	 * @param item
	 * @return true if it is
	 */
	public boolean isHolding(String item) {
		for(Item currentItem : items) {
			if(item.equals(currentItem.getName())) {
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * Gets item corresponding to the item name provided
	 * @param itemName
	 * @return the item object or null if not found
	 */
	public Item getItem(String itemName) {
		for(Item currentItem : items) {
			if(itemName.equals(currentItem.getName())) {
				return currentItem;
			}
		}
		
		return null;
	}
}
