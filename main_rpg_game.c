//making a text rpg game for fun
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>

#define MAX_FLOOR 20
#define START_STAT 699 // player stats at the beginning
#define BUFF START_STAT*0.05 // player stat increase each time lose
#define STAT_INCREASE START_STAT*0.3 // player stat plus each time use point
#define MONSTER_STAT START_STAT*0.45 // monster stats at when first start
#define MIN_BUFF (int)(MONSTER_STAT*0.3) // for variety of monster stats
#define MAX_BUFF (int)(MONSTER_STAT*0.5) // for variety of monster stats

typedef struct{int level; int hp; int atk; int def; int energy;}Player_Stat;
typedef struct{int skill; int ult;}Player_ability;
typedef struct{int hp; int atk; int def;}Enemy_Stat;

void clearScreen() {
    printf("\033[H\033[J"); // ANSI escape sequence to clear screen (or you can use: system("cls"); -> for window)
}

void guide(){
    printf("Press 1 to start the battle\n");
    printf("Press 2 to change the combat skill\n");
    printf("Press 3 to change the ultimate skill\n");
    printf("Press 4 to go to the upgrade section\n");
    printf("Press q to quit the game\n");
}

void boss_start_battle(Player_Stat player, Player_ability player_skill, Enemy_Stat monster,int *battle_number, int *point, int *win_or_lose){
    *win_or_lose = 0;
    for(int j = 0; j < *battle_number; j++){
        for(int i = 0; i < sizeof(Enemy_Stat) / sizeof(int); i++){ // change stats of the enemy
            int stat_buff = rand() % (MAX_BUFF - MIN_BUFF + 1) + MIN_BUFF;
            switch (i) {
                case 0:
                    monster.hp += stat_buff;
                    break;
                case 1:
                    monster.atk += stat_buff;
                    break;
                case 2:
                    monster.def += stat_buff;
                    break;
                default:
                    // Handle unexpected index
                    break;
            }
        }
    }

    int skill_cooldown = 0; // for skill 2
    int monster_turn = 0; // check if it can crit, deal 1.5* damage to player and reduce their current defense by 15%
    int monster_ult = 0; // check for boss ult, deal 3* damage to player and heal itself by 10% of its 3*dmg
    int corrosion_dmg = 0; // deal 10% of the boss dmg each turn
    while(1){
        player.energy++;
        char player_turn;
        while(1){
            printf("Floor: %d\n", *battle_number+1);
            printf("Battle Start:\n");
            printf("Your level: %d     Hp: %d     Atk: %d     Def: %d     Energy: %d\n",player.level, player.hp, player.atk, player.def, player.energy);
            printf("Final Boss         Hp: %d     Atk: %d    Def: %d\n",monster.hp, monster.atk, monster.def);
            printf("Your turn!\n");
            printf("Press 1 for normal attack\n");
            printf("Press 2 to use skill\n");
            printf("Press 3 to use ultimate skill\n");
            printf("Press s to surrender\n");
            
            scanf(" %c", &player_turn);
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Consume characters until newline or EOF
            clearScreen();

            if(player_turn == 's'){
                printf("You surrender!\n");
                return;
            }

            if(player_turn == '1'){ // normal atk
                if(monster.def < player.atk){
                    printf("You deal %d damage to the boss\n", player.atk - monster.def);
                    monster.hp = monster.hp - player.atk + monster.def;
                } else{
                    printf("The boss def is too high. You only deal %d damage to the boss\n", (int)(player.atk*0.05));
                    monster.hp = monster.hp - (int)(player.atk*0.05);
                }
                break;
            }

            if(player_turn == '2' && skill_cooldown == 0){ //skill
                switch(player_skill.skill){
                    case 2:
                        if(player.atk*2-monster.def > 0){
                            printf("You use Double X, and deal %d damage\n",player.atk*2-monster.def);
                            monster.hp = monster.hp - (player.atk*2 - monster.def);
                        }
                        else{
                            printf("Enemy defense is too high! You deal 0 damage!\n");
                        }
                        skill_cooldown = 2;
                        break;
                    case 3:
                        printf("You use Reduction! Enemy defense is reuduced by 20%%\n");
                        monster.def = (int)((float)monster.def*0.8);
                        skill_cooldown = 2;
                        break;
                    case 4:
                        printf("You use Holy Light! Your atk increase by 10%% and your current hp increase by 10%%\n");
                        printf("You gain an extra turn!\n");
                        player.atk = (int)((float)player.atk*1.1);
                        player.hp = (int)((float)player.hp*1.1);
                        skill_cooldown = 2;
                        player.energy++;
                        break;
                    default:
                        printf("You don't have any skill set up\n");
                        break;
                }

                if(player_skill.skill != 0 && player_skill.skill != 4){ // check if the skill is used
                    break; // out the while loop of the player turn
                }
            } else if(player_turn == '2' && skill_cooldown != 0){
                printf("Your skill still in cooldown!\n");
            }

            if(player_turn == '3' && player.energy >= 5){ //ultimate skill
                switch(player_skill.ult){
                    case 2:
                        printf("You use Dividing, enemy stat is reduce by 15%% and deal %d true damage\n", (int)(player.atk*1.5));
                        monster.hp -= player.atk*1.5;
                        monster.def = (int)((float)monster.def*0.85);
                        monster.atk = (int)((float)monster.atk*0.85);
                        player.energy -= 5;
                        break;
                    case 3:
                        printf("You use Triple Slash! You deal %d true damage\n", player.atk*2.5);
                        monster.hp -= (int)(player.atk*2.5);
                        player.energy -= 5;
                        break;
                    case 4:
                        printf("You use Eliminate! You deal %d true damage to the enemy and deal %d to yourself\n", player.atk*3, (int)(player.atk*0.25));
                        player.hp -= (int)(player.atk*0.25);
                        monster.hp -= player.atk*3;
                        player.energy -= 5;
                        break;
                    default:
                        printf("You don't have any ultimate skill set up\n");
                        break;
                }
                if(player_skill.ult != 0){ // check if the ult is used
                    break; // out the while loop of the player turn
                }
               
            } else if(player_turn == '3' && player.energy < 5){
                printf("You dont have enough enery to cast the ultimate skill\n");
            }
            
            if(player_turn != '1' && player_turn != '2' && player_turn != '3' && player_turn != 's'){
                printf("Seems like you have mistype! Please try again\n");
            }
        }

        printf("\n");

        if(monster.hp <= 0){
            printf("You win!\n");
            (*battle_number)++;
            (*point)++;
            *win_or_lose = 1;
            return;
        }

        // monster turn
        printf("Boss turn!\n");
        monster_turn++;
        monster_ult++;
        if(player.def < monster.atk){
            printf("The Boss deals %d damage to you\n", monster.atk - player.def);
            player.hp = player.hp - (monster.atk - player.def);
        }
        else{ // minimum damage for monster if your stat too high
            int least_damage_to_player = (int)((monster.atk*5)/100);
            printf("Your defense is too high, the monster can only deal %d damage to you\n",least_damage_to_player);
            player.hp -= least_damage_to_player;
            
        }

        if(monster_turn % 5 == 0){ // extra turn for monster
            printf("The Boss deals an extra %d true damage and reduce your current defense by 15%%\n", (int)(monster.atk*1.5));
            player.hp -= monster.atk*1.5;
            player.def = (int)((float)player.def*0.75);
            monster_turn = 0;
        }
        if(monster_ult % 10 == 0){
            printf("The Boss deals an extra %d true damage and recover %d HP\n", monster.atk*3, (int)(monster.atk*3*0.1));
            printf("The Boss applies corrosion damage to player for 2 turns\n");
            player.hp -= monster.atk*3;
            monster.hp += monster.atk*3*0.1;
            monster_ult = 0;
            corrosion_dmg = 2;
        }
        if(corrosion_dmg > 0 ){
            corrosion_dmg--;
            printf("Player lost %d hp due to corrosion damage\n",(int)(monster.atk*0.1));
            player.hp -= monster.atk*0.1;
        }
        if(skill_cooldown > 0){
            skill_cooldown--; // reduce cooldown of the skill
        }

        if(player.hp <= 0){
            printf("You lose!\n");
            return;
        }

        printf("\n");
        
    }
    return;
}

void start_battle(Player_Stat player, Player_ability player_skill, Enemy_Stat monster,int *battle_number, int *point, int *win_or_lose){
    *win_or_lose = 0;
    for(int j = 0; j < *battle_number; j++){
        for(int i = 0; i < sizeof(Enemy_Stat) / sizeof(int); i++){ // change stats of the enemy
            int stat_buff = rand() % (MAX_BUFF - MIN_BUFF + 1) + MIN_BUFF;
            switch (i) {
                case 0:
                    monster.hp += stat_buff;
                    break;
                case 1:
                    monster.atk += stat_buff;
                    break;
                case 2:
                    monster.def += stat_buff;
                    break;
                default:
                    // Handle unexpected index
                    break;
            }
        }
    }

    int skill_cooldown = 0; // for skill 2
    int monster_turn = 0; // check if it can crit, deal 1.5* damage to player and reduce their defense by 10%
    while(1){
        player.energy++;
        char player_turn;
        while(1){
            printf("Floor: %d\n", *battle_number+1);
            printf("Battle Start:\n");
            printf("Your level: %d     Hp: %d     Atk: %d     Def: %d     Energy: %d\n",player.level, player.hp, player.atk, player.def, player.energy);
            printf("Monster            Hp: %d     Atk: %d    Def: %d\n",monster.hp, monster.atk, monster.def);
            printf("Your turn!\n");
            printf("Press 1 for normal attack\n");
            printf("Press 2 to use skill\n");
            printf("Press 3 to use ultimate skill\n");
            printf("Press s to surrender\n");
            
            scanf(" %c", &player_turn);
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Consume characters until newline or EOF
            clearScreen();

            if(player_turn == 's'){
                printf("You surrender!\n");
                return;
            }
            if(player_turn == '1'){ // normal atk
                if(monster.def < player.atk){
                    printf("You deal %d damage to the monster\n", player.atk - monster.def);
                    monster.hp = monster.hp - player.atk + monster.def;
                } else{
                    printf("The monster def is too high. You only deal %d damage to the monster\n", (int)(player.atk*0.05));
                    monster.hp = monster.hp - (int)(player.atk*0.05);
                }
                break;
            }

            if(player_turn == '2' && skill_cooldown == 0){ //skill
                switch(player_skill.skill){
                    case 2:
                        if(player.atk*2-monster.def > 0){
                            printf("You use Double X, and deal %d damage\n",player.atk*2-monster.def);
                            monster.hp = monster.hp - (player.atk*2 - monster.def);
                        }
                        else{
                            printf("Enemy defense is too high! You deal 0 damage!\n");
                        }
                        skill_cooldown = 2;
                        break;
                    case 3:
                        printf("You use Reduction! Enemy defense is reuduced by 20%%\n");
                        monster.def = (int)((float)monster.def*0.8);
                        skill_cooldown = 2;
                        break;
                    case 4:
                        printf("You use Holy Light! Your atk increase by 10%% and your current hp increase by 10%%\n");
                        printf("You gain an extra turn!\n");
                        player.atk = (int)((float)player.atk*1.1);
                        player.hp = (int)((float)player.hp*1.1);
                        skill_cooldown = 2;
                        player.energy++;
                        break;
                    default:
                        printf("You don't have any skill set up\n");
                        break;
                }

                if(player_skill.skill != 0 && player_skill.skill != 4){ // check if the skill is used
                    break; // out the while loop of the player turn
                }
            } else if(player_turn == '2' && skill_cooldown != 0){
                printf("Your skill still in cooldown!\n");
            }

            if(player_turn == '3' && player.energy >= 5){ //ultimate skill
                switch(player_skill.ult){
                    case 2:
                        printf("You use Dividing, enemy stat is reduce by 15%% and deal %d true damage\n", (int)(player.atk*1.5));
                        monster.hp -= (int)(player.atk*1.5);
                        monster.def = (int)((float)monster.def*0.85);
                        monster.atk = (int)((float)monster.atk*0.85);
                        player.energy -= 5;
                        break;
                    case 3:
                        printf("You use Triple Slash! You deal %d true damage\n", player.atk*3);
                        monster.hp -= (int)(player.atk*2.5);
                        player.energy -= 5;
                        break;
                    case 4:
                        printf("You use Eliminate! You deal %d true damage to the enemy and deal %d to yourself\n", player.atk*3, (int)(player.atk*0.25));
                        player.hp -= (int)(player.atk*0.25);
                        monster.hp -= player.atk*3;
                        player.energy -= 5;
                        break;
                    default:
                        printf("You don't have any ultimate skill set up\n");
                        break;
                }
                if(player_skill.ult != 0){ // check if the ult is used
                    break; // out the while loop of the player turn
                }
               
            } else if(player_turn == '3' && player.energy < 5){
                printf("You dont have enough enery to cast the ultimate skill\n");
            }
            
            if(player_turn != '1' && player_turn != '2' && player_turn != '3' && player_turn != 's'){
                printf("Seems like you have mistype! Please try again\n");
            }
        }

        if(monster.hp <= 0){
            printf("You win!\n");
            (*battle_number)++;
            (*point)++;
            *win_or_lose = 1;
            return;
        }

        printf("\n");//seperate line for player turn and monster turn

        // monster turn
        printf("Monster turn!\n");
        monster_turn++;
        if(player.def < monster.atk){
            printf("The monster deals %d damage to you\n", monster.atk - player.def);
            player.hp = player.hp - (monster.atk - player.def);
            if(player.hp <= 0){
                printf("You lose!\n");
                return;
            }
        }
        else{ // minimum damage for monster if your stat too high
            int least_damage_to_player = (int)((monster.atk*5)/100);
            printf("Your defense is too high, the monster can only deal %d damage to you\n",least_damage_to_player);
            player.hp -= least_damage_to_player;
        }

        if(monster_turn % 5 == 0){ // extra turn for monster
            printf("Monster deals an extra %d true damage and reduce your current defense by 10%%\n", (int)(monster.atk*1.5));
            player.hp -= monster.atk*1.5;
            player.def = (int)((float)player.def*0.8);
            monster_turn = 0;
        }
        
        if(skill_cooldown > 0){
            skill_cooldown--; // reduce cooldown of the skill
        }

        if(player.hp <= 0){
            printf("You lose!\n");
            return;
        }
        printf("\n");
        
    }

    printf("\n");

    return;
}

void skill_change(Player_Stat *player, Player_ability *player_skill){
    unsigned char choose_skill;
    while(1){
        switch(player_skill->skill){
            case 2:
                printf("Your current skill is now Double X\n");
                break;
            case 3:
                printf("Yourc current skill is now Reduction\n");
                break;
            case 4:
                printf("Your current skill is now Holy Light\n");
                break;
            default:
                printf("Your current skill is not set\n");
                break;
        }

        printf("\nDo you want to change your combat skill?\n");
        printf("Press 1 to return to the last screen\n");
        printf("Press 2 to change your current skill to \" Double X \" \n");
        printf("    Description: Deal 2*atk damage\n");
        printf("Press 3 to change your current skill to \" Reduction \" \n");
        printf("    Description: Reduce enemy's defense by 20%%\n");
        printf("Press 4 to change your current skill to \" Holy Light \" \n");
        printf("    Description: Gain an extra turn, increase self's atk and current hp by 10%%\n");

        scanf(" %c", &choose_skill);
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Consume characters until newline or EOF
        clearScreen();

        if(choose_skill == '1'){
            break; // return to the previous screen
        }
        if(choose_skill == '2'){ //choosing
            if(player->level >= 5){
                player_skill->skill = 2;
            }
            else{
                printf("Your level is not high enough\n");
            }     
        }
        if(choose_skill == '3'){
            if(player->level >= 10){
                player_skill->skill = 3;
            }
            else{
                printf("Your level is not high enough\n");
            }     
        }
        if(choose_skill == '4'){
            if(player->level >= 15){
                player_skill->skill = 4;
            }
            else{
                printf("Your level is not high enough\n");
            }     
        }
        if(choose_skill != '1' && choose_skill != '2' && choose_skill != '3' && choose_skill != '4'){
            printf("Did you mistype? Please try again!\n");
        }
    }
    return;
}

void ult_change(Player_Stat *player, Player_ability *player_skill){
    unsigned char choose_ult;
    while(1){
        switch(player_skill->ult){
            case 2:
                printf("Your current ultimate skill is now Dividing\n");
                break;
            case 3:
                printf("Your current ultimate skill is now Triple Slash\n");
                break;
            case 4:
                printf("Your current ultimate skill is now Eliminate\n");
                break;
            default:
                printf("Your current ultimate skill is not set\n");
                break;
        }

        printf("\nDo you want to change your combat skill?\n");
        printf("Press 1 to return to the last screen\n");
        printf("Press 2 to change your current ultimate skill to \" Dividing \" \n");
        printf("    Description: Reduce enemy stat by 15%% and deal 1.5*atk damage\n");
        printf("Press 3 to change your current ultimate skill to \" Triple Slash \" \n");
        printf("    Description: Deal 2.5*atk true damage\n");
        printf("Press 4 to change your current ultimate skill to \" Eliminate \" \n");
        printf("    Description: Sacrifice 0.25*atk of your hp to deal 3*atk true damage to the enemy\n");

        scanf(" %c", &choose_ult, 1);
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Consume characters until newline or EOF
        clearScreen();

        if(choose_ult == '1'){
            clearScreen();
            break; // return to the previous screen
        }
        if(choose_ult == '2'){ //choosing
            if(player->level >= 5){
                player_skill->ult = 2;
            }
            else{
                printf("Your level is not high enough\n");
            }     
        }
        if(choose_ult == '3'){
            if(player->level >= 10){
                player_skill->ult = 3;
            }
            else{
                printf("Your level is not high enough\n");
            }     
        }
        if(choose_ult == '4'){
            if(player->level >= 15){
                player_skill->ult = 4;
            }
            else{
                printf("Your level is not high enough\n");
            }     
        }
        if(choose_ult != '1' && choose_ult != '2' && choose_ult != '3' && choose_ult != '4'){
            printf("Did you mistype? Please try again!\n");
        }
    }
    return;
}

void upgrade_section(int *point, Player_Stat *player){
    char up;
    while(1){
        printf("Your level: %d      Points: %d\n", player->level, *point);
        printf("Hp: %d     Atk: %d     Def: %d\n",player->hp, player->atk, player->def);
        printf("\n");
        printf("Note: You can only upgrade if you have enough point\n");
        printf("Press 1 to return to the last screen\n");
        printf("Press 2 to upgrade your hp\n");
        printf("Press 3 to upgrade your atk\n");
        printf("Press 4 to upgrade your def\n");

        scanf(" %c", &up, 1); // scan for input
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Consume characters until newline or EOF
        clearScreen();

        if(up == '1'){
            break; // return to the previous screen
        }
        if(*point == 0){
            printf("Seems like you dont have enough point, why dont you fight some monsters?\n");
        }
        if(*point > 0 && up == '2'){ //choosing
            player->hp+=STAT_INCREASE;
            (*point)--;
        }
        if(*point > 0 && up == '3'){
            player->atk+=STAT_INCREASE*0.25;
            (*point)--;
        }
        if(*point > 0 && up == '4'){
            player->def+=STAT_INCREASE*0.3;
            (*point)--;
        }
        if(up != '1' && up != '2' && up != '3' && up != '4' && *point > 0){
            printf("Did you mistype? Please try again!\n");
        }
    }
    
    return;
}

int main(){
    Player_Stat player = { 1, START_STAT, START_STAT*0.75, START_STAT*0.4, 1}; // base stat for player
    //Player_Stat player = { 16, 100000, 120, 220, 40}; // base stat for testing
    int point = 0;
    Player_ability player_skill = { 0, 0 };
    Enemy_Stat monster = { MONSTER_STAT, MONSTER_STAT*0.35, MONSTER_STAT*0.15}; // base stat for monster
    Enemy_Stat boss = { MONSTER_STAT*6, MONSTER_STAT*1.3, MONSTER_STAT*2.2};
    srand(time(NULL));
    char start_game[10];
    //asking if you wanna play or not
    while(1){
        printf("Welcome to the Wonder of Nothing, would you like to challenge yourself to complete the game? (Yes or No)\n");
        scanf("%s", start_game);
        for(int i = 0; i < strlen(start_game); i++){
            start_game[i] = tolower(start_game[i]);
        }
        if(strcmp(start_game, "yes") == 0){
            printf("\n");
            break;
        }
        else if(strcmp(start_game, "no") == 0){
            printf("O..Ok... Then see you next time\n");
            return 0;
        }
        else{
            printf("It seems like you have mistype...\n");
        }
    }
    clearScreen();
    int battle_number = 0;
    while(1){
        int win_or_lose;
        char input;
        printf("Welcome to floor %d\n",battle_number+1);
        guide();
        scanf(" %c", &input, 1);
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Consume characters until newline or EOF

        if(input == '1'){
            clearScreen();
            if(battle_number == MAX_FLOOR - 1){
                boss_start_battle(player, player_skill, boss, &battle_number, &point, &win_or_lose);
            }
            else{
                start_battle(player, player_skill, monster, &battle_number, &point, &win_or_lose);
            }
            if(win_or_lose == 1){
                player.level++;
                player.hp += STAT_INCREASE*0.6;
                player.atk += STAT_INCREASE*0.2;
                player.def += STAT_INCREASE*0.15;
            } else{
                printf("Sadly you lose the fight, don't worry, you have been a little bit stronger now!\n");
                player.hp += BUFF*0.3;
                player.atk += BUFF*0.25;
                player.def += BUFF*0.1;
            }
        } else if(input == '2'){
            clearScreen();
            skill_change(&player, &player_skill);
        } else if(input == '3'){
            clearScreen();
            ult_change(&player, &player_skill);
        } else if(input == '4'){
            clearScreen();
            upgrade_section(&point, &player);
        } else if(input == 'q'){
            printf("Thank you for playing!\n");
            break;
        } else {
            printf("It seems like you have mistyped, please try again!\n");
        }
        if(battle_number == MAX_FLOOR){
            printf("Congratulations! You have completed the game!\n");
            printf("This is your stats after complete the game\n");
            printf("Your level: %d  Hp: %d  Atk: %d  Def: %d", player.level, player.hp, player.atk, player.def);
            //print the stats of the player (not yet)
            break;
        }
    }
    return 0;
}