
#include "state.hpp"
#include "sfmlApp.hpp"



State* State::instance_ = nullptr;



void State::debugTxtSetup ()
{
	mouseTxt = Text("", Resources::getFont("debug"), 13);
	mouseTxt.sP(8, 9);
	mouseTxt.setFillColor(PURPLE);
		
	debugTxt = Text("", Resources::getFont("debug"), 13);
	debugTxt.sP(8, 25);
	debugTxt.setFillColor(Color::Blue);
}

bool State::handleTextEvent (Event& event)
{
	if (activeTbox
			&& (event.type == Event::TextEntered
				|| event.type == Event::KeyPressed
				|| event.type == Event::KeyReleased)) {
		if (event.type == Event::TextEntered) {
			if (event.text.unicode == 8)
				if (iKP(LShift))
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

void State::onCreate ()
{
	instance_ = this;
	
	debugTxtSetup();

	tbox = Textbox(Resources::getFont("debug"), {1500, 25});

	gridSize = {30, 20};
	
	ZImage zim {Resources::getTex("dirtBkgd").copyToImage()};
	zim.prportLighten(50);
	Resources::getTex("dirtBkgd").loadFromImage(zim);
	bkgdSpr.setTexture(Resources::getTex("dirtBkgd"));
	bkgdSpr.setScale({scrw / bkgdSpr.gLB().width, scrh / bkgdSpr.gLB().height});
	centerOrigin(bkgdSpr);
	bkgdSpr.setPosition({scrcx, scrcy});
		
	pcSpr.setTexture(Resources::getTex("pc"));
	centerOrigin(pcSpr);
	
	uint hueVal = 45;
	ZImage zim1 {Resources::getTex("isCorner").copyToImage()};
	int wid = zim1.getSize().x;
	int ht = zim1.getSize().y;
	for (int i = 0; i <  wid ; ++i) {
		for (int j = 0; j <  ht ; ++j) {
			Color p = zim1.getPixel(i, j);
			if (p.a > 0)
				zim1.setPixel(i, j, addHue(p, hueVal));
		}
	}
	Resources::getTex("isCorner").loadFromImage(zim1);
	
	zim1 = Resources::getTex("osCorner").copyToImage();
	wid = zim1.getSize().x;
	ht = zim1.getSize().y;
	for (int i = 0; i <  wid ; ++i) {
		for (int j = 0; j <  ht ; ++j) {
			Color p = zim1.getPixel(i, j);
			if (p.a > 0)
				zim1.setPixel(i, j, addHue(p, hueVal));
		}
	}
	Resources::getTex("osCorner").loadFromImage(zim1);
	
	zim1 = Resources::getTex("wall").copyToImage();
	wid = zim1.getSize().x;
	ht = zim1.getSize().y;
	for (int i = 0; i <  wid ; ++i) {
		for (int j = 0; j <  ht ; ++j) {
			Color p = zim1.getPixel(i, j);
			if (p.a > 0)
				zim1.setPixel(i, j, addHue(p, hueVal));
		}
	}
	Resources::getTex("wall").loadFromImage(zim1);
	
	createCellTxs();
	
	reset();
}

void State::createCellTxs ()
{
	cellRt.create(48, 48);
	
	forNum(16) {
		if (i == 15) {
			cellTxs[i] = Resources::getTex("border");
			continue;
		}
		cellRt.clear(Color::Transparent);
		
		Sprite spr(Resources::getTex("osCorner"));
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
		
		Sprite spr2(Resources::getTex("wall"));
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
		
		Sprite spr3(Resources::getTex("isCorner"));
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

void State::reset ()
{
	activeTbox = nullptr;
	
	curMaze = generateNewMaze();
	
	rt.clear(Color::Transparent);
//	rt.draw(va);
	assembleMazeSprite(curMaze);
	
	Sprite goalSpr(Resources::getTex("goal"));
	centerOrigin(goalSpr);
	goalSpr.setPosition(toVecF(cellCtrToPixels(curMaze.goalCell)));
	float factor = curMaze.cellSize / (goalSpr.gLB().width + 5);
	goalSpr.setScale(factor, factor);
	rt.draw(goalSpr);
	
	rt.display();
	rtSpr.setTexture(rt.getTexture());
	centerOrigin(rtSpr);
	rtSpr.setPosition({scrcx, scrcy});
	
	pcLoc = curMaze.startCell;
	pcSpr.setPosition(toVecF(cellCtrToPixels(pcLoc)));
	
}


void State::draw ()
{
	w->draw(bkgdSpr);
//	w->draw(tbox);
	
	w->draw(rtSpr);
	w->draw(pcSpr);
	
//	w->draw(mouseTxt);
	w->draw(debugTxt);
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
	
	// if (xxx.contains(x, y))
}


void State::onMouseUp (int x, int y)
{
	
}


void State::onKeyPress(Keyboard::Key k)
{
	switch(k) {
			
		case Keyboard::Escape:
			gw->close();
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


void State::onKeyRelease(Keyboard::Key k)
{
	switch(k) {
					
		default:
			break;
	}
}


void State::update (const Time& time)
{
	timedMgr->fireReadyEvents(time);
	
	adjustVal(A, gridSize.x, 2, 2, 1000);
	adjustVal(Z, gridSize.y, 2, 2, 1000);
	
	//sprite update

	// DEBUG/TESTING
	mouseTxt.setString(tS(mx) + ", " + tS(my));
	debugTxt.setString(vecfStr(toVecF(gridSize)));
	
} //end update

Maze State::generateNewMaze ()
{
	Maze maze;
	BitsetGrid grid = {(size_t)gridSize.y + 2, vector<unsigned char>(gridSize.x + 2, 15)};
	
	/* Set all border cells to "visited" so that we don't have to run special
	 * logic to keep from wandering out of bounds
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
	maze.goalCell = gridCorners[(idx + 2) % 4];
	/*
	 // QUEUE ONLY FOR getting the generating concept going: ultimately, use some sort of sorting step to start new branches from available remaining spots nearest the origin
	 
		priorityq/sort keeps track of cell's manhattan from origin, new branches always start from nearest option to origin, potentially creates longest wrong roads, most challenging?
		try letting a branch go # cells then force rebranch, rather than letting it go to dead end before
	 */
	queue<vecI> que;
	curCell = maze.startCell;
	grid[curCell.y][curCell.x] &= ~16;
	que.push(curCell);
	for (;;) {
		vecI nextCell {-1, -1};
		int curIdx = randRange(3);
		forNum(4) {
			vecI tempNext = curCell + dirCoords.at(dirStr[curIdx]);
			if (grid[tempNext.y][tempNext.x] & 16) {
//			auto cur = grid[tempNext.y][tempNext.x];
//			bool unvisited = cur & 16;
//			if (unvisited) {
				nextCell = tempNext;
				break;
			}
			++curIdx;
		}
		if (nextCell.x == -1) {
			que.pop();
			if (!que.empty())
				curCell = que.front();
			/* Reaching this statement means whole grid has been resolved */
			else break;
		}
		else {
			grid[curCell.y][curCell.x] &= ~(1 << (curIdx % 4));
			grid[nextCell.y][nextCell.x] &= ~(1 << ((curIdx + 2) % 4));
			curCell = nextCell;
			/* Mark as visited */
			grid[curCell.y][curCell.x] &= ~16;
			que.push(curCell);
		}
	}

	maze.grid = grid;
	maze.cellSize = min(scrw / (gridSize.x + 2), scrh / (gridSize.y + 2));
	return maze;
}

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

void State::assembleMazeSprite (Maze& curMaze)
{
	uint xsize = curMaze.cellSize * (uint)curMaze.grid[0].size();
	uint ysize = curMaze.cellSize * (uint)curMaze.grid.size();
	curMaze.cornerOffset = {float((scrw - xsize) / 2), float((scrh - ysize) / 2)};
	rt.create(xsize, ysize);
	rt.clear(Color::Transparent);
	Sprite spr(cellTxs[0]);
	float factor = float(curMaze.cellSize) / (spr.gLB().width - 1);
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

void State::movePC (Keyboard::Key k)
{
	char coordKey;
	int bitNum;
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
	
	if ( (getCell(pcLoc) & (1 << bitNum)) == 0) {
		pcLoc += dirCoords.at(coordKey);
		pcSpr.setPosition(toVecF(cellCtrToPixels(pcLoc)));
		if (pcLoc == curMaze.goalCell)
			winGame();
	}
	else {
		/* Can't go that way */
		//failed move sound
		//anim?
	}
}

void State::winGame ()
{
	pcSpr.setScale(2,2);
	//show path if not already drawn
	//sound, anim
	//delay till generate new?
}

u_char State::getCell (const vecI& vec)
{
	return curMaze.grid[vec.y][vec.x];
}

vecI State::cellCtrToPixels (const vecI& cell)
{
	return {cell.x * curMaze.cellSize + curMaze.cellSize / 2,
			cell.y * curMaze.cellSize + curMaze.cellSize / 2};
}

const string State::dirStr {"neswnes"};

const map<char, vecI> State::dirCoords {
	{'n', {0, -1}}
	, {'e', {1, 0}}
	, {'s', {0, 1}}
	, {'w', {-1, 0}}
};
