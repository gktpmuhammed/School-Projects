package CargoTrain;

import java.util.Stack;

import Util.Cargo;

public class Carriage {
	
	public int emptySlot;
	public Stack<Cargo> cargos = new Stack<>();
	public Carriage next;
	public Carriage prev;
	
	public Carriage(int capacity) {
		this.emptySlot = capacity;
		
	}
	
	public boolean isFull() {
		if(emptySlot==0) {
			return true;
		}
		else 
			return false;
	}
	
	public void push(Cargo cargo) {
		cargos.push(cargo);
		emptySlot -= cargo.size;
	}
	
	public Cargo pop() {
		Cargo removal = cargos.pop();
		emptySlot += removal.size;
		return removal;
	}

}