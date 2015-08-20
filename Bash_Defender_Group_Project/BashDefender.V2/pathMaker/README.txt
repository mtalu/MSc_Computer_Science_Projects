

Path Maker Tool

general: the tool will make a .txt file containing the path length and coordinates in data/levels/lvl_<levelNumber>/paths/
MAKE SURE there is a folder lvl_<levelNumber> in the levels folder or the pathMaker will not work properly
e.g. there must be a lvl_2 folder already created for making paths in level 2

Instructions

run ./pathMaker from terminal

enter the level number at prompt
enter the path number at prompt

these will be saved in '../data/levels/lvl_<levelNumber>/paths/' with filename 'path_<pathNumber>.txt'
if there is already a file with this name the program wil prompt for overwrite.


the program will then load a map (saved as 'map.png' in the pathMaker folder) for you to draw a path on

hold down the left mouse button to add coordinates to the path. The current path length and coordinates will be displayed in terminal.

ADVICE

paths of about 2,000 length are needed for good enemy speed
the path you draw will be where the TOP LEFT of the enemy appears, so don't draw down the middle of the path




