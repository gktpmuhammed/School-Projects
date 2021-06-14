
public class Tree {

	Node root;

	public void addUp(Node current, int[][] data) {
		current.upChild = new Node(data);
		current.upChild.parent = current;
		current.upChild.fromParent = 'U';
	}
	
	public void addDown(Node current, int[][] data) {
		current.downChild = new Node(data);
		current.downChild.parent = current;
		current.downChild.fromParent = 'D';
	}
	
	public void addLeft(Node current, int[][] data) {
		current.leftChild = new Node(data);
		current.leftChild.parent = current;
		current.leftChild.fromParent = 'L';
	}

	public void addRight(Node current, int[][] data) {
		current.rightChild = new Node(data);
		current.rightChild.parent = current;
		current.rightChild.fromParent = 'R';
	}

}
