import java.util.List;
import java.util.HashSet;
import java.io.File;

/**
 * The MyUtilities class contains several general purpose functions that may be useful throughout the
 * DB project
 * @author Michael
 *
 */
public class MyUtilities {
	
	/**
	 * Checks if a list contains repeated strings
	 * @param list
	 * @return
	 */
	public static boolean containsRepeatedElement(List<String> list) {
		HashSet<String> removedDuplicates = new HashSet<String>(list);
		
		if(list.size() != removedDuplicates.size()) {
			return true;
		}
		
		return false;
	}
	
	/**
	 * Checks each element in the array to see if it is below 25 characters
	 * @param elements the string array of elements
	 * @return true if all elements less than or equal to 25 characters, false otherwise
	 */
	public static boolean elementsCorrectSize(String[] elements) {
		for(int i = 0; i < elements.length; i++) {
			if(elements[i].length() > 25) {
				return false;
			}
		}
		
		return true;
	}
	
	/**
	 * Deletes specified directory and any files it may contain. Does not work if directory
	 * contains sub-directories
	 * @param directory the directory to be deleted
	 * @return true if successful, otherwise false
	 */
	public static boolean deleteDirectory(File directory) {
		if(directory.exists()) {
			File[] files = directory.listFiles();
			if(files != null) {
				for(int i=0; i<files.length; i++) {
					files[i].delete();
				}
			}
		}
	
		return(directory.delete());
	}
}
