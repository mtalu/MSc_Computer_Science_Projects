import java.util.Scanner;

/**
 * The Parser class contains functions to read and validate user commands and create
 * a Command object from them
 */
public class Parser {
	private Scanner input;
	
	/**
	 * Creates new scanner object to read from system input stream
	 */
	public Parser() {
		input = new Scanner(System.in);
	}
	
	/**
	 * Scans in a whole line of input from the user and creates command object if possible
	 * @return the created command object
	 * @throws InvalidCommandException if the user input is not one or two words
	 */
	public Command getCommand() throws InvalidCommandException {
		String tokens[] = input.nextLine().split(" ");
		if(tokens.length == 1) {
			return new Command(tokens[0]);
		}
		else if(tokens.length == 2) {
			return new Command(tokens[0], tokens[1]);
		}
		else{
			throw new InvalidCommandException("Please enter a valid command");
		}		
	}
}
