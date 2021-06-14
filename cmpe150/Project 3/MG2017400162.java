import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Scanner;

public class MG2017400162 {

	public static void main(String[] args) throws FileNotFoundException {
		readFile();
	}
	//to read file and print changed numbers
	public static void readFile() throws FileNotFoundException{
		Scanner fileScan = new Scanner(new File("input.txt"));
		String size = fileScan.next();//to take file size 
		String rowNumber = size.substring(0,size.indexOf("x"));
		int row = Integer.parseInt(rowNumber);//it represents row number
		String columnNumber = size.substring(size.indexOf("x")+1);
		int column = Integer.parseInt(columnNumber);//it represents column number
		int array[][] = new int[row][column];//creates an array
		String strArray[][] = new String[row][column];//it holds letters
		fileTable(row,column,array,strArray,fileScan,0,0);//it reads file
		printTable(array,0,0);//it prints just numbers
		changeTable(array,strArray,0,0);//all changes happen this method
		System.out.println("\n");
		printTable(array,0,0);//it prints changed table
	}
	//This method reads inputs from file
	public static void fileTable(int row, int column, int[][] array, String[][] strArray, Scanner fileScan, int r, int c) {
		String a = fileScan.next();
		strArray[r][c] = a.substring(0, 1);//holds all letters in file
		array[r][c]=Integer.parseInt(a.substring(1));//holds all numbers in the file and convert them to integers
		if (fileScan.hasNext()) {
			if(c<column-1) {
				fileTable(row,column,array,strArray,fileScan,r,c+1);
			}
			else{
				fileTable(row,column,array,strArray,fileScan,r+1,0);
			}
		}
	}
	//This method prints numbers of file
	public static void printTable(int[][] array, int i, int j) {
		System.out.print(array[i][j] + " ");
	    if (i == array.length-1 && j == array[i].length-1){
	        return;
	    }
	    if (j == array[i].length-1){
	        j = 0;
	        ++i;
	        System.out.println();
	        printTable(array, i, j);
	    }
	    else {
	        ++j;
	        printTable(array, i, j);
	    }
	}
	//This method changes table according to letters 
	public static void changeTable(int[][] array, String[][] strArray, int k, int l) {
		if (k < array.length && l < array[k].length){
			if(strArray[k][l].equals("R")) {
				int[] newArray = new int[array[k].length];
				changeR(array,strArray,k,0,newArray);
				Arrays.sort(newArray);
				array[k][l] = newArray[array[k].length-1];
			}
			if(strArray[k][l].equals("C")) {
				int[] newArrayC = new int[array.length];
				changeC(array,strArray,0,l,newArrayC);
				Arrays.sort(newArrayC);
				array[k][l] = newArrayC[(array.length-1)/2];
			}
			if(strArray[k][l].equals("N")) {
				int m = k;
				int n = l;
				changeN(array,strArray,m,n);
			}
			if(strArray[k][l].equals("D")) {
				int p = k;
				int q = l;
				int[] sum = new int[1];
				sum[0] = array[p][q];
				int[] count = new int[1];
				count[0] = 1;
				changeD(array,strArray,p,q,sum,count,k,l);
				array[k][l] = sum[0]/count[0];
			}
			if (l == array[k].length-1){
		        l = 0;
		        ++k;
		        changeTable(array,strArray,k,l);
			}
			else {
		        ++l;
		        changeTable(array,strArray,k,l);
			}
		}
	}
	//to change number if letter equals R
	public static void changeR(int[][] array, String[][] strArray, int k, int l, int[] newArray) {
		if(l<array[k].length) {
			newArray[l]=array[k][l];
			changeR(array,strArray,k,l+1,newArray);
		}
	}
	//to change number if letter equals C
	public static void changeC(int[][] array, String[][] strArray, int k, int l, int[] newArrayC) {
		if(k<array.length) {
			newArrayC[k]=array[k][l];
			changeC(array,strArray,k+1,l,newArrayC);
		}
	}
	//to change number if letter equals N
	public static void changeN(int[][] array, String[][] strArray, int m, int n) {
		if(m+1<array.length && n<array[m].length && strArray[m+1][n].equals("N")) {
			array[m+1][n]=array[m][n];
			strArray[m+1][n] = "X";
			changeN(array,strArray,m+1,n);
		}
		if(m<array.length && n+1<array[m].length && strArray[m][n+1].equals("N")) {
			array[m][n+1]=array[m][n];
			strArray[m][n+1] = "X";
			changeN(array,strArray,m,n+1);
		}
		if(m<array.length && n-1>=0 && strArray[m][n-1].equals("N")) {
			array[m][n-1]=array[m][n];
			strArray[m][n-1] = "X";
			changeN(array,strArray,m,n-1);
		}
		if(m-1>=0 && n<array[m].length && strArray[m-1][n].equals("N")) {
			array[m-1][n]=array[m][n];
			strArray[m-1][n] = "X";
			changeN(array,strArray,m-1,n);
		}
		else
			return;
			
	}
	//to change number if letter equals D
	public static void changeD(int[][] array, String[][] strArray, int p, int q, int[] sum, int[] count, int k, int l) {
		if(p+1<array.length && q+1<array[p].length && k<=p && l<=q) {
			sum[0]+=array[p+1][q+1];
			count[0]++;
			changeD(array,strArray,p+1,q+1,sum,count,k,l);
		}
		if(p+1<array.length && q-1>=0 && k<=p && l>=q) {
			sum[0]+=array[p+1][q-1];
			count[0]++;
			changeD(array,strArray,p+1,q-1,sum,count,k,l);
		}
		if(p-1>=0 && q+1<array[p].length && k>=p && l<=q) {
			sum[0]+=array[p-1][q+1];
			count[0]++;
			changeD(array,strArray,p-1,q+1,sum,count,k,l);
		}
		if(p-1>=0 && q-1>=0 && k>=p && l>=q) {
			sum[0]+=array[p-1][q-1];
			count[0]++;
			changeD(array,strArray,p-1,q-1,sum,count,k,l);
		}
	}
}