#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

class Player
{
public:
    int bet;
    int total = 0;
    bool stand = false;
    std::vector<std::string> cards;

    void add_card(std::string card, int number)
    {
        cards.push_back(card);
        total += number;
    }

    void adjust_for_ace()
    {
        for (const auto &card : cards)
        {
            if (total > 21 && card[1] == 'A')
            {
                total -= 10;
            }
        }
    }

    void reset()
    {
        total = 0;
        stand = false;
        cards.clear();
    }
};

class Dealer
{
public:
    int total = 0;
    bool stand = false;
    std::vector<std::string> cards;

    void add_card(std::string card, int number)
    {
        cards.push_back(card);
        total += number;
    }

    void adjust_for_ace()
    {
        for (const auto &card : cards)
        {
            if (total > 21 && card[1] == 'A')
            {
                total -= 10;
            }
        }
    }

    void reset()
    {
        total = 0;
        stand = false;
        cards.clear();
    }
};

std::string deck[52] = {
    "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10", "CJ", "CQ", "CK", "CA",
    "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "D10", "DJ", "DQ", "DK", "DA",
    "H2", "H3", "H4", "H5", "H6", "H7", "H8", "H9", "H10", "HJ", "HQ", "HK", "HA",
    "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "SJ", "SQ", "SK", "SA"};

int deck_size = sizeof(deck) / sizeof(deck[0]);

std::string get_card()
{
    return deck[rand() % deck_size];
}

int get_card_number(std::string card)
{
    if (isdigit(card[1]))
    {
        return std::stoi(card.substr(1));
    }
    else
    {
        if (card[1] == 'J' || card[1] == 'Q' || card[1] == 'K')
        {
            return 10;
        }
        else
        {
            return 11;
        }
    }
}

void deal_card_to_player(Player &player)
{
    std::string card = get_card();
    player.add_card(card, get_card_number(card));
}

void deal_card_to_dealer(Dealer &dealer)
{
    std::string card = get_card();
    dealer.add_card(card, get_card_number(card));
}

void show_score(std::string who, std::vector<std::string> cards, int total)
{
    std::cout << "\n"
              << who << " Cards: " << std::endl;
    for (const auto &c : cards)
    {
        std::cout << c << " ";
    }
    std::cout << "\nTotal: " << total << std::endl;
}

void win_screen(Player player, Dealer dealer)
{
    if (player.total <= 21 && (player.total > dealer.total || dealer.total > 21))
    {
        if (dealer.total > 21)
        {
            std::cout << "Dealer busts. You win! " << player.bet * 2 << "€\n";
        }
        std::cout << "You win! " << player.bet * 2 << "€\n";
    }
    else
    {
        std::cout << "Dealer wins. You lose " << player.bet << "€\n";
    }
}

int main()
{
    srand(time(NULL));

    Player player;
    Dealer dealer;

    while (true)
    {

        std::cout << "___BLACKJACK___" << std::endl;
        std::cout << "How much do you want to bet? (€)";
        std::cin >> player.bet;

        std::cout << "\n_Dealing Cards_" << std::endl;

        deal_card_to_player(player);
        deal_card_to_player(player);
        deal_card_to_dealer(dealer);

        while ((!player.stand || !dealer.stand) && player.total <= 21 && dealer.total <= 21)
        {
            player.adjust_for_ace();
            dealer.adjust_for_ace();

            show_score("Your", player.cards, player.total);
            show_score("Dealer", dealer.cards, dealer.total);

            if (!player.stand)
            {
                std::cout << "Hit or Stand? (h/s): ";
                char choice;
                std::cin >> choice;
                if (choice == 'h')
                {
                    deal_card_to_player(player);
                    if (player.total > 21)
                    {
                        std::cout << "Bust! You lose.\n";
                        break;
                    }
                }
                else
                {
                    player.stand = true;
                }
            }

            if (!dealer.stand)
            {
                if (dealer.total < 17)
                {
                    deal_card_to_dealer(dealer);
                }
                else
                {
                    dealer.stand = true;
                }
            }
        }

        show_score("Your", player.cards, player.total);
        show_score("Dealer", dealer.cards, dealer.total);

        win_screen(player, dealer);

        char restart_choice;
        std::cout << "\nDo you want to play again? (y/n): ";
        std::cin >> restart_choice;

        if (restart_choice == 'n' || restart_choice == 'N')
        {
            std::cout << "Thank you for playing!\n";
            break;
        }

        player.reset();
        dealer.reset();
    }

    return 0;
}