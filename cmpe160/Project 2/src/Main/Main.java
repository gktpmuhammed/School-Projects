package Main;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.Scanner;

import CargoTrain.Train;
import Util.Cargo;
import Util.Station;

public class Main {

	public static int numStations;
	public static Station[] station;
	public static Train a;

	public static void main(String[] args) throws FileNotFoundException {
		PrintStream printStream = new PrintStream(new File(args[1]));
		Scanner inputFile = new Scanner(new File(args[0]));
		readAndInitialize(printStream, inputFile);
		execute();
	}

	public static void readAndInitialize(PrintStream printStream, Scanner inputFile) throws FileNotFoundException {
		int length = inputFile.nextInt();
		int carCapacity = inputFile.nextInt();
		numStations = inputFile.nextInt();
		station = new Station[numStations];
		for (int i = 0; i < numStations; i++) {
			station[i] = new Station(i, printStream);
		}
		a = new Train(length, carCapacity);
		while (inputFile.hasNextLine()) {
			String line = inputFile.nextLine();
			Scanner lineScan = new Scanner(line);
			while (lineScan.hasNext()) {
				int id = lineScan.nextInt();
				int stationId = lineScan.nextInt();
				int targetStation = lineScan.nextInt();
				int cargoSize = lineScan.nextInt();
				if (cargoSize <= carCapacity) {
					if (targetStation >= stationId) {
						if (stationId >= 0) {
							station[stationId].cargoQueue
									.add(new Cargo(id, station[stationId], cargoSize, station[targetStation]));
						}
					}
				}
			}

		}

	}

	public static void execute() {

		for (int i = 0; i < numStations; i++) {
			station[i].process(a);
		}
	}
}