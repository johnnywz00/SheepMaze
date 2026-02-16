# SheepMaze

## C++/SFML sprite-based maze game

(DRAGANDDROPSCREENSHOTHERE)

### ABOUT THE PROJECT

## COMING SOON
Mention the unnecessary elaboration of python version

### FILE DESCRIPTIONS
* **sfmlApp:**  Implements `main()` and the abstract app
* **:**  Implements primary graphical elements; physics; game logic
  
(From my "reusable modules" repo: https://github.com/johnnywz00/SFML-shared-headers)
* **jwz:**  C++ utility functions, #defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific

### BUILDING INSTRUCTIONS
Ready-made program files are available on the Releases page of this repository, with versions for MacOS, Windows, and Linux. NO INSTALLATION NECESSARY: just download and double-click. If your OS isn't supported by the pre-made versions, or if you have other reasons for building from source:
- Clone this repository, and navigate to the root folder of that clone in a terminal window.
- Run:
<pre>
   cmake -B build
   cmake --build build
</pre>
