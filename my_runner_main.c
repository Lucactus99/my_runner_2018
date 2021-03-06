/*
** EPITECH PROJECT, 2018
** MUL_my_runner_2018
** File description:
** my_runner_main
*/

#include "get_next_line.h"
#include "my.h"

void main_loop_2(struct sfRunner *sf)
{
    if (sf->positionPortal.x > 100)
        analyse_score(sf);
    analyse_map(sf);
    sfSprite_move(sf->spritePlayer, sf->mvmtPlayer);
    manage_spike(sf);
    manage_platform(sf);
    if (sf->playerCondition == END)
        manage_portal(sf);
    increase_speed(sf);
    move_sprites(sf, 100);
    sf->speedEnemy = 1.2 * sf->speedMoveGround;
    if (sfMusic_getStatus(sf->music) == sfPaused ||
    (sfMusic_getStatus(sf->music) == sfStopped && sf->positionPortal.x > 0) ||
    ((sfMusic_getPlayingOffset(sf->music).microseconds / 1000000.0) > 290.0)) {
        sfMusic_play(sf->music);
    }
    draw_sf(sf);
}

void main_loop(struct sfRunner *sf)
{
    while (sfRenderWindow_pollEvent(sf->window, &sf->event))
        analyse_events(sf);
    if (sf->playerCondition != PAUSE && sf->playerCondition != MENU &&
    sf->positionPortal.x > -100 && sf->positionPlayer.y < 800) {
        analyse_after_pause(sf);
        sf->time = sfClock_getElapsedTime(sf->clock);
        sf->timeSpawn = sfClock_getElapsedTime(sf->clockSpawn);
        sf->seconds = sf->time.microseconds / 1000000.0;
        sf->secondSpawn = sf->timeSpawn.microseconds / 1000.0 + sf->pauseTime;
        check_player_condition(sf);
        check_position_player(sf);
        check_position_player_platform(sf);
        check_position_2(sf);
        main_loop_2(sf);
    } else {
        analyse_menu(sf);
        analyse_pause(sf);
    }
}

void help(void)
{
    my_putstr("\nUSAGE\n");
    my_putstr("\t./my_runner [OPTIONS]\n");
    my_putstr("\nDESCRIPTION\n");
    my_putstr("\tHelp Pickle Rick to reach the portal !\n");
    my_putstr("\tBe careful, Giant Heads placed obstacles on your way !\n");
    my_putstr("\nOPTIONS\n");
    my_putstr("\tmap.txt\t\tlaunch the game.\n");
    my_putstr("\tmap.txt -i\tlaunch the game in infinity mode.\n");
    my_putstr("\t-h\t\tprint the usage and quit.\n");
    my_putstr("\nUSER INTERACTIONS\n");
    my_putstr(" During the game:\n");
    my_putstr("\tSPACE_KEY\tjump.\n");
    my_putstr("\tRETURN_KEY\tpause menu.\n");
    my_putstr(" In the end or dead menu:\n");
    my_putstr("\tQ_KEY\t\tquit the game.\n");
    my_putstr("\tR_KEY\t\trestart the game.\n");
    my_putstr(" In main menu:\n");
    my_putstr("\tE_KEY\t\tchange the skin.\n");
}

int main(int ac, char **av)
{
    struct sfRunner *sf = malloc(sizeof(struct sfRunner));
    int fd;

    if (check_args(ac, av, sf) == 84)
        return (84);
    fd = open(av[1], O_RDONLY);
    if (fd <= 0)
        return (84);
    create_map(sf, fd, av[1]);
    create_sf(sf);
    if (check_errors(sf) == 84) {
        destroy_texture_sprite(sf);
        sfRenderWindow_destroy(sf->window);
        return (84);
    }
    while (sfRenderWindow_isOpen(sf->window))
        main_loop(sf);
    destroy_texture_sprite(sf);
    sfRenderWindow_destroy(sf->window);
    return (0);
}