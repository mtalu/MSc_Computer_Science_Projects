/**
 * The Item class represents any item that can be picked up by the player. Items in this game usually
 * act either as keys to unlock another room or as decoys for the player.
 */
public class Item implements java.io.Serializable {
	private String name;
	private String usedString;
	private boolean used;
	private boolean isKey;
	
	/**
	 * Creates new item object, all items are usable and therefore must have a used string.
	 * Certain items are used as keys to unlock a new room, while others act as decoys (user thinks
	 * they might be a key, but they have no effect).
	 * @param name
	 * @param usedString
	 * @param isKey
	 */
	public Item(String name, String usedString, boolean isKey) {
		this.name = name;
		this.usedString = usedString;
		used = false;
		this.isKey = isKey;
	}
	
	public String getName() {
		return name;
	}
	
	public String getUsedString() {
		return usedString;
	}
	
	public boolean hasBeenUsed() {
		return used;
	}
	
	public void setUsed(boolean used) {
		this.used = used;
	}
	
	public boolean isKey() {
		return isKey;
	}
}
