/**
Author: Peng
*/
public class Main {
	public static void main (String args[]) {
		Model m = new Model();
		Puzzle p = m.getPuzzle();
		SlidingPuzzleApp app = new SlidingPuzzleApp(m);
		app.setVisible(true);
		
 /*console helper*/
//		System.out.printf("\n");
//		for (int i=0; i<=4; i++) {
//			for (int j=0; j<=3; j++) {
//				if(p.tiles[i][j]!=null && p.tiles[i][j].isSelected) {
//					System.out.printf("0");
//				}else if (p.tiles[i][j]!=null && !p.tiles[i][j].isSelected){
//					System.out.printf("X");
//				}else if (p.tiles[i][j]==null ) {
//					System.out.printf("-");
//				}
//			}
//			System.out.printf("\n");
//		}
		
	}
}
