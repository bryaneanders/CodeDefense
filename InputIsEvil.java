import java.io.File;
import java.util.Scanner;

import javax.swing.filechooser.FileNameExtensionFilter;


public class InputIsEvil
{

    public static void main(String[] args)
    {
        String firstName = null;
        String lastName = null;
        int num1;
        int num2;
        String inputFile;
        String outputFile;
        Scanner kb = new Scanner(System.in);
        firstName = getFirstName(kb);
        lastName = getLastName(kb);
        num1 = getNumber(kb);
        num2 = getNumber(kb);
        kb.nextLine();
        inputFile = getInput(kb);
        outputFile = getOutput(kb);


    }

    private static String getOutput(Scanner kb)
    {
        String name = null;
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Text File", "txt");
        while(name == null)
        {
            System.out.print("Please enter the output file (Only .txt is allowed): ");
            name = kb.nextLine();
            File temp = new File(name);
            if(!filter.accept(temp))
            {
                name = null;
            }
        }
        return name;
    }

    private static String getInput(Scanner kb)
    {
        // TODO Auto-generated method stub
        return null;
    }

    private static String getFirstName(Scanner kb)
    {
        String firstName = null;
        while(firstName == null)
        {
            System.out.print("Please enter your first name: ");
            firstName = kb.nextLine();
            int length = firstName.length();
            if(length > 50)
            {
                firstName = null;
                System.out.println("First name must be 50 characters or less! Try again!");
            }
        }
        return firstName;
    }

    private static String getLastName(Scanner kb)
    {
        String lastName = null;
        while(lastName == null)
        {
            System.out.print("Please enter your last name: ");
            lastName = kb.nextLine();
            int length = lastName.length();
            if(length > 50)
            {
                lastName = null;
                System.out.println("Last name must be 50 characters or less! Try again!");
            }
        }
        return lastName;
    }

    private static int getNumber(Scanner kb)
    {
        while(true)
        {
            System.out.print("Please enter a number: ");
            try
            {
                int num = kb.nextInt();
                return num;
            }
            catch(Exception e)
            {
                System.out.println("An error has occured with your number.");
                kb.next();
            }
        }   
    }

}
