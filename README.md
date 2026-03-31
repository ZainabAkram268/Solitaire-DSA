# Solitaire Card Game

A console-based Solitaire implementation in C++ using **Doubly Linked Lists**, **Iterators**, and **Stacks**.

## Data Structures Used

| Structure | Purpose |
|-----------|---------|
| Doubly Linked List | Tableau columns (7 columns) |
| Iterator | Traverse and extract card sequences |
| Stack | Foundation piles, Stockpile, Wastepile, Undo history |

## Features

- Complete Solitaire rules (alternating colors, descending ranks)
- Draw from stockpile to wastepile
- Move cards between tableau, foundation, and wastepile
- Move multiple cards at once
- Undo support (`z` command)
- Colored console output (red for hearts/diamonds)
- Win detection

## Commands

| Command | Description |
|---------|-------------|
| `s` | Draw card from stockpile |
| `m c1, c2, 2` | Move 2 cards from column1 to column2 |
| `m w, c1, 1` | Move 1 card from wastepile to column1 |
| `m c1, f1, 1` | Move 1 card from column1 to foundation1 |
| `z` | Undo last move |
| `quit` | Exit game |

## Setup & Run

```bash
g++ Solitaire.cpp -o solitaire -std=c++17
./solitaire
Game Rules
Tableau: Descending ranks, alternating colors. Empty columns take only Kings.

Foundation: Same suit, Ace to King.

Stockpile: Draw cards to wastepile. Only top wastepile card can be played.

Author
Zainab Akram | FAST-NUCES
