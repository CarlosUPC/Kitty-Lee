# Changelog

## [0.3.0] - 2018-11-18
### Added

- Enemies:
	- Flying.
	- Terrestrial.
- Path Finding module.
- Path Finding attached to enemies in order to go to player's position.
- Entity system.
- System load of entities with tsx files.
- Scene place entities automatically after load map.
- In God Mode now can fly.

### Changed

- Movement system with delta time between frames to move at same speed in all machines.
- Player now is a entity
- Save and Load game system is improved.

### Fixed

- Game crashes when enemy doesn't be able to go to player's position with pathfinding.

## [0.2.1] - 2018-10-14
### Added

- God mode utility.
- Player death animation and functionality.

## [0.2.0] - 2018-10-14
### Added

- New mechanic. Become a ghost.
- New player animation "ghost".
- When press S and spacebar on top of platform, player goes down.
- Change between scenes with colliders.

### Changed

- Improved scenes.

### Fixed

- When player falls with lot of speed and respawning, player moves out of map.
- If fall without jumping, player enters inside of floor.
- When change between scenes, seldom happen an error with audio.
- Player doesn't fade well between stages.

## [0.1.5] - 2018-10-13
### Added

- Complete player collision with map.
- Camera and player cannot go out of map.
- Save and load functionality with F5 and F6.
- New map created.
- Sounds effect while walking and jumping.
- In config.xml there is a variable that regule if starts game with debug activated.

### Changed

- Player collision system

### Fixed

- In Release doesn't fade into scenes well.

### Removed

## [0.1.0] - 2018-10-13
### Added

- Created main structure.
- Added assets.
- Added module player with basic functionality (Blit, walk, jump and its corresponding animations).
- Added collision system.
- Load Map, animations and colliders from xml file, edited with Tiled.
- Player collision with floor.

### Changed

### Fixed

### Removed
