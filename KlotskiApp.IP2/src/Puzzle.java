/**
Author: Peng
*/
public class Puzzle {

	Tile[][] tiles;//new Tile[5][4];


	private int moveCnt;

	public Puzzle() {
		tiles = new Tile[5][4];
		reset();
	}
	void clear() {
		for (int r = 0; r <= 4; r++) {
			for (int c = 0; c <= 3; c++) {
				this.tiles[r][c] = null;
			}
		}
		moveCnt = 0;

	}
	
	void reset() {
		this.clear();
		
		setTile(new Tile(0,0,2,2), 0,1); //tile, index row, index column
		setTile(new Tile(0,0,1,2), 0,0);
		setTile(new Tile(0,0,1,2), 0,3);
		setTile(new Tile(0,0,1,2), 2,3);
		setTile(new Tile(0,0,1,2), 2,0);
		setTile(new Tile(0,0,1,1), 2,1);
		setTile(new Tile(0,0,1,1), 3,1);
		setTile(new Tile(0,0,1,1), 2,2);
		setTile(new Tile(0,0,1,1), 3,2);
		setTile(new Tile(0,0,2,1), 4,1);
		//System.out.printf("setTile SUCCESS\n");
	}

	void setTile(Tile t, int r, int c) {

		int w = t.width - 1;
		int h = t.heigth - 1;

		do{
			int hTemp = h;
			do{
				if (tiles[r + hTemp][c + w] == null) {
					t.column = c;
					t.row = r;
					tiles[r + hTemp][c + w] = t; //!!test it
				}
				hTemp--;
			}while (hTemp >= 0) ;
			w--;
		}while (w >= 0) ;
	}

	boolean select(int r, int c) {
		if (r > 4 || c > 3) return false;

		//check any previously selected
		for (int i = 0; i <= 4; i++) {
			for (int j = 0; j <= 3; j++) {
				if (tiles[i][j]!= null && tiles[i][j].isSelected) return false;
			}		
		}
		
		Tile t = tiles[r][c];

		if(t == null) return false;
		
		else {
			int rInd = t.row;
			int cind = t.column;
			int w = t.width - 1;
			int h = t.heigth - 1;

			do{
				int hTemp = h;
				do{
					if (tiles[rInd + hTemp][cind + w]!=null)
					tiles[rInd + hTemp][cind + w].select();
					hTemp--;
				}while (hTemp >= 0) ;
				w--;
			}while (w >= 0) ;

			return true;
		}
	}

	boolean deSelect(int r, int c) {
		if (r > 4 || c > 3) return false;

		Tile t = tiles[r][c];

		if(t == null) return false;
		
		else {
			int rInd = t.row;
			int cind = t.column;
			int w = t.width - 1;
			int h = t.heigth - 1;

			do{
				int hTemp = h;
				do{
					if(tiles[rInd + hTemp][cind + w]!=null)
					tiles[rInd + hTemp][cind + w].deselect();

					hTemp--;
				}while (hTemp >= 0) ;
				w--;
			}while (w >= 0) ;

			return true;
		}
	}

	boolean moveSelected (char dir) {

		Tile t = null;
		int w = 0;
		int h = 0;
		int r = 0;
		int c = 0;

		//find the selected tiles
		for (int i=0; i<=4; i++) {
			for (int j=0; j<=3; j++) {
				if(tiles[i][j] != null && tiles[i][j].isSelected) {
					t = tiles[i][j]; //found the index tile
					break;
				}
			}
		}

		if (t == null) return false; //not found

		else {
			r = t.row; //index r
			c = t.column; //index c
			w = t.width - 1;
			h = t.heigth - 1;

//			System.out.printf("\n move: Found Tile: (%d, %d)",r,c);

			//test if move is valid and make move
			switch(dir) {
			//^^^^^^^
			case 'w':
				if(isValid(t, dir)) {
					int wTemp = w;
					do{
//						int hTemp = h;
//						do{
							//move the tile
							t.row = r-1;
							t.column = c;
							t.deselect();
							tiles[r+h][c+wTemp] = null; //set org tile
							tiles[r-1][c+wTemp] = t; //set des tile	
//							System.out.printf("\n move: Found Tile: (%d, %d)",r,c);

//							hTemp--;
//						}while (hTemp >= 0) ;
						wTemp--;
					}while(wTemp >= 0);

					//increment move count
					moveCnt++;
					return true;//move SUCCESS
				}else {
					this.deSelect(r, c);
					return false; //hit boundary
				}

				//<<<<<<<
			case 'a':
				if(isValid(t, dir)) {
//					do{
						int hTemp = h;
						do{
							//move the tile
							t.row = r;
							t.column = c-1;
							t.deselect();
							tiles[r+hTemp][c+w] = null; //set org tile
							tiles[r+hTemp][c-1] = t; //set des tile	

							hTemp--;
						}while (hTemp >= 0) ;
//						w--;
//					}while(w >= 0);

					//increment move count
					moveCnt++;
					return true;//move SUCCESS
				}else {
					this.deSelect(r, c);
					return false; //hit boundary
				}


				//VVVVVVV
			case 's':
				if(isValid(t, dir)) {
					int wTemp = w;
					do{
//						int hTemp = h;
//						do{
							//move the tile
							t.row = r+1;
							t.column = c;
							t.deselect();
							tiles[r][c+wTemp] = null; //set org tile
							tiles[r+h+1][c+wTemp] = t; //set des tile

//							hTemp--;
//						}while (hTemp >= 0) ;
							wTemp--;
					}while(wTemp >= 0);

					//increment move count
					moveCnt++;
					return true;//move SUCCESS
				}else {
					this.deSelect(r, c);
					return false; //hit boundary
				}
				
				//>>>>>>>
			case 'd':
				if(isValid(t, dir)) {
//					do{
						int hTemp = h;
						do{
							//move the tile
							t.row = r;
							t.column = c+1;
							t.deselect();
							tiles[r+hTemp][c] = null; //set org tile
							tiles[r+hTemp][c+1+w] = t; //set des tile	

							hTemp--;
						}while (hTemp >= 0) ;
//						w--;
//					}while(w >= 0);

					//increment move count
					moveCnt++;
					return true;//move SUCCESS
				}else {
					this.deSelect(r, c);
					return false; //hit boundary
				}



			default: break; //not a valid move command
			}
		}
		return false;
	}

	boolean isValid (Tile t, char dir) {

		boolean validFlag = true;
		int r = t.row; //index r
		int c = t.column; //index c
		int w = t.width - 1;
		int h = t.heigth - 1;

//		System.out.printf("\n isValid: Found Tile: (%d, %d)",r,c);

		//test if move is valid and make move
		switch(dir) {
		//^^^^^^^
		case 'w':
			if (r-1 >= 0) {// not on border
//				System.out.printf("\n index: (%d, %d)",r,c);
				do{
					if (tiles[r-1][c+w] != null) {
						validFlag = false;
						}
					w--;
				}while(w >= 0);

				if(validFlag) {}

				return validFlag;
			}else return false; //hit boundary

			//<<<<<<<
		case 'a':
			if (c-1 >= 0) {// not on border
				int hTemp = h;
				do{
					if (tiles[r+hTemp][c-1] != null) {
						validFlag = false;
					}
					hTemp--;
				}while (hTemp >= 0) ;

				return validFlag;//move SUCCESS
			}else return false; //hit boundary

			//VVVVVVV
		case 's':
			if (r+h+1 <= 4) {
				do{
					if (tiles[r+h+1][c+w] != null) { 
						validFlag = false;
					}
					w--;
				}while(w >= 0);

				return validFlag;
			}else return false;//hit boundary

			//>>>>>>>
		case 'd':
			if (c+w+1 <= 3) {// not on border
				int hTemp = h;
				do{
					if (tiles[r+hTemp][c+w+1] != null) {
						validFlag = false;
					}
					hTemp--;
				}while (hTemp >= 0) ;
				return validFlag;
			}else return false; //hit boundary

		default: break; //not a valid move command
		}
		return false;
	}

	boolean isWin() {
		//search for the highlight tile
		for (int i=0; i<=4; i++) { //colum
			for (int j=0; j<=3; j++) {
				if(tiles[i][j]!=null&&tiles[i][j].isHighlighted) {

					//check position
					if (i==3 && j==1 ) {
						System.out.println("YOU WIN!!!");
						
						return true;
						}
					else return false;
				}
			}
		}
		return false;
	}

	public int moveCount() {
		return moveCnt;
	}
}
