import junit.framework.TestCase;

public class KeyTest extends TestCase {

	public void testSelectTest() { 
		Model m = new Model();
		Puzzle p = m.getPuzzle();
		Tile[][] t = p.tiles;
		
		SlidingPuzzleApp app = new SlidingPuzzleApp(m);
		app.setVisible(true);

		PuzzleView view = new PuzzleView(m.getPuzzle());

		
		PuzzleKeyController control = new PuzzleKeyController(app, m);

		//move trapped tile
		assertTrue(p.select(2, 1));
		assertFalse(control.moveTile('s'));
		assertTrue(p.select(2, 1));
		assertFalse(control.moveTile('w'));
		assertTrue(p.select(2, 1));
		assertFalse(control.moveTile('a'));
		assertTrue(p.select(2, 1));
		assertFalse(control.moveTile('d'));

		//move the moveable and test its post-move property
		assertTrue(p.select(4, 1));
		assertTrue(control.moveTile('a'));
		assertTrue(t[4][0]!=null);
		assertTrue(t[4][0].width == 2);
		assertTrue(t[4][1].heigth == 1);
		assertTrue(t[4][0].column == 0);
		assertTrue(t[4][1].row == 4);
		
		//reset tile board
		Tile ht = t[0][1];
		p.clear();
		//put the 2X2 and the up-right corner and try move it
		p.setTile(ht, 1, 0);
		assertTrue(p.select(1, 1));
		assertTrue(control.moveTile('w'));
		assertTrue(p.select(0, 1));
		assertFalse(control.moveTile('w'));
		assertTrue(p.select(0, 1));
		assertFalse(control.moveTile('a'));
		assertTrue(p.select(0, 1));
		assertTrue(control.moveTile('d'));
		p.setTile(ht, 0, 2);
		assertTrue(p.select(1, 3));
		assertFalse(control.moveTile('d'));
		assertTrue(p.select(1, 2));
		assertTrue(control.moveTile('s'));
		p.setTile(ht, 3, 2);
		assertTrue(p.select(4, 3));
		assertFalse(control.moveTile('s'));

		//make the puzzle win
		p.setTile(ht, 3, 1);
		assertTrue(p.select(4, 1));
		assertFalse(control.moveTile('s'));
		
	}

}
