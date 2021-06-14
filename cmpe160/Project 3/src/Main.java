import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws FileNotFoundException {
		
		Tree tree = new Tree();
		PrintStream printStream = new PrintStream(new File(args[1]));
		Scanner fileScan = new Scanner(new File(args[0]));
		String input = fileScan.nextLine();
		String newInput = "";
		fileScan.close();
		int count= 0;
		for (int i = 0; i < input.length(); i++) {
			char c = input.charAt(i);
			if (c == '-') {
				count++;
			}
		}
		for(int k = 0; k < count; k++) {
			newInput += input.substring(0, input.indexOf('-'));
			input = input.substring(input.indexOf('-')+1);
			newInput += "\n";
		}
		newInput += input;
		
		int size = (int) Math.sqrt(count+1);
		int[][] table = new int[size][size];
		Scanner newInputScan = new Scanner(newInput);
		for(int l = 0; l<size; l++) {
			for(int lm = 0; lm<size; lm++) {
				table[l][lm] = Integer.parseInt(newInputScan.nextLine());
			}
		}
		newInputScan.close();
		
		GameRun game = new GameRun(table,size,tree,printStream);
	}
}
