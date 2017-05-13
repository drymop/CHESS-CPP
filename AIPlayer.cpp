#include <stdio.h>

#include "AIPlayer.h"
#include "PieceType.h"

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
  saveBoard();
  numNodes = 0;

  int color = b->getPlayer()? -1 : 1;

  int bestMove = -1;
  int alpha = -MATE_VALUE - maxDepth;
  int beta = MATE_VALUE + maxDepth;
  int numMoves = b->getNumMoves();

  int moveOrder[numMoves];
  reorderMoves(moveOrder, numMoves, color);
  for (int m = 0; m < numMoves; m++) {
    //b->makeMove(m);
    b->makeMove(moveOrder[m]);
    int val = -negamax(maxDepth-1, -beta, -alpha, -color);
    b->undoMove();
    if (val > alpha) {
      alpha = val;
      //bestMove = m;
      bestMove = moveOrder[m];
      if (alpha >= beta) break;
    }
    bgui->getInput();
    if (GUI::quit) break;
    printf("%i\n", numMoves - m - 1);
  }

  if (isBoardDifferent())
  {
    GUI::quit = true;
    return -1;
  }
  printf("Looked through %i nodes\n", numNodes);
  printf("AI Player best move is %i, value %i\n", bestMove, alpha);
  return bestMove;
}

int AIPlayer::negamax(int depth, int alpha, int beta, int color)
{
  numNodes++;
  /*
   * If out of depth or terminal node
   */
  int numMoves = b->getNumMoves();

  if (depth <= 0 || numMoves == 0) {
    int score = color * heuristicEval();
    if (score > 0) {
      score = score + depth;
    } else if (score < 0) {
      score = score - depth;
    }
    return score;
  }
  /*
   * Evaluate each branch
   */
  int moveOrder[numMoves];
  reorderMoves(moveOrder, numMoves, color);
  for (int m = 0; m < numMoves; m++) {
    b->makeMove(moveOrder[m]);
    //b->makeMove(m);
    int val = -negamax(depth-1, -beta, -alpha, -color);
    b->undoMove();
    if (val > alpha) {
      alpha = val;
      if (alpha >= beta) break;
    }
  }
  return alpha;
}

void AIPlayer::reorderMoves(int *moveOrder, int numMoves, int color) {
  int score[numMoves];
  for(int m = 0; m < numMoves; m++) {
    b->makeMove(m);
    score[m] = color * heuristicEval();
    b->undoMove();
  }

  int numOrder = (numMoves > 6)? 6 : numMoves; // Only order the first 6 best moves
  int n; //the index in moveOrder
  // Put 6 best moves in the first indexes of moveOrder array
  for (n = 0; n < numOrder; n++) {
    // find the best scores, then turn it to -infinity. Do that 6 times
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

const int AIPlayer::MATE_VALUE = 10000;
const int AIPlayer::pieceValues[6] = {900, 0, 500, 320, 330, 100};
const int AIPlayer::positionValues[7][8][8] =
  {{ // queen
		{-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20},
	},
	{ //king
		{-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,-10,-10,-10,-10, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20},
	},
	{ //rook
		{ 0,  0,  0,  0,  0,  0,  0,  0},
    { 5, 10, 10, 10, 10, 10, 10,  5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    { 0,  0,  0,  5,  5,  0,  0,  0},
	},
	{ //knight
		{-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
	},
	{ //bishop
		{-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20},
	},
	{ //pawn
		{ 0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 5,  5, 10, 25, 25, 10,  5,  5},
    { 0,  0,  0, 20, 20,  0,  0,  0},
    { 5, -5,-10,  0,  0,-10, -5,  5},
    { 5, 10, 10,-20,-20, 10, 10,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0},
	},
	{ //king end game
		{-50,-40,-30,-20,-20,-30,-40,-50},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-30,  0,  0,  0,  0,-30,-30},
    {-50,-30,-30,-30,-30,-30,-30,-50},
	}};

int AIPlayer::heuristicEval()
{
  if (b->getNumMoves() == 0)
  {
    int winner = b->getWinner();
    switch (winner)
    {
      case 0: return MATE_VALUE;
      case 1: return -MATE_VALUE;
      case 2: return 0;
    }
  }
  int score = 0;
  int material = 0;
  int piece;
  for (int s = 0; s < 64; s++)
  {
    piece = b->getPiece(s);
    if (piece == -1) continue;
    if (piece > 5) // white piece
    {
      if (piece == WK) continue;
      score += pieceValues[piece - 6];
      material += pieceValues[piece - 6];
      score += positionValues[piece-6][7 - s/8][s%8];
    }
    else // black piece
    {
      if (piece == BK) continue;
      score -= pieceValues[piece];
      material -= pieceValues[piece];
      score -= positionValues[piece][s/8][s%8];
    }
  }
  if (material > 2800) {
    int kingSq = b->getKingSquare(0);
    score += positionValues[piece-6][7- kingSq/8][kingSq%8];
    kingSq = b->getKingSquare(1);
    score -= positionValues[piece][kingSq/8][kingSq%8];
  } else {
    int kingSq = b->getKingSquare(0);
    score += positionValues[6][7- kingSq/8][kingSq%8];
    kingSq = b->getKingSquare(1);
    score -= positionValues[6][kingSq/8][kingSq%8];
  }
  return score;
}
