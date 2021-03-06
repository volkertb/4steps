#ifndef MOVETREE_HPP
#define MOVETREE_HPP

#include "gamestate.hpp"

struct MoveTree {
  MoveTree* previousNode;
  GameState currentState;
  Result result;
  std::list<std::pair<ExtendedSteps,MoveTree> > branches;

  explicit MoveTree(const GameState& currentState_=GameState(),MoveTree* const previousHistory=nullptr);
  explicit MoveTree(const MoveTree& moveTree);
  MoveTree& operator=(const MoveTree& rhs);
  bool operator==(const MoveTree& rhs) const;
  unsigned int numDescendants() const;
  std::vector<GameState> history() const;
  bool changedSquare(const SquareIndex square) const;
  MoveLegality legalMove(const GameState& resultingState) const;
  bool hasLegalMoves(const GameState& startingState) const;
  void detectGameEnd();
  MoveTree& makeMove(const ExtendedSteps& move);
};

#endif // MOVETREE_HPP
