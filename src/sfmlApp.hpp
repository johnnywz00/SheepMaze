
#ifndef SFMLAPP_HPP
#define SFMLAPP_HPP


#include "state.hpp"
#include "timedeventmanager.hpp"

inline const string defaultTitle { "SheepMaze" };
inline const string iconPath { "resources/images/icon.png" };
inline Color baseScreenColor = Color::White;


class SFGameWindow
{
public:
	friend class Game;
    
    SFGameWindow ();

    SFGameWindow (const string& title, const vecU& size);
    
	~SFGameWindow () { destroy(); }
    
	void draw (Drawable& d) { window.draw(d); }
    
	void beginDraw () { window.clear(redrawColor); }
    
	void endDraw () { window.display(); }
    
	bool isDone () { return _isDone; }
    
	bool isFullscreen () { return _isFullscreen; }
        
	bool isFocused () { return _isFocused; }
    
	vecU getWindowSize () { return windowSize; };
    
	RenderWindow* getRenderWindow () { return &window; };
    
	void close () { _isDone = true; };
    
	void toggleFullscreen ();
	
	void setRedrawColor(const Color& c) { redrawColor = c; }
	    
	Color					redrawColor = baseScreenColor;
    
private:
    
    void destroy () { window.close(); };
    
	void setup (const string& title, const vecU& size);
    
	void create ();
    
    Image                   icon;
    RenderWindow            window;
    vecU                    windowSize;
    static const int        defaultWidth { 1280 };
    static const int        defaultHeight { 720 };
    string                  windowTitle;

    bool                    _isDone;
    bool                    _isFullscreen;
    bool                    _isFocused;
};




class Game
{
public:
    
    Game ();
       
	void update ();
    
	void render () {
        window.beginDraw();
        state.draw();
        window.endDraw();
	}
    
	void lateUpdate () { restartClock(); }
    
	SFGameWindow* getWindow () { return &window; };
    
	Time getElapsed () { return elapsed; };
    
	void restartClock () { elapsed += clock.restart(); };

private:

    SFGameWindow     	window;
	TimedEventManager   timedMgr;
    State            	state;
    Clock            	clock;
    Time             	elapsed;
};




#endif /* SFMLAPP_HPP */
