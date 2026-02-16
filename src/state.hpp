/*
 - switch between fence and wall drawing
 - sheep sprites for going n/s
 - Fuses for smooth motion
 - friction sprite motion as optional gameplay?
 - panning for larger mazes
 - color traversed cells
 - highlight solution path
 - grass blades on background
 - get key (trampoline? somewhere in maze, bring it to goal (keyhole door, low wall to trampoline over...)
 
 - mazes where no path leads from start to goal, have to find where to place trampoline and get one jump over a fence
 
 {
 Maze generation:
-use some sort of sorting step/priorityq to start new branches from available remaining spots nearest the origin (potentially creates longest wrong roads, most challenging?)
-? try letting a branch go # cells then force rebranch, rather than letting it go to dead end before
 }
 */

#ifndef SHEEPMAZE_HPP
#define SHEEPMAZE_HPP

#include "maze.hpp"


class FullscreenOnlyApp;
class TimedEventManager;

class State
{
public:
	static State* getSelf () { return instance_; }
	
	void onCreate ();
	
	bool handleTextEvent (Event&);

	void onMouseDown (int x, int y);
	
	void onMouseUp (int x, int y) { }
	
	void onKeyPress (Keyboard::Key);
    
	void onKeyRelease (Keyboard::Key) { }
	
	void update (const Time& time);
	
	void draw ();
	
	RenderWindow*  		 	rwin;
	FullscreenOnlyApp* 		app;
	TimedEventManager*      timedMgr;
	vecI					oldMouse
							, mouseVec
	;

private:
	static inline const string		instrucsStr =
		"Y: Create new maze!   A/Shift + A: Adjust maze width   Z/Shift + Z:"
		" Adjust maze height          John Ziegler, 2026   johnnywz00@yahoo.com"
	;
	static inline const string				dirStr  = "neswnes";
	static inline const map<char, vecI> 	dirCoords {
		{'n', {0, -1}}
		, {'e', {1, 0}}
		, {'s', {0, 1}}
		, {'w', {-1, 0}}
	};
	
	void reset ();
		
	Maze generateNewMaze ();
	
	void createCellTxs ();
	
	void createCellTxs2 ();
	
	void assembleMazeSprite (Maze&);
	
	void assembleMazeSprite2 (Maze&);
	
	void loadVxArrFromMaze (Maze&);
	
	u_char getCell (const vecI&);
	
	vecI cellCtrToPixels (const vecI&);
	
	void movePC (Keyboard::Key);
	
	void winGame ();
	
	void adjustResourceColors ();
	
	static State* 		instance_;
	View				vw;

	Maze				curMaze;
	vector<Sprite>		rtSprVec;
	Sprite				rtSpr
						, bkgdSpr
						, pcSpr
						, goalSpr
	;
	Textbox				tbox;
	Text    			instrucsTxt
						, sizeTxt
	;
	
	RenderTexture		rt
						, cellRt
	;
	vector<Texture> 	rtTxVec;
	Texture				cellTxs[16];
	VertexArray 		va {Lines};
	
	Textbox*			activeTbox = nullptr;
	vecI 				gridSize;
	vecI				pcLoc;
}; //end class State

#endif




