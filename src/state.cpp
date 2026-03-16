
#include "state.hpp"
#include "sfmlApp.hpp"



State* State::instance_ = nullptr;


void State::onCreate ()
{
	instance_ = this;
	app->setRedrawColor(GRASSGREEN);
	rwin->setMouseCursorVisible(false);
	
	instrucsTxt = Text(instrucsStr, gFont("mazeSize"), 18);
	instrucsTxt.setFillColor(DKAZURE);
	
	sizeTxt = Text("", gFont("mazeSize"), 24);
	sizeTxt.setFillColor(DKAZURE);

	tbox = Textbox(gFont("debug"), {1500, 25});
	
	gSound("move").setVolume(5.f);
	gSound("hitWall").setVolume(15.f);

	gridSize = {14, 8};

#if 0
	/* If using the dirtfill, lighten the color first */
	ZImage zim {gTexture("dirtBkgd").copyToImage()};
	zim.prportLighten(50);
	gTexture("dirtBkgd").loadFromImage(zim);
	bkgdSpr.setTexture(gTexture("dirtBkgd"));
#endif
	bkgdSpr.setTexture(gTexture("bkgd"));
//	bkgdSpr.setColor(CHARCOAL);
	bkgdSpr.setColor(GRASSGREEN);

	pcSpr.setTexture(gTexture("pc"));
	centerOrigin(pcSpr);
	
	/////// TEMP TESTING Animations
	sheep = make_shared<AnimatableSprite>();
	sheep->spr.setTexture(gTexture("pc"));
	centerOrigin(sheep->spr);
	auto an = make_shared<XlatAnimation>();
	an->setDuration(.15);
	an->easePattern = std::move(make_shared<SineEaseInOut>(.15, 2));
	an->myObj = sheep;
	an->tag = "moveSheep";
	animMgr.animations.push_back(std::move(an));
	
	goalSpr.setTexture(gTexture("goal"));
	centerOrigin(goalSpr);

	adjustResourceColors();
	
	createCellTxs();
//	createCellTxs2();
	
	reset();
}

/* Currently unused */
bool State::handleTextEvent (Event& event)
{
	if (activeTbox
		&& (event.type == Event::TextEntered
			|| event.type == Event::KeyPressed
			|| event.type == Event::KeyReleased)) {
		if (event.type == Event::TextEntered) {
			if (event.text.unicode == 8)
				if (isShiftPressed())
					activeTbox->clear();
				else activeTbox->deleteLastChar();
				else if (event.text.unicode == 9) ; // Don't write the \t
				else activeTbox->appendText(event.text.unicode);
		}
		if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Enter)) {
			activeTbox->setActive(false);
			activeTbox = nullptr;
		}
		return true;
	}
	return false;
}

void State::onMouseDown (int x, int y)
{
	if (tbox.tbox.gGB().contains(x, y)) {
		tbox.setActive(true);
		activeTbox = &tbox;
		return;
	}
	else if (tbox.isActive) {
		tbox.setActive(false);
		activeTbox = nullptr;
		return;
	}
}

void State::onKeyPress(Keyboard::Key k)
{
	switch(k) {
			
		case Keyboard::Escape:
			app->close();
			break;
			
		case Keyboard::Up:
		case Keyboard::Down:
		case Keyboard::Left:
		case Keyboard::Right:
			movePC(k);
			break;
			
		case Keyboard::Y:
			reset();
			break;
			
		default:
			break;
	}
}

void State::update (const Time& time)
{
	timedMgr->fireReadyEvents(time);
	animMgr.update(time);
	
	adjustVal(A, gridSize.x, 2, 2, 1000);
	adjustVal(Z, gridSize.y, 2, 2, 1000);
	
	sizeTxt.setString(vecfStr(toVecF(gridSize)));
} //end update

void State::draw ()
{
	rwin->draw(bkgdSpr);
	//	rwin->draw(tbox);
		
	forNum (curMaze.grid.size()) {
		if (curMaze.goalCell.y == i)
			rwin->draw(goalSpr);
		if (pcLoc.y == i
			&& sheep->spr.getScale().x < 2)
//			&& pcSpr.getScale().x < 2)
			rwin->draw(sheep->spr);
//			rwin->draw(pcSpr);
		rwin->draw(rtSprVec[i]);
	}
	if (pcSpr.getScale().x > 1.9)
		rwin->draw(pcSpr);
	
#if 0 // using walls instead of fences
	rwin->draw(rtSpr);
	rwin->draw(pcSpr);
	
	// using plain lines instead of sprites
	rwin->draw(va);
#endif
	
	
	rwin->draw(instrucsTxt);
	rwin->draw(sizeTxt);
}

void State::reset ()
{
	activeTbox = nullptr;
	
	curMaze = generateNewMaze();
	assembleMazeSprite(curMaze);
//	assembleMazeSprite2(curMaze);
	
	goalSpr.setPosition(toVecF(cellCtrToPixels(curMaze.goalCell)));
	float factor = curMaze.cellSize / (goalSpr.gLB().width + 10);
	goalSpr.setScale(factor, factor);
	
#if 0 // using walls instead of fences
	Sprite goalSpr_(gTexture("goal"));
	centerOrigin(goalSpr_);
	goalSpr_.setPosition(toVecF(cellCtrToPixels(curMaze.goalCell)));
	goalSpr_.setScale(factor, factor);
	rt.draw(goalSpr_);

	rt.display();
	rtSpr = Sprite(rt.getTexture());
#endif
	
	/* To simplify all of the positioning and maze coordinates,
	 * leave the maze oriented to 0, 0 and shift the window view
	 * to center the maze, rather than tryng to move the maze
	 * to the center.
	 */
	vw = View({curMaze.spriteSize / 2.f}, {scrw, scrh});
	rwin->setView(vw);
	
	bkgdSpr.setScale({scrw/ bkgdSpr.gLB().width, scrh / bkgdSpr.gLB().height});
	centerOrigin(bkgdSpr);
	/* Background needs to follow the View */
	bkgdSpr.setPosition(vw.getCenter());
	
	/* Text needs to be relative to View, not to maze */
	sizeTxt.setPosition(vw.getCenter() - vw.getSize() / 2.f + vecF(8, 25));
	instrucsTxt.setPosition(vw.getCenter() - vw.getSize() / 2.f + vecF(15, scrh - 30));
	
	pcLoc = curMaze.startCell;
	sheep->spr.setPosition(toVecF(cellCtrToPixels(pcLoc)));
//	pcSpr.setPosition(toVecF(cellCtrToPixels(pcLoc)));
	sheep->spr.setScale(factor, factor);
//	pcSpr.setScale(factor, factor);
}

Maze State::generateNewMaze ()
{
	/* The maza data will be represented as a 2D array of
	 * unsigned chars, where the bits of each unsigned char stand for
	 * the presence or absence of walls on the four sides of that
	 * cell.
	 */
	Maze maze;
	/* `gridSize` represents the number of traversable maze cells, but
	 * here we add 2 to the values to make a border
	 */
	BitsetGrid grid = {(size_t)gridSize.y + 2, vector<unsigned char>(gridSize.x + 2, 15)};
	
	/* Set all border cells to "visited" so that we don't have to run special
	 * logic in the algorithm to keep from wandering out of bounds
	 */
	for (size_t row = 1; row < grid.size() - 1; ++row) {
		for (size_t col = 1; col < grid[row].size() - 1; ++col) {
			grid[row][col] |= 16;
		}
	}
	vecI curCell;
	
	/* For now, let starting point be a random corner and goal be
	 * the opposite corner. We want the second tier "in" because
	 * we essentially added a dummy outermost tier for bounding walls.
	 */
	vector<vecI> gridCorners {	// NW, NE, SE, SW
		{1, 1}
		, {gridSize.x, 1}
		, {gridSize.x, gridSize.y}
		, {1, gridSize.y}
	};
	auto idx = randRange(3);
	maze.startCell = gridCorners[idx];
	/* The expression here gets the opposite corner */
	maze.goalCell = gridCorners[(idx + 2) % 4];
	
	
	queue<vecI> que;
	curCell = maze.startCell;
	/* Mark this cell as visited */
	grid[curCell.y][curCell.x] &= ~16;
	que.push(curCell);
	for (;;) {
		vecI nextCell {-1, -1};
		int curIdx = randRange(3);
		forNum(4) {
			vecI tempNext = curCell + dirCoords.at(dirStr[curIdx]);
			/* We found an unresolved cell neighboring our current one */
			if (grid[tempNext.y][tempNext.x] & 16) {
				nextCell = tempNext;
				break;
			}
			++curIdx;
		}
		/* Everything surrounding curCell is already resolved, so this
		 * branch has reached a dead end and we need to go back to the
		 * queue to see if there are any unresolved branches left to
		 * pursue
		 */
		if (nextCell.x == -1) {
			que.pop();
			if (!que.empty())
				curCell = que.front();
			/* Reaching this statement means whole grid has been resolved */
			else break;
		}
		/* We just moved to a new unresolved cell in the currently active branch */
		else {
			/* curCell and nextCell: each has to remove its wall that separates
			 * it from the other
			 */
			grid[curCell.y][curCell.x] &= ~(1 << (curIdx % 4));
			grid[nextCell.y][nextCell.x] &= ~(1 << ((curIdx + 2) % 4));
			/* Mark latest cell as visited and add it to the queue */
			curCell = nextCell;
			grid[curCell.y][curCell.x] &= ~16;
			que.push(curCell);
		}
	}
	
	maze.grid = grid;
	/* Determine the pixel size of each cell of this maze, taking into
	 * consideration leaving some open space around the screen edges.
	 * Because we want to do proportional resizing, and because the user
	 * can adjust the x and y sizes of the grid independently, we have to
	 * use min() to know which value determines the cell size
	 */
	maze.cellSize = min(scrw / (gridSize.x + 4), scrh / (gridSize.y + 4));
	return maze;
}

/* For fences/layering as opposed to bird's-eye solid walls */
void State::createCellTxs ()
{
	/* There are sixteen possible combinations of wall-or-no-wall
	 * for north, south, east, west. We'll draw a sprite for each,
	 * so that a later routine can read the map data and pick one
	 * of these sprites to draw at the position of that cell.
	 
	 * The positioning and coordinates used here are a little odd and
	 * unintuitive, but it's an attempt (not necessarily the most elegant
	 * one) to build the cell sprites in such a way that the finished
	 * maze can be drawn by the window row by row (from top down), inserting
	 * the PC sprite at the right juncture so that it appears to be behind
	 * the fences lower than it, but in front of the fences higher than it
	 */
	
	cellRt.create(48, 65);
	cellRt.create(48, 65);
	
	forNum(16) {
		if (i == 15) {
			cellTxs[i] = gTexture("border");
			continue;
		}
		cellRt.clear(Color::Transparent);
		
		Sprite spr1(gTexture("lpost"));
		spr1.setOrigin(0, spr1.gLB().height - 1);
		spr1.setPosition({0, 64});
		Sprite spr2(gTexture("rpost"));
		spr2.setOrigin(spr2.gLB().width - 1, spr2.gLB().height - 1);
		spr2.setPosition({47, 64});
		Sprite spr3(gTexture("lfence"));
		Sprite spr4(gTexture("rfence"));
		spr4.setOrigin(spr4.gLB().width - 1, 0);
		spr4.setPosition(47, 0);
		Sprite spr5(gTexture("fence"));
		spr5.setPosition(3, 44);
		
		cellRt.draw(spr1);
		cellRt.draw(spr2);
		if (i & 4)
			cellRt.draw(spr5);
		if (i & 2)
			cellRt.draw(spr4);
		if (i & 8)
			cellRt.draw(spr3);
		
		cellRt.display();
		cellTxs[i] = cellRt.getTexture();
	}
}

/* For creating the maze sprite out of solid walls that won't overlap the PC */
void State::createCellTxs2 ()
{
	cellRt.create(48, 48);
	
	forNum(16) {
		if (i == 15) {
			cellTxs[i] = gTexture("border");
			continue;
		}
		cellRt.clear(Color::Transparent);
		
		/* The large number of code statements here is the tradeoff for
		 * less pixel drawing and stored resource files
		 */
		Sprite spr(gTexture("osCorner"));
		cellRt.draw(spr);
		spr.setPosition({0, 47});
		spr.setScale({1, -1});
		cellRt.draw(spr);
		spr.setPosition({47, 47});
		spr.setScale({-1, -1});
		cellRt.draw(spr);
		spr.setPosition({47, 0});
		spr.setScale({-1, 1});
		cellRt.draw(spr);
		
		Sprite spr2(gTexture("wall"));
		if (i & 1) {
			spr2.setPosition({0, 0});
			spr2.setRotation(270);
			spr2.setScale({-1, 1});
			cellRt.draw(spr2);
			spr2.setPosition({47, 0});
			spr2.setScale({-1, -1});
			cellRt.draw(spr2);
		}
		if (i & 2) {
			spr2.setRotation(0);
			spr2.setPosition({47, 0});
			spr2.setScale({-1, 1});
			cellRt.draw(spr2);
			spr2.setPosition({47, 47});
			spr2.setScale({-1, -1});
			cellRt.draw(spr2);
		}
		if (i & 4) {
			spr2.setRotation(270);
			spr2.setPosition({0, 47});
			spr2.setScale({1, 1});
			cellRt.draw(spr2);
			spr2.setPosition({47, 47});
			spr2.setScale({1, -1});
			cellRt.draw(spr2);
		}
		if (i & 8) {
			spr2.setRotation(0);
			spr2.setPosition({0, 0});
			spr2.setScale({1, 1});
			cellRt.draw(spr2);
			spr2.setPosition({0, 47});
			spr2.setScale({1, -1});
			cellRt.draw(spr2);
		}
		
		Sprite spr3(gTexture("isCorner"));
		if (i & 1 && i & 8) {
			cellRt.draw(spr3);
		}
		if (i & 1 && i & 2) {
			spr3.setPosition({47, 0});
			spr3.setScale({-1, 1});
			cellRt.draw(spr3);
		}
		if (i & 2 && i & 4) {
			spr3.setPosition({47, 47});
			spr3.setScale({-1, -1});
			cellRt.draw(spr3);
		}
		if (i & 4 && i & 8) {
			spr3.setPosition({0, 47});
			spr3.setScale({1, -1});
			cellRt.draw(spr3);
		}
		
		cellRt.display();
		cellTxs[i] = cellRt.getTexture();
	}
}

/* For fences vs. solid walls: create sprites for each row of the
 * maze rather than assembling a monolithic sprite
 */
void State::assembleMazeSprite (Maze& curMaze)
{
	uint xsize = curMaze.cellSize * (uint)curMaze.grid[0].size();
	uint ysize = curMaze.cellSize * (uint)curMaze.grid.size();
	curMaze.spriteSize = toVecF(vecU(xsize, ysize));
	
	rtSprVec.clear();
	rtSprVec.shrink_to_fit();
	rtSprVec.reserve(curMaze.grid.size());
	rtTxVec.clear();
	rtTxVec.shrink_to_fit();
	rtTxVec.reserve(curMaze.grid.size());
	
	rt.create(xsize, ysize);
	float factor = float(curMaze.cellSize) / (cellTxs[0].getSize().x - 1); // Keep the - 1
	forNum (curMaze.grid.size()) {
		rt.clear(Color::Transparent);
		forNumJ (curMaze.grid[i].size()) {
			auto cur = curMaze.grid[i][j];
			Sprite spr(cellTxs[cur]);
			spr.setOrigin({24.f, cur == 15 ? 24.f : 41.f});
			spr.setScale({factor, factor});
			spr.setPosition(toVecF(cellCtrToPixels({j, i})));
			rt.draw(spr);
		}
		rt.display();
		rtTxVec.emplace_back(rt.getTexture());
		rtSprVec.emplace_back(rtTxVec.back());
		rtSprVec.back().setColor(Color(175, 167, 128));
	}
}

/* Build a single sprite to draw per frame, when using solid walls */
void State::assembleMazeSprite2 (Maze& curMaze)
{
	uint xsize = curMaze.cellSize * (uint)curMaze.grid[0].size();
	uint ysize = curMaze.cellSize * (uint)curMaze.grid.size();
	curMaze.spriteSize = toVecF(vecU(xsize, ysize));
	
	rt.create(xsize, ysize);
	rt.clear(Color::Transparent);
	Sprite spr(cellTxs[0]);
	float factor = float(curMaze.cellSize) / (spr.gLB().width - 1); // Keep the - 1
	spr.setScale({factor, factor});
	centerOrigin(spr);
	forNum (curMaze.grid.size()) {
		forNumJ (curMaze.grid[i].size()) {
			auto cur = curMaze.grid[i][j];
			spr.setTexture(cellTxs[cur]);
			spr.setPosition(toVecF(cellCtrToPixels({j, i})));
			rt.draw(spr);
		}
	}
	rt.display();
}

/* Draw the maze data using plain lines */
void State::loadVxArrFromMaze (Maze& curMaze)
{
	va.clear();
	int& csz = curMaze.cellSize;
	forNum (curMaze.grid.size()) {
		forNumJ (curMaze.grid[i].size()) {
			auto cur = curMaze.grid[i][j];
			if (cur & 1) {
				va.appendCoords(j * csz, i * csz);
				va.appendCoords(j * csz + csz, i * csz);
			}
			if (cur & 2) {
				va.appendCoords(j * csz + csz, i * csz);
				va.appendCoords(j * csz + csz, i * csz + csz);
			}
			if (cur & 4) {
				va.appendCoords(j * csz, i * csz + csz);
				va.appendCoords(j * csz + csz, i * csz + csz);
			}
			if (cur & 8) {
				va.appendCoords(j * csz, i * csz);
				va.appendCoords(j * csz, i * csz + csz - 1);
			}
		}
	}
}

unsigned char State::getCell (const vecI& vec)
{
	return curMaze.grid[vec.y][vec.x];
}

vecI State::cellCtrToPixels (const vecI& cell)
{
	return {cell.x * curMaze.cellSize + curMaze.cellSize / 2,
		cell.y * curMaze.cellSize + curMaze.cellSize / 2};
}

void State::movePC (Keyboard::Key k)
{
	char coordKey;
	int bitNum;
	/* The sf::Keyboard::Key codes are
	 * Left = 71
	 * Right = 72
	 * Up = 73
	 * Down = 74
	 * therefore the following block:
	 */
	auto num = (int)k - 71;
	if (num == 0) { // WEST
		coordKey = 'w';
		bitNum = 3;
	}
	else if (num == 1) { // EAST
		coordKey = 'e';
		bitNum = 1;
	}
	else if (num == 2) { // NORTH
		coordKey = 'n';
		bitNum = 0;
	}
	else if (num == 3) { // SOUTH
		coordKey = 's';
		bitNum = 2;
	}
	
	////// TEMP TESTING ANIM: eventually allow opp.direction switch
	///before anim completes, maybe even instantly finish an anim if
	///key is pushed to continue the direction (or queue the movement)
	///but make sure not to allow a turn to the north if a westward move
	///is in progress, unless queueing it
	auto idx = indexWhich(animMgr.animations, [](auto& x){ return x->tag == "moveSheep"; });
	if (idx >= 0 && animMgr.animations[idx]->isPlaying())
		return;	// for now just force wait for the movement to finish
	/////
	
	/* Make PC face the direction it's heading */
	auto curScale = sheep->spr.getScale();
//	auto curScale = pcSpr.getScale();
	if (coordKey == 'w')
		sheep->spr.setScale(abs(curScale.x), curScale.y);
//		pcSpr.setScale(abs(curScale.x), curScale.y);
	else if (coordKey == 'e')
		sheep->spr.setScale(abs(curScale.x) * -1, curScale.y);
//		pcSpr.setScale(abs(curScale.x) * -1, curScale.y);

	/* There's no wall in the attempted direction, so
	 * carry out a move
	 */
	if ( (getCell(pcLoc) & (1 << bitNum)) == 0) {
		pcLoc += dirCoords.at(coordKey);
		
		//// TESTING NEW ANIM
		auto dest = toVecF(cellCtrToPixels(pcLoc));
		auto ptr = dynamic_pointer_cast<XlatAnimation>(animMgr.animations[idx]);
		if (!ptr)
			;
		ptr->startPt = sheep->spr.getPosition();
		ptr->destPt = dest;
		ptr->play(animMgr.elapsed);
//		pcSpr.setPosition(toVecF(cellCtrToPixels(pcLoc)));
		//////
		
		if (pcLoc == curMaze.goalCell)
			winGame();
		else
			gSound("move").play();
	}
	/* Can't go that way */
	else {
		gSound("hitWall").play();
		//anim?
	}
}

void State::winGame ()
{
	pcSpr.setScale(2,2);
	gSound("win").play();
	//show path if not already drawn
	//anim
	/* Move on to a new maze of increased difficulty */
	gridSize += {2, 2};
	timedMgr->addEvent(3, [&](){ reset(); });
}

void State::adjustResourceColors ()
{
	uint hueVal = 45;
	Color newCol = CAPPUCCINO;
	ZImage zim1 {gTexture("isCorner").copyToImage()};
	int wid = zim1.getSize().x;
	int ht = zim1.getSize().y;
	for (int i = 0; i <  wid ; ++i) {
		for (int j = 0; j <  ht ; ++j) {
			Color p = zim1.getPixel(i, j);
			if (p.a > 0)
				//				zim1.setPixel(i, j, addHue(p, hueVal));
				zim1.setPixel(i, j, addBrightness(newCol, 8));
		}
	}
	gTexture("isCorner").loadFromImage(zim1);
	
	zim1 = gTexture("osCorner").copyToImage();
	wid = zim1.getSize().x;
	ht = zim1.getSize().y;
	for (int i = 0; i <  wid ; ++i) {
		for (int j = 0; j <  ht ; ++j) {
			Color p = zim1.getPixel(i, j);
			if (p.a > 0)
				//				zim1.setPixel(i, j, addHue(p, hueVal));
				zim1.setPixel(i, j, addBrightness(newCol, 7));
		}
	}
	gTexture("osCorner").loadFromImage(zim1);
	
	zim1 = gTexture("wall").copyToImage();
	wid = zim1.getSize().x;
	ht = zim1.getSize().y;
	for (int i = 0; i <  wid ; ++i) {
		for (int j = 0; j <  ht ; ++j) {
			Color p = zim1.getPixel(i, j);
			if (p.a > 0)
				//				zim1.setPixel(i, j, addHue(p, hueVal));
				zim1.setPixel(i, j, addBrightness(newCol, 6));
		}
	}
	gTexture("wall").loadFromImage(zim1);
}
