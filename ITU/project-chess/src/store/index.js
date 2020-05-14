// This component was created by Peter Horňák (xhorna14) as part of project to subject ITU

import Vue from "vue";
import Vuex from "vuex";

Vue.use(Vuex);

function makeMoveFromNumber(moveArr) {
  let firstBlock = moveArr[0].toString();
  let secondBlock = moveArr[1].toString();
  const map = {
    1: "a",
    2: "b",
    3: "c",
    4: "d",
    5: "e",
    6: "f",
    7: "g",
    8: "h"
  };
  firstBlock = map[firstBlock[1]] + parseInt(firstBlock[0] - 1);
  secondBlock = map[secondBlock[1]] + parseInt(secondBlock[0] - 1);
  return [firstBlock, secondBlock];
}

export default new Vuex.Store({
  state: {
    blockIsClicked: false,
    firstClickedBlock: "",
    secondClickedBlock: "",
    secondClickedMove: "",
    chessHistory: null,
    availableMoves: [],
    bestMoves: [],
    bestMove: null,
    lastAIMove: [],
    stateChess: null,
    engineState: null,
    updatedBoard: 0
  },
  mutations: {
    undoMove(state) {
      if (state.updatedBoard < 2) {
        return;
      }
      state.stateChess.undo();
      state.stateChess.undo();
      state.engineState.jump_to_moveno(state.updatedBoard - 2);
      state.updatedBoard--;
    },
    initChess(state, initVals) {
      state.blockIsClicked = false;
      state.firstClickedBlock = "";
      state.secondClickedBlock = "";
      state.secondClickedMove = "";
      state.chessHistory = null;
      state.availableMoves = [];
      state.bestMove = null;
      state.lastAIMove = [];
      state.stateChess = null;
      state.engineState = null;
      state.updatedBoard = 0;
      state.stateChess = initVals["chess"];
      state.engineState = initVals["engine"];
      state.updatedBoard = 0;
    },
    firstClick(state, blockVal) {
      state.blockIsClicked = true;
      state.firstClickedBlock = blockVal;
      state.availableMoves = state.stateChess.moves({ square: blockVal });
      state.secondClickedBlock = "";
      state.secondClickedMove = null;
    },
    secondClick(state, blockVals) {
      state.secondClickedMove = blockVals.blockMove;
      state.secondClickedBlock = blockVals.blockVal;
      state.blockIsClicked = false;
    },
    makeMove(state) {
      if (state.availableMoves !== []) {
        state.stateChess.move(state.secondClickedMove);
        state.engineState.move(state.secondClickedMove);
        state.availableMoves = [];
        // state.bestMoves = [];
        state.updatedBoard++;
        state.chessHistory = state.stateChess.history({ verbose: true });
      }
    },
    makeAIMove(state) {
      const level = Math.round(Math.random() * (5 - 1) + 1);
      const nextMove = state.engineState.findmove(level);
      const moves = makeMoveFromNumber(nextMove);
      state.stateChess.move({ from: moves[0], to: moves[1] });
      state.engineState.move(moves[0], moves[1]);
      state.lastAIMove = moves;
      state.updatedBoard++;
      state.chessHistory = state.stateChess.history({ verbose: true });
    },
    findBestMoves(state, bestOnly) {
      if (bestOnly === true) {
        if (state.bestMove !== null) {
          return;
        }
        state.bestMove = makeMoveFromNumber(state.engineState.findmove(5));
        state.bestMoves[state.bestMoves.length] = state.bestMove;
        return;
      }
      if (state.bestMoves.length > 30) {
        return;
      }
      for (let i = 0; i < 5; i++) {
        state.bestMoves[state.bestMoves.length] = makeMoveFromNumber(
          state.engineState.findmove(i)
        );
      }
    },
    clearBestMove(state) {
      state.bestMove = null;
    },
    clearAvailable(state) {
      state.availableMoves = [];
    }
  },
  actions: {},
  modules: {}
});
