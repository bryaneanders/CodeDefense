import java.io.File;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
	
public final class DefendTheFort // changed class to final
{
	private static Scanner kb = null;		// added global variables
	private final static int MAX_BUF = 50;
	private final static Pattern regx = Pattern.compile("^[a-zA-Z0-9]+$");
	
    public static void main(String[] args)
    {
			kb = new Scanner(System.in); //allocate scanner

			String outputFile;
			
			outputFile = getOutput();
			
			System.out.println("Filename is: " + outputFile );
	
	
    }

    private static String getOutput()
    {
        String name = "";
        Matcher match; 
        boolean isValid = false;
        	// asking user for only the name of the file without extension and adding ".txt" 
        	// if input is validated
        while ( !isValid )
        	{
        		System.out.print("Please enter the output file name WITHOUT extension: ");
        		name = kb.next();
        		match = regx.matcher(name);
        		if ( match.matches() == true && name.length() < MAX_BUF )
        		{
        			name += ".txt";
        			isValid = true;
        		}
        	}
                 
        return name;
    }
}
	
 
