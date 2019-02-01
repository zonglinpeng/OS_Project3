/**
Author: Peng
*/
public class Tile {

	int row; //index 0
	int column; //index 0
	int width; //index 1
	int heigth; //index 1
	
	boolean isSelected;
	boolean isHighlighted;
	
	public Tile (int r, int c, int w, int h) {
		this.row = r;
		this.column = c;
		this.width =w;
		this.heigth = h;
		this.isSelected = false;
		
		if((h+w) == 4) isHighlighted= true;
		else isHighlighted = false;
	}
	
	public void select () {
		this.isSelected = true;
	}	
	
	public void deselect () {
		this.isSelected = false;
	}
}
