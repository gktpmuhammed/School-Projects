
public class MG2017400162 {
	
	public static void main(String[] args) {
		int glassSize = Integer.parseInt(args[0]);
		int strawPos  = Integer.parseInt(args[1]);
		
		glass(glassSize,strawPos);
		
	}
	
	public static void glass(int glassSize,int strawPos) {//For print whole glass (top, bottom and straw together)

		int i;
		for(i=0; i<glassSize; i++) {

			if(strawPos%2 == 1) {//For odd strawPos numbers
				if((glassSize-i)*2 >= strawPos-1) {//This checks if spaces between '\/' greater or equal than the spaces before straw
					straw(glassSize,strawPos);
					top(glassSize, i+1,strawPos);
					bottom(glassSize);
				}
			}
			
			if(strawPos%2 == 0) {//For even strawPos numbers
				if((glassSize-i)*2 + 1 >= strawPos-1) {//This checks if spaces between '\/' greater or equal than the spaces before straw
					straw(glassSize,strawPos);
					top(glassSize, i+1,strawPos);
					bottom(glassSize);
				}
			}
		
		}
		
		if(strawPos == 1 || strawPos == 2) {//To prevent disruption of order in case of the location of straw at 1 and 2
			straw(glassSize,strawPos);
			top(glassSize, i+1,strawPos);
			bottom(glassSize);
		}
		
	}
	
	public static void top(int glassSize,int step,int strawPos) {//For the top of glass and straw
		
		//FOR EMPTY LINES
		for(int i=0;i<step-1;i++) {
			
			//For spaces before the '\'
			for(int j=0; j<i; j++) {
				System.out.print(" ");
			}
			
			System.out.print("\\");
			
			int totalSpaces = (glassSize-i)*2;//To determine the location of straw
			
			//For spaces before the straw
			for(int l=0; l<strawPos-1; l++) {
				System.out.print(" ");
			}
			
			if(strawPos-1 < totalSpaces) {//To print 'o' when it necessary
				System.out.print("o");
			}
			
			for(int l=0; l<totalSpaces-strawPos; l++) {//For spaces after the straw
				System.out.print(" ");
			}
			
			System.out.println("/");
		
			
		}
		
		//FOR FILLED LINES
		for(int i=step-1;i<glassSize;i++) {
			//For spaces
			for(int j=0; j<i; j++) {
				System.out.print(" ");
			}
			
			System.out.print("\\");
			
			for(int l=0; l<(glassSize-i)*2; l++) {//To print '*'
				System.out.print("*");
			}
			
			System.out.println("/");
		}
		
	}
	
	public static void bottom(int glassSize) {//For the bottom of the glass
		
		for(int i=0; i<glassSize; i++) {//To print spaces before the '--'
			System.out.print(" ");
		}
		System.out.println("--");
		
		for(int j=0; j<glassSize; j++) {//For print spaces before the '||'
			for(int k=0; k<glassSize; k++) {
				System.out.print(" ");
			}
			System.out.println("||");
		}
	}
	
	public static void straw(int glassSize, int strawPos) {//For the part of the straw inside the glass
		
		for(int i=0; i<strawPos; i++) {//For spaces before the straw
			for(int j=0; j<i; j++) {
				System.out.print(" ");
			}
			System.out.println("o");
		}
	}
	


}


