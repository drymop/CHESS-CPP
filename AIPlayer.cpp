#include <stdio.h>

#include "AIPlayer.h"

AIPlayer::AIPlayer (Board* brd, BoardGUI* brdgui, int difficulty) {
  b = brd;
  bgui = brdgui;
  maxDepth = difficulty;
}

bool AIPlayer::isHuman() {
  return false;
}

void AIPlayer::saveBoard() {
  bSave = *b;
}

bool AIPlayer::isBoardDifferent() {
  return b->isDifferent(bSave);
}

int AIPlayer::decideMove() {
   saveBoard(); // uncomment if want to find bugs in board or AI
  // numNodes = 0; // uncomment to check pruning's effect (see how many nodes the AI evaluates)

  int color = b->getPlayer()? -1 : 1;

  int bestMove = -1;
  int alpha = -2 * MATE_VALUE;
  int beta = 2 * MATE_VALUE;
  int numMoves = b->getNumMoves();

  // reorder moves
  int moveOrder[numMoves];
  reorderMoves(moveOrder, numMoves, color);

  // Find the sub-tree with best value
  // Instead of calling negamax(maxDepth, alpha, beta, color), the first ply moves are search separately
  // because after evaluating each move, the piled up GUI events need to be handled.
  for (int m = 0; m < numMoves; m++) {
    b->makeMove(moveOrder[m]);
    // get the value of the sub-tree
    int val = -negamax(maxDepth-1, -beta, -alpha, -color);
    b->undoMove();
    // update the best value and cut off values
    if (val > alpha) {
      alpha = val;
      bestMove = moveOrder[m];
      if (alpha >= beta) break;
    }
    // handle GUI events: if user quit, immediately
    if (GUI::quit || bgui->getInput() == BoardGUI::INPUT_HOME) break;
  }
  // uncomment to check for board or AI bugs.
  if (isBoardDifferent()) {
    GUI::quit = true;
    return -1;
  }
  return bestMove;
}

int AIPlayer::negamax(int depth, int alpha, int beta, int color) {
  //numNodes++; // uncomment to check pruning performance
  int numMoves = b->getNumMoves();

  //////////////////////////////////////////////////////////////////
  // If reaches cut-off depth or a terminal node (end game node),
  // evaluate the board at that node and return the board's score.
  //////////////////////////////////////////////////////////////////
  if (depth <= 0 || numMoves == 0) {
    // get board's score for the player to move (color adjusts the sign of board's score)
    int score = color * heuristicEval();
    // try to win early or lose late by adding or subtracting depth.
    if (score > 0) {
      score = score + depth;
    } else if (score < 0) {
      score = score - depth;
    }
    return score;
  }

  //////////////////////////////////////////////////////////////////
  // If node is not a terminal node,
  // evaluate each sub-tree and return the best value
  //////////////////////////////////////////////////////////////////

  // reorder move so that the first few best moves are searched first
  int moveOrder[numMoves];
  reorderMoves(moveOrder, numMoves, color);

  for (int m = 0; m < numMoves; m++) {
    b->makeMove(moveOrder[m]);
    int val = -negamax(depth-1, -beta, -alpha, -color);
    b->undoMove();
    if (val > alpha) {
      alpha = val;
      if (alpha >= beta) break;
    }
  }
  return alpha;
}

const int AIPlayer::NUM_BEST_MOVES = 6;

void AIPlayer::reorderMoves(int *moveOrder, int numMoves, int color) {
  // Find the 1-ply score of each move
  int score[numMoves];
  for(int m = 0; m < numMoves; m++) {
    b->makeMove(m);
    score[m] = color * heuristicEval();
    b->undoMove();
  }

  // In case there are less than NUM_BEST_MOVES moves available
  int numOrder = (numMoves > NUM_BEST_MOVES)? NUM_BEST_MOVES : numMoves;
  int n; //the index in moveOrder

  // Put the best moves in the first indexes of moveOrder array by
  // first finding the best scores, then turning it to -infinity.
  // Repeat this NUM_BEST_MOVES times
  for (n = 0; n < numOrder; n++) {
    int bestScore = score[0];
    int bestIndex = 0;
    for (int m = 1; m < numMoves; m++) {
      if (score[m] > bestScore) {
        bestScore = score[m];
        bestIndex = m;
      }
    }
    moveOrder[n] = bestIndex;
    score[bestIndex] = -2*MATE_VALUE;
  }
  // Put the rest of the moves in the moveOrder array
  for (int m = 0; m < numMoves; m++) {
    if (score[m] != -2*MATE_VALUE) {
      moveOrder[n] = m;
      n++;
    }
  }
}

const int AIPlayer::pieceValues[6] = {900, 0, 500, 320, 330, 100};
const int AIPlayer::MATE_VALUE = 9 * pieceValues[Board::BQ]
                               + 2 * pieceValues[Board::BR]
                               + 2 * pieceValues[Board::BK]
                               + 2 * pieceValues[Board::BB]
                               + 8 * pieceValues[Board::BP];
const int AIPlayer::positionValues[7][64] = {
  { // queen
		-20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
	},
	{ //king
		-30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,-10,-10,-10,-10, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
	},
	{ //rook
		 0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     0,  0,  0,  5,  5,  0,  0,  0
	},
	{ //knight
		-50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
	},
	{ //bishop
		-20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
	},
	{ //pawn
		 0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
	},
	{ //king end game
		-50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50,
	}
};
const int AIPlayer::END_GAME_MATERIAL = 3500;

int AIPlayer::heuristicEval() {
  //////////////////////////////////////////////////////////////////
  //                          End board
  //////////////////////////////////////////////////////////////////
  if (b->getNumMoves() == 0) {
    int winner = b->getWinner();
    switch (winner) {
      case Board::WHITE      : return MATE_VALUE;  // white wins
      case Board::BLACK      : return -MATE_VALUE; // black wins
      case Board::BOTH_COLOR : return 0;           // draw
    }
  }

  //////////////////////////////////////////////////////////////////
  //                       Not end board
  //////////////////////////////////////////////////////////////////

  int score = 0; //the score of the board for white
  int material = 0;
  int piece;
  // go through all squares and evaluate each square
  // using pieceValues and positionalValues arrays
  for (int s = 0; s < Board::NUM_SQUARES; s++) {
    piece = b->getPiece(s);
    if (piece == Board::EMPTY) { //empty square
      continue;
    } else if (piece > Board::MAX_BLACK_TYPE) { // square has white piece
      //leave the king for last
      // because during late game, king has different positional value
      if (piece == Board::WK) continue;

      piece -= Board::MIN_WHITE_TYPE; //get the type of piece without color
      score += pieceValues[piece];
      material += pieceValues[piece];
      score += positionValues[piece][56 + 2* (s % Board::COLS) - s]; //flips positionValues
    }
    else { // square has black piece
      if (piece == Board::BK) continue;

      score -= pieceValues[piece];
      material += pieceValues[piece];
      score -= positionValues[piece][s];
    }
  }
  if (material > END_GAME_MATERIAL) { // early game
    int kingSq = b->getKingSquare(Board::WHITE); //white king
    score += positionValues[Board::BK][56 + 2* (kingSq % Board::COLS) - kingSq];
    kingSq = b->getKingSquare(Board::BLACK); //black king
    score -= positionValues[Board::BK][kingSq];
  } else { // late game
    int kingSq = b->getKingSquare(Board::WHITE);
    score += positionValues[6][56 + 2* (kingSq % Board::COLS) - kingSq];
    kingSq = b->getKingSquare(Board::BLACK);
    score -= positionValues[6][kingSq];
  }
  return score;
}
