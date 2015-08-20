/**
 * The Command class stores one or two word commands
 */
public class Command {
	private String commandWord;
	private String secondWord;
	
	/**
	 * Constructor to be used for one word commands
	 * @param command word
	 */
	public Command(String commandWord) {
		this.commandWord = commandWord;
		this.secondWord = null;
	}
	
	/**
	 * constructor to be used for two word commands
	 * @param commandWord
	 * @param secondWord
	 */
	public Command(String commandWord, String secondWord) {
		this.commandWord = commandWord;
		this.secondWord = secondWord;
	}
	
	/**
	 * Returns the command word
	 * @return command word
	 */
	public String getCommandWord() {
		return commandWord;
	}
	
	/**
	 * returns the second word
	 * @return second word
	 */
	public String getSecondWord() {
		return secondWord;
	}
	
	/**
	 * checks if command has second word
	 * @return true if it does
	 */
	public boolean hasSecondWord() {
		return secondWord == null ? false : true;
	}
}
