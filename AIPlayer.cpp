#include "AIPlayer.h"
#include <stdio.h>

AIPlayer::AIPlayer(Board* brd, BoardGUI* brdgui, int difficulty)
{
  b = brd;
  bgui = brdgui;
  lookAhead = difficulty;
}

void AIPlayer::saveBoard()
{
  bSave = *b;
}

bool AIPlayer::isBoardDifferent()
{
  return b->isDifferent(bSave);
}

void AIPlayer::decideMove()
{
  saveBoard();
  bestMove = -1;
  numNodes = 0;
  int color = (b->getPlayer() == 0)? 1 : -1;
  negamax(lookAhead, -MATE_VALUE, MATE_VALUE, color);
  if (isBoardDifferent())
  {
    GUI::quit = true;
    return;
  }
  printf("Looked through %i nodes\n", numNodes);
  /*if (GUI::quit)
  {
    return;
  }*/
  printf("AI Player best move is %i\n", bestMove);
  b->makeMove(bestMove);
}

int AIPlayer::negamax(int depth, int alpha, int beta, int color)
{
  numNodes++;
  /*
   * If out of depth or terminal node
   */
  int numMoves = b->getNumMoves();
  if (depth == 0 || numMoves == 0)
  {
    return color * heuristicEval(depth);
  }

  /*
   * Evaluate each branch
   */
  int bestVal = -MATE_VALUE;
  for (int m = 0; m < numMoves; m++)
  {
    //printf("Depth %i, move %i\n", depth, m);
    b->makeMove(m);
    int val = -negamax(depth-1, -beta, -alpha, -color);
    b->undoMove();
    if (val > bestVal)
    {
      bestVal = val;
      if (depth == lookAhead) bestMove = m;
    }
    if (val > alpha) alpha = val;
    if (alpha >= beta) break;
  }
  return bestVal;
}

const int AIPlayer::MATE_VALUE = 10000;
const int AIPlayer::pieceValues[6] = {900, 0, 500, 320, 330, 100};
const int AIPlayer::positionValues[6][8][8] =
  {{ // queen
		{-20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{ -5,  0,  5,  5,  5,  5,  0, -5},
		{ -5,  0,  5,  5,  5,  5,  0, -5},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20},
	},
	{ //king
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-20,-30,-30,-40,-40,-30,-30,-20},
		{-10,-20,-20,-20,-20,-20,-20,-10},
		{ 20, 20,  0,  0,  0,  0, 20, 20},
		{ 20, 30, 10,  0,  0, 10, 30, 20},
	},
	{ //rook
		{  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10, 10, 10, 10, 10,  5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{  0,  0,  0,  5,  5,  0,  0,  0},
	},
	{ //knight
		{-50,-40,-30,-30,-30,-30,-40,-50},
		{-40,-20,  0,  5,  5,  0,-20,-40},
		{-30,  5, 10, 15, 15, 10,  5,-30},
		{-30,  0, 15, 20, 20, 15,  0,-30},
		{-30,  5, 15, 20, 20, 15,  5,-30},
		{-30,  0, 10, 15, 15, 10,  0,-30},
		{-40,-20,  0,  0,  0,  0,-20,-40},
		{-50,-40,-30,-30,-30,-30,-40,-50},
	},
	{ //bishop
		{-20,-10,-10,-10,-10,-10,-10,-20},
		{-10,  5,  0,  0,  0,  0,  5,-10},
		{-10, 10, 10, 10, 10, 10, 10,-10},
		{-10,  0, 10, 10, 10, 10,  0,-10},
		{-10,  5,  5, 10, 10,  5,  5,-10},
		{-10,  0,  5, 10, 10,  5,  0,-10},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-20,-10,-10,-10,-10,-10,-10,-20},
	},
	{ //pawn
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{50, 50, 50, 50, 50, 50, 50, 50},
		{10, 10, 20, 30, 30, 20, 10, 10},
		{ 5,  5, 10, 25, 25, 10,  5,  5},
		{ 0,  0,  0, 20, 20,  0,  0,  0},
		{ 5, -5,-10,  0,  0,-10, -5,  5},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 5, 10, 10,-20,-20, 10, 10,  5},
	}};

int AIPlayer::heuristicEval(int depth)
{
  if (b->getNumMoves() == 0)
  {
    int winner = b->getWinner();
    switch (winner)
    {
      case 0: return MATE_VALUE - depth;
      case 1: return -MATE_VALUE + depth;
      case 2: return 0;
    }
  }
  int score = 0;
  int piece;
  for (int s = 0; s < 64; s++)
  {
    piece = b->getPiece(s);
    if (piece == -1) continue;
    if (piece > 5) // white piece
    {
      score += pieceValues[piece - 6];
      score += positionValues[piece-6][7 - s/8][s%8];
    }
    else // black piece
    {
      score -= pieceValues[piece];
      score -= positionValues[piece][s/8][s%8];
    }
  }
  return score;
}
