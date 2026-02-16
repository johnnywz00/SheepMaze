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
	
	void reset ();
	
	void debugTxtSetup ();
	
	void onMouseDown (int x, int y);
	
	void onMouseUp (int x, int y) { }
	
	void onKeyPress (Keyboard::Key);
    
	void onKeyRelease (Keyboard::Key) { }
	
	void update (const Time& time);
	
	void draw ();

	bool handleTextEvent (Event&);
	
	void createCellTxs ();
	
	void createCellTxs2 ();
	
	Maze generateNewMaze ();
	
	void loadVxArrFromMaze (Maze&);
	
	void assembleMazeSprite (Maze&);
	
	void assembleMazeSprite2 (Maze&);
	
	void movePC (Keyboard::Key);
	
	void winGame ();
	
	u_char getCell (const vecI&);
	
	vecI cellCtrToPixels (const vecI&);

	static State* 			instance_;
	RenderWindow*  		 	rwin;
	View					vw;
	FullscreenOnlyApp* 		app;
	TimedEventManager*      timedMgr;
	vecI					oldMouse
							, mouseVec
	;

	
	Maze							curMaze;
	vecI 							gridSize;
	static inline const string		instrucsStr =
		"Y: Create new maze!   A/Shift + A: Adjust maze width   Z/Shift + Z:"
		" Adjust maze height          John Ziegler, 2026   johnnywz00@yahoo.com"
	;
	static const string				dirStr;
	static const map<char, vecI> 	dirCoords;
	
	vecI							pcLoc;
	vecF							rtOffset;
	RenderTexture	rt
					, cellRt;
	Texture			cellTxs[16];
	VertexArray va {Lines};
	vector<Sprite>	rtSprVec;
	vector<Texture> rtTxVec;
	Sprite	rtSpr
			, bkgdSpr
			, pcSpr
			, goalSpr
	;
	
	Textbox					tbox;
	Textbox*				activeTbox = nullptr;
	
	

	Text    			 instrucsTxt,
						 debugTxt;
}; //end class State

#endif




