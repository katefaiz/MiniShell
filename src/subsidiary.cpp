#include "subsidiary.h"
#include "tools.h"

void PrintGreeting(void) {
    printf(PINK_CLR "  __  __ _       _     _          _ _ \n");
    printf(" |  \\/  (_)     (_)   | |        | | |\n");
    printf(" | \\  / |_ _ __  _ ___| |__   ___| | |\n");
    printf(" | |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |\n");
    printf(" | |  | | | | | | \\__ \\ | | |  __/ | |\n");
    printf(" |_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|\n" END);
}

void PrintExit(void) {
    printf(CYAN("\n  Bye bye from\n"));
    printf(CYAN("  __  __ _  __ __ _ \n"));
    printf(CYAN(" |  \\/  (_) |  | (_)\n"));
    printf(CYAN(" |_|  |_|_|_|\\_||_|\n\n"));
    printf(GREEN("\n[minishell ended up working :)]\n"));
}