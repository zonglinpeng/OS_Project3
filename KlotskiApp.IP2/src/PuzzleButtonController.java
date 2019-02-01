/**
Author: Peng
*/

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class PuzzleButtonController implements ActionListener{

	SlidingPuzzleApp app;
	Model model;

	public PuzzleButtonController(SlidingPuzzleApp app, Model m) {
		this.app = app;
		this.model = m;
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		resetTile();
	}

	public void resetTile() {
		Puzzle puzzle = model.getPuzzle();

		puzzle.reset();
		app.label.setText("0");
		app.repaint();
	}

}
