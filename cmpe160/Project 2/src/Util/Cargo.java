package Util;

import Util.Station;

public class Cargo {
	
	public int id;
	public Station loadingStation;
	public int size;
	public Station targetStation;
	
	public Cargo(int id, Station loadingStation, int size, Station targetStation) {
		this.id = id;
		this.loadingStation = loadingStation;
		this.size = size;
		this.targetStation = targetStation;
		
	}
	
	@Override
	public String toString() {
		 
		return id + " " + loadingStation.id + " " + targetStation.id + " " + size;
		
	}
	
}