package CargoTrain;

import java.util.Queue;

import Util.Cargo;

public class Train {

	public int carCapacity;
	public int length;
	public Carriage head;
	public Carriage tail;
	public Carriage current;

	public Train(int length, int carCapacity) {
		this.length = length;
		this.carCapacity = carCapacity;
		this.head = new Carriage(carCapacity);
		this.tail = head;
		head.prev = null;
		tail.next = null;
		current = head;

		for (int i = 1; i < length; i++) {
			current.next = new Carriage(carCapacity);
			tail = current.next;
			tail.prev = current;
			tail.next = null;
			current = current.next;
		}
	}

	public void load(Queue<Cargo> cargos) {
		int size = cargos.size();
		for (int i = 0; i < size; i++) {
			current = head;
			Cargo now = cargos.poll();
			while (current != null) {
				if (current.emptySlot >= now.size) {
					current.push(now);
					break;
				}
				current = current.next;
			}
			if (current == null) {
				add(tail);
				tail.push(now);
			}
		}
		current = head;
		while (current != null) {

			if (current.emptySlot == carCapacity) {
				remove(current);
			}
			current = current.next;
		}
	}

	public void add(Carriage n) {
		if(tail == null) {
			head = tail = new Carriage(carCapacity);
		}
		else {
			tail.next = new Carriage(carCapacity);
			Carriage current1 = tail;
			tail = current1.next;
			tail.next = null;
			tail.prev = current1;
		}
		length++;

	}

	public void remove(Carriage n) {
		if (n.prev == null) {
			head = tail = null;
			length--;
		} else if (n.next == null) {
			tail = n.prev;
			tail.next = null;
			length--;
		} else {
			Carriage a = n.prev;
			Carriage b = n.next;
			a.next = b;
			b.prev = a;
			length--;
		}

	}

	public void unload(Queue<Cargo> cargos) {
		current = head;
		while (current != null) {
			while (true) {
				if (current.emptySlot == carCapacity) {
					break;
				}
				cargos.add(current.pop());
			}
			current = current.next;
		}
	}
}