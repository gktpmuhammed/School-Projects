package Util;

import java.io.File;
import java.io.PrintStream;
import java.util.LinkedList;
import java.util.Queue;

import CargoTrain.Train;

public class Station {

	public int id;
	public Queue<Cargo> cargoQueue = new LinkedList<Cargo>();
	PrintStream printStream;
	Station[] stations;

	public Station(int id, PrintStream printStream) {
		this.id = id;
		this.printStream = printStream;
	}

	public void process(Train train) {
		train.unload(cargoQueue);
		Queue<Cargo> newLoad = new LinkedList<Cargo>();
		while (cargoQueue.size() > 0) {
			Cargo a = cargoQueue.poll();
			if (a.targetStation.id != id) {
				newLoad.add(a);
			} else {
				printStream.println(a.toString());
			}
		}
		train.load(newLoad);
		printStream.println(this.id + " " + train.length);
	}

}