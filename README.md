# SheepMaze

## C++/SFML sprite-based maze game

<img width="1728" height="1117" alt="Screenshot 2026-02-16 at 2 11 52 PM" src="https://github.com/user-attachments/assets/d843d7b6-dc51-4343-a450-aabd88211876" />

### ABOUT THE PROJECT
Early in my software engineering self-teaching journey, figuring out a maze generator was one of the challenges that made its way into my head and wouldn't leave till I'd take it on. As with most of my projects, the resulting product was eventually extended or modified to have some sort of usefulness or appeal to my small children, in this case making a controllable character to traverse the maze.

The projects of this period (ca. 2020) were motivated primarily by the desire to know if I could "solve" the challenge of bringing this or that mechanic or concept into being through programming language, largely without consulting any other sources to see how it was done (this was also pre-AI era). The projects were characterized by a lack of concern for code organization, best practices, readability, or maintainability, because my free time was very limited and my chief objective was getting the satisfaction of knowing I got the idea to work. Most of the time, when I achieved that much of the goal, I was ready to move on to the next logical challenge rather than pore over the details of making a well-rounded product fit for public consumption. 

The first incarnation of this game was done in Python and its Arcade library, which I dabbled in before broaching C++. In fact, the programming language utilized was the least of the discrepancies between the original version and this present rendition. The original sprite character moved fluidly with acceleration and friction, which seemed neat but ended up being frustrating for the kids, who had trouble navigating into the paths they wanted to go. And under the hood, the maze generation system was an extravagant convolution of bewilderment which was never 100% guaranteed to generate a maze without bugs. 
<details>
<summary>More rambling about the coding</summary>
The reason for this was that, according to the first plan that my mind latched on to for creating a maze, I had to firstly plot a path from the start point that actually reached the end point. It needed to be a random wander or else it would be a poor maze, but there were constraints about which way this initial path could turn when it ran up against a wall or a snaking part of itself. I actually find my original solution to be interesting to look back upon, even though it was way more work than was required, since I was ignorant of the basic queue-style behavior for making a maze. At any rate, I got it to consistently make a random path from start to goal without dead-ending itself against a wall or in a wide loop of itself; after that I picked random unresolved cells from the grid and made them follow the same constrained wandering method until they somewhere connected with the original path. 
</details>

Except when noted, all the graphics in my programs are created by me, some programmatically and some as pixel art. The majority of sound effects that I use are from the public domain.

### FILE DESCRIPTIONS
* **sfmlApp:**  Implements `main()` and the abstract app
* **state:**  Implements primary graphical elements, game logic, and maze generation
* **maze:**  Struct to hold maze data
  
(From my "reusable modules" repo: https://github.com/johnnywz00/SFML-shared-headers)
* **jwz:**  C++ utility functions, #defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific
* **resourcemanager:**  Static class for accessing resource files globally
* **timedeventmanager:**  Manages fuses/daemons, delayed callbacks

### BUILDING INSTRUCTIONS
Ready-made program files are available on the Releases page of this repository, with versions for MacOS, Windows, and Linux. NO INSTALLATION NECESSARY: just download and double-click. If your OS isn't supported by the pre-made versions, or if you have other reasons for building from source:
- Clone this repository, and navigate to the root folder of that clone in a terminal window.
- Run:
<pre>
   cmake -B build
   cmake --build build
</pre>
