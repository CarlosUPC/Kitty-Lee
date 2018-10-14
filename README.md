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
  * F11 Insta kill.

## Built With

* [Microsoft Visual Studio 2017](https://visualstudio.microsoft.com/es/vs/)
* [Tiled](https://www.mapeditor.org/)
* [SDL](https://www.libsdl.org/license.php)
* [Pugi](https://pugixml.org/license.html)

## Art/Music used

* [Main Character](https://opengameart.org/content/cat-fighter-sprite-sheet)
* [Map Tileset](https://ansimuz.itch.io/sunny-land-pixel-game-art)
* [Map objects](https://opengameart.org/content/a-platformer-in-the-forest)
* [Music Scene](https://opengameart.org/content/platformer-game-music-pack)
* [jump fx](https://opengameart.org/content/platformer-jumping-sounds)
* [walk_fx](https://freesound.org/people/FxKid2/sounds/362609/)

## Authors

* **Carlos Peña** - [CarlosUPC](https://github.com/CarlosUPC)
* **Christian Martínez de la Rosa** - [christt105](https://github.com/christt105)

See also the list of [contributors](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/CarlosUPC/Game_Dev_2DPlatformGame/blob/master/LICENSE) file for details

## Innovation

* Created a system to load data animation provided directly from Tiled. All player information is in a xml edited with Tiled.
* Created system to load parallax info from Tiled.
* Load collision boxes from Tiled with its own type collision.
* Created a mechanic that player can change to a ghost and be able to enter in rooms across specific walls. In ghost form, player oversteps platforms.
* While pressing 'S' and spacebar on top of a platform, player will trespass that platform.
* Save and Load functionallity in run time.
