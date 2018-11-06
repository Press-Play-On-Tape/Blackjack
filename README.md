# Blackjack
<img src="/assets/BlackJack Banner.png" data-canonical-src="/assets/BlackJack Banner.png" width="700" height="257" />

This implementation plays a ‘unified’ version of the various BlackJack games from different casinos around the world. It allows splits, double up and insurance on a dealer blackjack and so forth. Initially it will be a one-player game (play against the house) but version 2 might allow two players.


## Rules

The rules are pretty simple.

* You nominate your starting bet. In this game anywhere from $1 to $200.
* You and the dealer are then dealt two cards. You can see your hand and you can see the dealers top card. At this point and depending on the cards showing you have a number of choices:
  * If you have a pair you can split them into two hands. You have to double your bet to do this but you can potentially win twice as much. Typically you should split Aces and Eights but never 10s (or picture cards).
  * You can double up. You double your bet and are dealt a single card (at right angles to your normal cards). This is the end of your hand. Best to double up when your had equals 10 or 11.
  * If the dealer has an Ace showing you will be offered an insurance bet. This is a side bet and will pay out if the dealer has BlackJack. An insurance bet effectively offsets you original bet - you win money because the dealer has a Blackjack but you lose on the main hand (unless you too have a blackjack).
* You then start playing your first hand. You can draw cards to get your hand score as close to 21 as you can without going bust. Aces can be counted as 1 or 11 - your choice.
* You play your second hand (if you have one).
* Dealer plays his hand and works out who has won.
  * If you have a black jack, the Dealer pays you out 5:2 (two and a half times your original bet).
  * If you have a non-black jack hand that is better than the dealers, he pays you 2:1 (twice your original bet).
  * If you and the dealer have the same, you get your money back
  * If the dealer has a better hand you lose!
