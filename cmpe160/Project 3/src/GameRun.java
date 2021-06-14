import java.io.PrintStream;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

public class GameRun {

	public int[][] table;
	public int size;
	public Tree tree;
	public PrintStream printStream;

	public GameRun(int[][] table, int size, Tree tree, PrintStream printStream) {
		this.table = table;
		this.size = size;
		this.tree = tree;
		this.printStream = printStream;
		tree.root = new Node(table);
		tree.root.parent = null;
		int row = -10, column = -10;
		run(table, size, tree, row, column,printStream);

	}

	public void run(int[][] table, int size, Tree tree, int row, int column,PrintStream printStream) {
		
		Queue<Node> queue = new LinkedList<Node>();
		queue.add(tree.root);
		HashSet<String> traversed = new HashSet<>();
		Node foundNode = null;
		int[][] target = new int[size][size];
		for (int k = 0; k < size; k++) {
			for (int l = 0; l < size; l++) {
				target[k][l] = size * k + l + 1;
			}
		}
		target[size - 1][size - 1] = 0;
		while (!queue.isEmpty()) {
			Node current = (Node) queue.poll();
			if (isTarget(current, target)) {
				foundNode = current;
				break;
			}
			traversed.add(tableTostring(current.data));
			int[][] newTable = current.data;
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (newTable[i][j] == 0) {
						row = i;
						column = j;
					}
				}
			}

			if (row - 1 >= 0) {
				int[][] a = up(newTable, row, column);
				if (!traversed.contains(tableTostring(a))) {
					tree.addUp(current, a);
					queue.add(current.upChild);
				}
			}

			if (row + 1 < size) {
				int[][] a = down(newTable, row, column);
				if (!traversed.contains(tableTostring(a))) {
					tree.addDown(current, a);
					queue.add(current.downChild);
				}
			}

			if (column - 1 >= 0) {
				int[][] a = left(newTable, row, column);
				if (!traversed.contains(tableTostring(a))) {
					tree.addLeft(current, a);
					queue.add(current.leftChild);

				}
			}

			if (column + 1 < size) {

				int[][] a = right(newTable, row, column);
				if (!traversed.contains(tableTostring(a))) {
					tree.addRight(current, a);
					queue.add(current.rightChild);
				}
			}
		}
		if (foundNode != null) {
			Node c = foundNode;
			Stack<Character> resultStack = new Stack<>();
			while (c.parent != null) {
				resultStack.push(c.fromParent);
				c = c.parent;
			}
			while (!resultStack.isEmpty()) {
				printStream.print(resultStack.pop());
			}
			printStream.println();
		} else {
			printStream.println("N");
		}
	}

	public int[][] up(int[][] table, int row, int column) {
		int[][] a = new int[size][size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				a[i][j] = table[i][j];
			}
		}
		int t = a[row - 1][column];
		a[row - 1][column] = a[row][column];
		a[row][column] = t;
		return a;
	}

	public int[][] down(int[][] table, int row, int column) {
		int[][] a = new int[size][size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				a[i][j] = table[i][j];
			}
		}
		int t = a[row + 1][column];
		a[row + 1][column] = a[row][column];
		a[row][column] = t;
		return a;

	}

	public int[][] left(int[][] table, int row, int column) {
		int[][] a = new int[size][size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				a[i][j] = table[i][j];
			}
		}
		int t = a[row][column - 1];
		a[row][column - 1] = a[row][column];
		a[row][column] = t;
		return a;
	}

	public int[][] right(int[][] table, int row, int column) {
		int[][] a = new int[size][size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				a[i][j] = table[i][j];
			}
		}
		int t = a[row][column + 1];
		a[row][column + 1] = a[row][column];
		a[row][column] = t;
		return a;
	}

	public boolean isTarget(Node current, int[][] target) {

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (current.data[i][j] != target[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	public void printTable(Node current) {

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				System.out.print(current.data[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println();
	}

	public String tableTostring(int[][] table) {
		String result = "";
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				result += table[i][j];
			}
		}
		return result;
	}
}
