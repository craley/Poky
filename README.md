# Pokedex
Software Engineering team project for CEN4010

[Team Calendar](https://www.google.com/calendar/embed?src=131fpt7evnm5tlck78joh8210g%40group.calendar.google.com&ctz=America/New_York )

#### Introduction
######Core mechanics:
Search for Pokemon for name or types
JSON for serialization

######Mini Games:
- Who's that Pokemon? & Pokemon quiz
- Pokemon snap 2D

######Ideas:
- Give out Pokemon cards (achievements)
- Facebook integration

######Nice to have:
- Augmented reality
- Mobile (Android)

#### Dependencies
- [SDL2](https://www.libsdl.org/download-2.0.php)
- [SDL_image 2.0](https://www.libsdl.org/projects/SDL_image/)
- [SDL_ttf 2.0](https://www.libsdl.org/projects/SDL_ttf/)
- [Sqlite 3](http://www.sqlite.org/download.html)

#### Workflow Instructions
1. Fork this repo
2. Clone: ```git clone link-to-your-fork```
3. Create branch: ```git checkout -b my-feature```
4. Make some change
5. Commit it ```git commit```
6. Do a pull request

#### Build Instructions
Building  requires [Cmake >= 2.8](http://www.cmake.org/) in order to generate the build files

##### *nix instructions

```bash
mkdir _build && cd _build
cmake ..
make
make install
```
A pokedex executable will be generated in the root

##### Windows instructions
1. Download the cmake gui tool
2. Set the source path to point to the pokedex root
3. Set the build path to point to Pokedex/_build
4. configure
5. generate
6. the generated visual studio (or other ide files) project is in _build
