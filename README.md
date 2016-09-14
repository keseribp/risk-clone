# world_conquer_game

## Build:
`$ nvcc -std=c++11 player.cpp uiObject.cpp frame.cpp canvas.cpp panel.cpp button.cpp label.cpp radioGroup.cpp humanPlayer.cpp splash.cpp gameBoard.cpp renderer.cpp main.cpp -o game -lglfw3 -lGLU -lGL -lGLEW -lX11 -lXxf86vm -lXinerama -lXcursor -lrt -lm -lXrandr -lpthread -lXi -ldl`

## Screenshots:

![Image](https://github.com/keseribp/world_conquer_game/blob/master/screenshot.png)

## Initial Placement:

Blue and Red players trade off placing two units of their own, onto owned territories, and one Green neutral unit, per turn. The "Next" button will appear when the total of three units have been placed. Place units by left clicking on a territory and remove placed units by right clicking on a territory. Green units only defend.

## Placement Phase:

At the beggining of the attacking player's turn, they may choose to use three cards to gain extra troops. Cards must be three of a kind or all unique with respect to troop type labels. If the player has five cards, the player must use three that turn. The number of units each set of three cards is exchanged for is increased after each exchange. One extra unit is placed on any territory you control maching a territory label on an exchanged card. Additionally, players are awarded a number of units proportional to the number of territories owned and bonus units for each continent totally controlled.

## Attack Phase:

Choose a territory you own and a legal adjacent territory to attack. Next, choose the number of units to attack with and then press the "Attack" button to simulat a dice roll and to determine the battle outcome. If you conquer the territory, you have the opportunity to move your units forward. The number of units used in the attack must alwasy move forward if the territory is conquered and one unit must always occupy any one of your territories.

## End Turn:

One free unit movement is allowed, where the attacking player may move any legal number of units from one territory to another adjacent owned territory. If a territory was conquered during the turn, then the attacking player gets one card. Begin placement phase for previously defending player.

## End Game:

The game ends when either the Blue or Red player is annihilated from the game map.
