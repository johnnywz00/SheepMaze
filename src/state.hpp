/*
 - panning
 
 */

#ifndef SHEEPMAZE_HPP
#define SHEEPMAZE_HPP

#include "maze.hpp"


class FullscreenOnlyApp;
class TimedEventManager;

class State {
public:
	
	static State* getSelf () { return instance_; }
	
	void onCreate ();
	
	void reset ();
	
	void debugTxtSetup ();
	
	void onMouseDown (int x, int y);
	
	void onMouseUp (int x, int y);
	
	void onKeyPress (Keyboard::Key);
    
    void onKeyRelease (Keyboard::Key);
	
	void update (const Time& time);
	
	void draw ();

	bool handleTextEvent (Event&);
	
	void createCellTxs ();
	
	Maze generateNewMaze ();
	
	void loadVxArrFromMaze (Maze&);
	
	void assembleMazeSprite (Maze&);
	
	void movePC (Keyboard::Key);
	
	void winGame ();
	
	u_char getCell (const vecI&);
	
	vecI cellCtrToPixels (const vecI&);
	
	float SCRW () { return w->getDefaultView().getSize().x; }
	float SCRH () { return w->getDefaultView().getSize().y; }
	float SCRCX () { return w->getDefaultView().getSize().x / 2; }
	float SCRCY () { return w->getDefaultView().getSize().y / 2; }

	Maze							curMaze;
	vecI 							gridSize;
	static const string				dirStr;
	static const map<char, vecI> 	dirCoords;
	
	vecI							pcLoc;
	
	RenderTexture	rt
					, cellRt;
	Texture			cellTxs[16];
	VertexArray va {Lines};
	Sprite	rtSpr
			, bkgdSpr
			, pcSpr
	;
	
	Textbox					tbox;
	Textbox*				activeTbox = nullptr;
	static State* 			instance_;
	RenderWindow*  		 	w;
	SFGameWindow* 		 	gw;
	TimedEventManager*      timedMgr;
    int             	 	mx = 0,
							my = 0,
							mxOld = 0,
							myOld = 0;
	
	
////////////  DEBUG  /////////////////////

	Text    			 mouseTxt,
						 debugTxt;

}; //end class State


#endif




