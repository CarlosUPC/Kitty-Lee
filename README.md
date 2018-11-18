# Kitty Lee

A 2D platformer of a fighter cat.

## Getting Started

This is a small project made by two students of CITM - Terrassa of Design and Development of Video Games grade.

### Installing

* Download last release zip from our [Repository](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/releases).
* Extract zip.
* Run KittyLee.exe.

## Controls

* A to move left.
* D to move right.
* G to change between ghost form and normal form.
* Spacebar to jump.
* S + Spacebar on a platform to go down.

  ### Development controls
  
  * F1 Start from the very first level.
  * F2 Start from the beginning of the current level.
  * F3 Increase music volume of game.
  * F4 Decrease music volume of game.
  * F5 Save the current state.
  * F6 Load the previous state (even across levels).
  * F8 Switch between levels.
  * F9 To view colliders.
  * F10 God Mode.
  * F11 Cap on/off fps to 30.

## Built With

* [Microsoft Visual Studio 2017](https://visualstudio.microsoft.com/es/vs/)
* [Tiled](https://www.mapeditor.org/)
* [SDL](https://www.libsdl.org/license.php)
* [Pugi](https://pugixml.org/license.html)
* Compatible with [Brofiler](http://www.brofiler.com/)

## Art/Music used

* [Main Character](https://opengameart.org/content/cat-fighter-sprite-sheet)
* [Flying enemy](https://opengameart.org/content/flying-tongue-monster-sprite-sheet)
* [Map Tileset](https://ansimuz.itch.io/sunny-land-pixel-game-art)
* [Map objects](https://opengameart.org/content/a-platformer-in-the-forest)
* [Music Scene](https://opengameart.org/content/platformer-game-music-pack)
* [jump fx](https://opengameart.org/content/platformer-jumping-sounds)
* [walk_fx](https://freesound.org/people/FxKid2/sounds/362609/)

## Repository Link

[Repository](https://github.com/CarlosUPC/Kitty-Lee)

## Authors

* **Carlos Peña** - [CarlosUPC](https://github.com/CarlosUPC)
* **Christian Martínez de la Rosa** - [christt105](https://github.com/christt105)

See also the list of [contributors](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/graphs/contributors) who participated in this project.

## Innovation
### First Assignment
* Created a system to load data animation provided directly from Tiled. All player information is in a xml edited with Tiled.
* Created system to load parallax info from Tiled.
* Load collision boxes from Tiled with its own type collision.
* Created a mechanic that player can change to a ghost and be able to enter in rooms across specific walls. In ghost form, player oversteps platforms.
* While pressing 'S' and spacebar on top of a platform, player will trespass that platform.
* Save and Load functionallity in run time.
### Second Assignment
* Imported and remaked player load data to entity. Entity load common data and special data depending type of entity.
* System to load all enemies you put on Tiled in each scene.

## Members Task
### Carlos Peña
* Enemy gladiator (logic, pathfinding player)
* Enemy flying tongue (logic, pathfinding player)
### Christian Martínez
* Entity Manager system
* Apply dt system
* Cap on/off fps
* Adapt player movement to dt system
* Make Entity father class
* Adapt old player to new entity system
* Adapted old player load data to all entities
* Little changes adapting change scene/save & load/dead player with new entity manager

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/blob/master/LICENSE) file for details

MIT License

Copyright (c) 2018 CarlosUPC and christt105

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
