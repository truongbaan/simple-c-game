# Wonder of Nothing

## About  
**Wonder of Nothing** is a simple text-based RPG game written in C. This is my first project in C and was created purely for fun while learning the language. The game runs in the terminal and features battles, skills, and upgrades.

## Features  
- Turn-based combat against monsters and a final boss.  
- Upgrade your character's stats after battles.  
- Choose and change your combat and ultimate skills.  
- Progressive difficulty with increasing enemy stats.  
- Text-based interface with simple commands.

## Installation  
### Prerequisites  
- You need a C compiler, such as **GCC**.  
- The game was tested using **GCC 13.2.0** and **VS Code**.  

### Compilation & Execution  
1. Open a terminal in the project directory.  
2. Compile the game using:  
   ```sh
   gcc -o rpg_game main_rpg_game.c
   ```
3. Run the game:  
   ```sh
   ./rpg_game
   ```

## How to Play  
- The game starts by asking if you want to play.  
- Navigate through battles and upgrade your character using keyboard inputs.  
- The commands available depend on the current game state:
  - **Main Menu:**  
    - `1` - Start battle.  
    - `2` - Change combat skill.  
    - `3` - Change ultimate skill.  
    - `4` - Upgrade character stats.  
    - `q` - Quit the game.
  - **During Battle:**  
    - `1` - Normal attack.  
    - `2` - Use skill (if available and not on cooldown).  
    - `3` - Use ultimate skill (if enough energy is available).  
    - `s` - Surrender.  
  - **Skill Selection:**  
    - `1` - Return to the previous menu.  
    - `2` - Select "Double X" (Requires Level 5).  
    - `3` - Select "Reduction" (Requires Level 10).  
    - `4` - Select "Holy Light" (Requires Level 15).  
  - **Ultimate Skill Selection:**  
    - `1` - Return to the previous menu.  
    - `2` - Select "Dividing" (Requires Level 5).  
    - `3` - Select "Triple Slash" (Requires Level 10).  
    - `4` - Select "Eliminate" (Requires Level 15).  
  - **Upgrade Section:**  
    - `1` - Return to the previous menu.  
    - `2` - Upgrade HP.  
    - `3` - Upgrade Attack.  
    - `4` - Upgrade Defense.

## Contributing  
This project was created for learning purposes. Feel free to fork and modify it for fun!

## License  
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

