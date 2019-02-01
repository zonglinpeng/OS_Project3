/**
Author: Peng
*/

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JOptionPane;

public class PuzzleKeyController extends KeyAdapter{
	SlidingPuzzleApp app;
	Model model;
	boolean isWin = false;
	
	public PuzzleKeyController(SlidingPuzzleApp app, Model m) {
		this.app = app;
		this.model = m;
	}
	
	@Override
	public void keyPressed(KeyEvent evt) {
//		System.out.printf("Key: %s", evt.getKeyChar());
		
		if (moveTile(evt.getKeyChar()) && isWin) { 
			return; 
			}   

	}

	public boolean moveTile (char dirt) {
		PuzzleView view = app.getPuzzleView();
		
		Puzzle puzzle = model.getPuzzle();
		
		if (puzzle.isWin()) { 
			isWin = true;
			return false;
			}
		
		if (puzzle.moveSelected(dirt)){
			app.label.setText(Integer.toString(puzzle.moveCount()));
			view.repaint();
			return true;
		}
		else return false;
	}
}
