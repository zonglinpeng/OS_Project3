import junit.framework.TestCase;

public class ButtonTest extends TestCase {

	public void testButtonTest() {
		Model m = new Model();
		Puzzle p = m.getPuzzle();
		Tile[][] t = p.tiles;
		
		SlidingPuzzleApp app = new SlidingPuzzleApp(m);
		app.setVisible(true);

		PuzzleView view = new PuzzleView(m.getPuzzle());

		
		PuzzleButtonController control = 
				new PuzzleButtonController(app, m);
		
		control.resetTile();
		assertTrue(t[4][1].row == 4 && t[4][1].column ==1);
	}
}
