/**
Author: Peng
*/

import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JPanel;

//import eight.model.Puzzle;

/** 
 * Knows how to visually present the puzzle. 
 */
public class PuzzleView extends JPanel {

	Puzzle puzzle;
	private Tile [][] tiles;
	int offset = 2;  // buffer between squares
	
	int boxHeight = 100;      // computed once constructed
	int boxWidth = 106;
	
	public PuzzleView(Puzzle puzzle) {
		this.boxHeight = 100; //getWidth() / 4;
		this.boxWidth = 106; //getHeight() / 5;
		this.puzzle = puzzle;
		this.tiles = puzzle.tiles;
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
				
		boxWidth = getWidth() / 4;
		boxHeight = getHeight() / 5;

		for (int r = 0; r <= 4; r++) {
			for (int c = 0; c <= 3; c++) {
				Tile t = this.tiles[r][c];

				if (t!= null) {
					int rIdx = t.row;
					int cIdx = t.column;
					int w = t.width - 1;
					int h = t.heigth - 1;
					if (rIdx==r && cIdx==c) {
						if (t.isSelected) { //toppest color layer 
							g.setColor(Color.blue);
						}
						else if (t.isHighlighted) {
							g.setColor(Color.red);
						}
						else {
							g.setColor(Color.gray);
						}
						g.fillRect(offset + c*boxWidth, offset + r*boxHeight, 
								boxWidth*(w+1) - 2*offset, boxHeight*(h+1) - 2*offset);

					}
				}
			}
		}

	}
	
	//get row/height
	public int getMRow(int y) {
		if ((y >= offset) && (y <= boxHeight - offset)) { return 0; }
		if ((y >= boxHeight + offset) && (y <= 2*boxHeight - offset)) { return 1; }
		if ((y >= 2*boxHeight + offset) && (y <= 3*boxHeight - offset)) { return 2; }
		if ((y >= 3*boxHeight + offset) && (y <= 4*boxHeight - offset)) { return 3; }
		if ((y >= 4*boxHeight + offset) && (y <= 5*boxHeight - offset)) { return 4; }
		return -1;
	}

	//get col/width
	public int getMColumn(int x) {
		if ((x >= offset && x <= boxWidth - offset)) { return 0; }
		if ((x >= boxWidth + offset) && (x <= 2*boxWidth - offset)) { return 1; }
		if ((x >= 2*boxWidth + offset) && (x <= 3*boxWidth - offset)) { return 2; }
		if ((x >= 3*boxWidth + offset) && (x <= 4*boxWidth - offset)) { return 3; }
		return -1;
	}

}
