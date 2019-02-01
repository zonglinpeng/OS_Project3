/**
Author: Peng
*/

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JOptionPane;


//import Model;
//import Puzzle;
//import eight.view.PuzzleApp;
//import eight.view.PuzzleView;

public class PuzzleMouseController extends MouseAdapter {

	SlidingPuzzleApp app;
	Model model;
	int x;
	int y;
	
	public PuzzleMouseController(SlidingPuzzleApp app, Model m) {
		this.app = app;
		this.model = m;
	}
	
	@Override
	public void mouseClicked(MouseEvent e) {
		x = e.getY();
		y = e.getX();
		selectTile(x, y);
	}

	public void selectTile(int x, int y) {
		
		Puzzle p = model.getPuzzle();
		app.getPuzzleView().requestFocus();
		
		if (p.isWin()) { 
			JOptionPane.showMessageDialog(app, 
					"You Win! Congrats!", 
					"SYSTEM MESSAGE", 
					JOptionPane.INFORMATION_MESSAGE, null);
			return; 
			}  
		
		PuzzleView view = app.getPuzzleView();

		
		int r = view.getMRow(x);
		int c = view.getMColumn(y);
//		System.out.printf("M Row: %d", r);
//		System.out.printf("M Col: %d", c);

		
		if (r < 0 || c < 0) { return; }  // nothing to do

		if(p.select(r, c)) {
			view.repaint();

			/*console helper*/
//			System.out.printf("\n");
//			for (int i=0; i<=4; i++) {
//				for (int j=0; j<=3; j++) {
//					if(p.tiles[i][j]!=null && p.tiles[i][j].isSelected) {
//						System.out.printf("0");
//					}else if (p.tiles[i][j]!=null && !p.tiles[i][j].isSelected){
//						System.out.printf("X");
//					}else if (p.tiles[i][j]==null ) {
//						System.out.printf("-");
//					}
//				}
//				System.out.printf("\n");
//			}
			
			return;
		}
		else return;
	}

}	

