import java.awt.event.MouseEvent;
//import java.util.concurrent.TimeUnit;

import junit.framework.TestCase;

public class MouseTest extends TestCase {

	public void testSelectTest() { 

		//create elements
		Model m = new Model();
		Puzzle p = m.getPuzzle();
		Tile[][] t = p.tiles;

		SlidingPuzzleApp app = new SlidingPuzzleApp(m);
//		app.setVisible(true);

		PuzzleView view = new PuzzleView(m.getPuzzle());

		//create a event
		PuzzleMouseController control = new PuzzleMouseController(app, m);
		MouseEvent event = new MouseEvent(view, MouseEvent.MOUSE_PRESSED, 
				System.currentTimeMillis(), 0, 100, 100, 0, false);

		//check event info
		control.mouseClicked(event);	
		assertTrue(control.x == 100);
		assertTrue(control.y == 100);
		
		//use mouse to select
		control.selectTile(120, 100);
		assertTrue(t[0][0].isSelected);
		assertTrue(t[1][0].isSelected);
		
		//test getMColumn
		assertTrue(view.getMColumn(50)==0);
		assertTrue(view.getMColumn(150)==1);
		assertTrue(view.getMColumn(250)==2);
		assertTrue(view.getMColumn(350)==3);
		assertTrue(view.getMColumn(500)==-1);

		
		//test getMRow
		assertTrue(view.getMRow(50)==0);
		assertTrue(view.getMRow(150)==1);
		assertTrue(view.getMRow(250)==2);
		assertTrue(view.getMRow(350)==3);
		assertTrue(view.getMRow(450)==4);
		assertTrue(view.getMRow(500)==-1);

		//select the 2x2 tile
		assertTrue(t[1][1].isHighlighted);

		//select non-selectables
		assertFalse(p.select(4, 0));
		assertFalse(p.select(1, 1));

			
		//check win state
		Tile ht = t[0][1];
		p.clear();
		p.setTile(ht, 3, 1);
		assertTrue(p.isWin());

	}


}
