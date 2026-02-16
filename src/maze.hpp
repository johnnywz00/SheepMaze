//
//  maze.hpp
//  SheepMaze
//
//  Created by John Ziegler on 12/27/25.
//  Copyright © 2025 John Ziegler. All rights reserved.
//

#ifndef maze_hpp
#define maze_hpp

#include "jwzsfml.hpp"
#include "resourcemanager.hpp"
#include "timedeventmanager.hpp"
#include "zsprite.hpp"


using BitsetGrid = vector<vector<unsigned char>>;

/*
 Bits of unsigned char stand for:
 
							16			8			4			2			1
 UNUSED	UNUSED UNUSED	unresolved	westwall	southwall	eastwall	northwall
 */

struct Maze
{	
	BitsetGrid		grid;
	vecI			gridSize;
	vecF			spriteSize;
	int				cellSize;
	vecI			startCell;
	vecI			goalCell;
	vecF			cornerOffset;
};

#endif /* maze_hpp */
