#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Card{
    int card;
    int score;
    struct Card *next; // point next card
} Card;

// function prototype
Card make_deck();
Card seat_player();
void game_init(Card* deck, Card* player, Card* dealer);
Card* make_card(int number);
void append_card(Card* deck, Card* card);
Card* draw_card(Card* deck, int offset);
void print_deck(Card* deck);
int deck_size(Card* deck);
int total_score(Card *deck);
void player_turn(Card *deck, Card *player);
void dealer_turn(Card *deck, Card *dealer);
void game_result(Card *player, Card *dealer);
void change_score(Card *player, int offset);
void delete_deck(Card* deck);

int main() {

    // game loop
    while (1) {

        // make deck and players
        Card deck = make_deck();
        Card dealer = seat_player();
        Card player = seat_player();

        // start game
        game_init(&deck, &player, &dealer);

        // players turn
        player_turn(&deck, &player);
        dealer_turn(&deck, &dealer);

        // result
        game_result(&player, &dealer);

        // put away trump
        delete_deck(&deck);
        delete_deck(&dealer);
        delete_deck(&player);

        char frag;
        do {
            printf("Continue the game? (y/n): ");
            scanf(" %c", &frag);
            if (frag != 'y' && frag != 'n') {
                printf("Sorry, input `y` or `n`\n");
            }
            printf("\n");
        } while (frag != 'y' && frag != 'n');
        if (frag == 'n') { break; }

    }

    return 0;
}

Card make_deck() {
    // make new deck

    /* 0: Joker
     * other: card of the number
     */
    int card_list[54] = {
            0, 0,
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6,
            7, 7, 7, 7,
            8, 8, 8, 8,
            9, 9, 9, 9,
            10, 10, 10, 10,
            11, 11, 11, 11,
            12, 12, 12, 12,
            13, 13, 13, 13
    };
    int i = 0;

    Card deck;
    deck.card = -1;
    deck.score = -1;
    deck.next = NULL;
    for (i=0;i<54;i++) {
        append_card(&deck, make_card(card_list[i]));
    }
    return deck;
}

Card seat_player() {
    // make new player

    Card player;
    player.card = -1;
    player.score = -1;
    player.next = NULL;
    return player;
}

void game_init(Card* deck, Card* player, Card* dealer) {
    //prepare the game

    srand((unsigned int) time(NULL));
    int size, i;
    Card *card;
    for (i=0;i<2;i++) {
        size = deck_size(deck);
        card = draw_card(deck, rand()%size);
        append_card(player, card);
    }
    for (i=0;i<2;i++) {
        size = deck_size(deck);
        card = draw_card(deck, rand()%size);
        append_card(dealer, card);
    }
}

Card* make_card(int number) {
    // make a card
    // use in make_deck only

    Card *p = (Card*) malloc(sizeof(Card));
    p->card = number;
    p->next = NULL;

    switch (number) {
        case 0:
            p->score = 5;
            break;

        case 1:
            p->score = 1;
            break;

        case 11:
        case 12:
        case 13:
            p->score = 10;
            break;

        default:
            p->score = number;
            break;
    }
    return p;
}

void append_card(Card* deck, Card* card) {
    // add a card to deck

    if (card->next != NULL) {
        printf("It's not a card\n");
    }
    Card *next;
    Card *prev;
    prev = deck;
    next = deck->next;
    while (next != NULL) {
        prev = next;
        next = prev->next;
    }
    prev->next = card;
}

Card* draw_card(Card* deck, int offset) {
    // draw a card to deck
    int i = 0;
    Card* prev;
    Card* card;
    prev = deck;
    card = deck->next;
    for(i=0;i<offset;i++) {
        if(card == NULL) {
            printf("invalid offset");
            return NULL;
        }
        prev = card;
        card = card->next;
    }
    prev->next = card->next;
    card->next = NULL;
    return card;
}

void print_deck(Card* deck) {
    // print players deck

    Card *card;
    int i = 0;
    for (card=deck->next;card!=NULL;card=card->next) {
        printf("%d: ", i);
        switch (card->card) {
            case 0:
                printf("Joker");
                break;

            case 1:
                printf("Ace");
                break;

            case 11:
                printf("Jack");
                break;

            case 12:
                printf("Queen");
                break;

            case 13:
                printf("King");
                break;

            default:
                printf("%d", card->card);
                break;
        }
        printf(" (%d)\n", card->score);
        i++;
    }
}

int deck_size(Card* deck) {
    // this function return the deck size

    int i = 0;
    Card* p;
    for (p=deck->next;p!=NULL;p=p->next) {
        i++;
    }
    return i;
}

int total_score(Card* deck) {
    // this function return the total score of player

    int total = 0;
    Card* p;
    for (p=deck->next;p!=NULL;p=p->next) {
        total += p->score;
    }
    return total;
}

void player_turn(Card* deck, Card* player) {
    // player's turn

    char frag;
    Card* card;
    int size, card_num, score;

    do {
        score = total_score(player);
        printf("Your hand --------\n");
        print_deck(player);
        printf("\n");
        printf("Your score: %d\n", score);
        printf("------------------\n\n");
        if (score > 21) { break; }

        printf("Hit? Stand? Change the score of card (Ace or Joker)? (h/s/c): ");
        scanf(" %c", &frag);
        switch (frag) {
            case 'h':
                size = deck_size(deck);
                card = draw_card(deck, rand()%size);
                append_card(player, card);
                break;

            case 'c':
                printf("Input card number(0-%d): ", deck_size(player)-1);
                scanf("%d", &card_num);
                change_score(player, card_num);
                break;

            case 's':
                break;

            default:
                printf("Sorry, input `h`, `s` or `c`\n");
                break;
        }
        printf("\n");
    } while (frag != 's');
}

void dealer_turn(Card *deck, Card *dealer) {
    // dealer's turn

    int score = total_score(dealer);
    while (score < 17) {
        int size = deck_size(deck);
        Card *card = draw_card(deck, rand()%size);
        append_card(dealer, card);
        score = total_score(dealer);
    }
}

void game_result(Card *player, Card *dealer) {
    // result the game

    int p_score = total_score(player);
    int d_score = total_score(dealer);
    printf("Result -----------\n");
    printf("player's score: %d\n", p_score);
    printf("dealer's score: %d\n", d_score);
    printf("\n");
    if (p_score > 21) {
        printf("Oops! You bust!\n");
        printf("You lose...\n");
    } else if (d_score > 21) {
        printf("Wow! Dealer bust!\n");
        printf("You win!!\n");
    } else if (p_score > d_score) {
        printf("You win!!\n");
    } else if (p_score < d_score) {
        printf("You lose...\n");
    } else {
        printf("It's a drawn game\n");
    }
    printf("------------------\n");
    printf("\n");
}

void change_score(Card *player, int offset) {
    // change score of Ace or Joker

    Card *card = player->next;
    int i, number;
    for (i=0;i<offset;i++) {
        card = card->next;
    }
    number = card->card;
    switch (number) {
        case 0:
            if (card->score == 5) {
                card->score = 10;
            } else {
                card->score = 5;
            }
            break;

        case 1:
            if (card->score == 1) {
                card->score = 11;
            } else {
                card->score = 1;
            }
            break;

        default:
            printf("Can't change this card\n\n");
            break;

    }
}

void delete_deck(Card *deck) {
    // delete a deck

    while (deck->next != NULL) {
        Card *prev;
        Card *p;
        prev = deck;
        p = prev->next;
        while (p->next != NULL) {
            prev = p;
            p = prev->next;
        }
        free(p);
        prev->next = NULL;
    }
}
