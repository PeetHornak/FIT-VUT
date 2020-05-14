<!-- This component was created by Peter Horňák (xhorna14) as part of project to subject ITU -->

<template>
  <div @click="onClickMethod()" id="ChessBlock" :style="style">
    <div v-if="iAmAvailable" id="inner" :style="innerStyle"></div>
    <component
      v-if="myPiece"
      :is="myComponent"
      :color="myPiece.color"
    ></component>
  </div>
</template>
<script>
import bPawn from "./bPawn.vue";
import bKing from "./bKing.vue";
import bKnight from "./bKnight.vue";
import bBishop from "./bBishop.vue";
import bQueen from "./bQueen.vue";
import bRook from "./bRook.vue";
import wPawn from "./wPawn.vue";
import wKing from "./wKing.vue";
import wKnight from "./wKnight.vue";
import wBishop from "./wBishop.vue";
import wQueen from "./wQueen.vue";
import wRook from "./wRook.vue";
import { mapState } from "vuex";

export default {
  name: "ChessBlock",
  props: ["color", "pos"],
  components: {
    bPawn,
    bKing,
    bKnight,
    bBishop,
    bQueen,
    bRook,
    wPawn,
    wKing,
    wKnight,
    wBishop,
    wQueen,
    wRook
  },
  data: function() {
    return {
      blockColor: this.color,
      blockPos: this.pos,
      isClicked: false,
      myPiece: null,
      iAmAvailable: false,
      iAmGood: false,
      iWasUsed: false,
      myMove: null,
      map: {
        p: bPawn,
        r: bRook,
        b: bBishop,
        k: bKing,
        n: bKnight,
        q: bQueen,
        P: wPawn,
        R: wRook,
        B: wBishop,
        K: wKing,
        N: wKnight,
        Q: wQueen
      }
    };
  },
  created: function() {
    this.myPiece = this.$store.state.stateChess.get(this.blockPos);
    const color = this.stateChess.turn();
    let numb = 1;
    if (color === "b") {
      numb = 8;
    }
    const map = {
      "O-O": `h${numb}`,
      "O-O-O": `a${numb}`
    };
    for (const moves of this.$store.state.bestMoves) {
      if (moves[1].includes(this.blockPos) || map[moves[1]] === this.blockPos) {
        this.iAmGood = true;
        return;
      }
    }
    this.iAmGood = false;
  },
  computed: {
    style() {
      return {
        "box-shadow": this.isClicked ? "0px 0px 0px 2px black inset" : "",
        background: this.blockColor,
        "z-index": 1
      };
    },
    innerStyle() {
      if (this.iWasUsed) {
        return { background: "rgba(255, 165, 0, 0.4)" };
      }
      if (this.iAmGood) {
        return { background: "rgba(0, 255, 0, 0.4)" };
      } else {
        return { background: "rgba(255, 0, 0, 0.4)" };
      }
    },
    myComponent() {
      if (this.myPiece === null) {
        return null;
      }
      if (this.myPiece.color === "b") {
        return this.map[this.myPiece.type];
      } else {
        return this.map[this.myPiece.type.toUpperCase()];
      }
    },
    somethingClicked() {
      return this.$store.state.blockIsClicked;
    },
    secondClicked() {
      return this.$store.state.secondClickedBlock;
    },
    availableMovesInStore() {
      return this.$store.state.availableMoves;
    },
    goodMovesInStore() {
      return this.$store.state.bestMoves;
    },
    bestMoveInStore() {
      return this.$store.state.bestMove;
    },
    lastAIMoveInStore() {
      return this.$store.state.lastAIMove;
    },
    ...mapState(["stateChess", "updatedBoard"])
  },
  watch: {
    updatedBoard() {
      this.myPiece = this.$store.state.stateChess.get(this.blockPos);
    },
    secondClicked(newVal, oldVal) {
      if (newVal !== "" && oldVal === "") {
        if (
          this.$store.state.firstClickedBlock === this.blockPos &&
          this.isBlockAvailable(newVal)
        ) {
          this.$store.commit("makeMove");
          let vm = this;
          setTimeout(function() {
            vm.$store.commit("makeAIMove");
          }, 300);
          this.$store.commit("findBestMoves", false);
        }
        this.isClicked = false;
      }
    },
    lastAIMoveInStore(newVal) {
      if (newVal[1] === this.blockPos || newVal[0] === this.blockPos) {
        this.iWasUsed = true;
        this.iAmAvailable = true;
      }
    },
    bestMoveInStore(newVal) {
      if (newVal === null) {
        return;
      }
      if (newVal[1] === this.blockPos) {
        this.iAmGood = true;
        this.iAmAvailable = true;
      }
    },
    availableMovesInStore(newVal) {
      if (this.iWasUsed === true) {
        this.iWasUsed = false;
      }
      const color = this.stateChess.turn();
      let numb = 1;
      if (color === "b") {
        numb = 8;
      }
      const map = {
        "O-O": `h${numb}`,
        "O-O-O": `a${numb}`
      };
      for (const move of newVal) {
        if (move.includes(this.blockPos) || map[move] === this.blockPos) {
          this.iAmAvailable = true;
          this.myMove = move;
          return;
        }
      }
      this.iAmAvailable = false;
      this.$store.commit("clearBestMove");
    },
    goodMovesInStore(newVal) {
      const color = this.stateChess.turn();
      let numb = 1;
      if (color === "b") {
        numb = 8;
      }
      const map = {
        "O-O": `h${numb}`,
        "O-O-O": `a${numb}`
      };
      for (const moves of newVal) {
        if (
          moves[1].includes(this.blockPos) ||
          map[moves[1]] === this.blockPos
        ) {
          this.iAmGood = true;
          return;
        }
      }
      this.iAmGood = false;
    }
  },
  methods: {
    isBlockAvailable(blockVal) {
      const color = this.stateChess.turn();
      let numb = 1;
      if (color === "b") {
        numb = 8;
      }
      const map = {
        "O-O": `h${numb}`,
        "O-O-O": `a${numb}`
      };
      for (const move of this.availableMovesInStore) {
        let toCheck = move;
        if (map[move]) {
          toCheck = map[move];
        }
        if (toCheck.includes(blockVal)) {
          return true;
        }
      }
      return false;
    },
    onClickMethod() {
      this.$store.commit("findBestMoves", false);
      if (!this.somethingClicked) {
        if (
          this.myPiece === null ||
          this.stateChess.turn() !== this.myPiece.color
        ) {
          return;
        }
        this.isClicked = true;
        this.$store.commit("firstClick", this.blockPos);
      } else {
        if (!this.iAmAvailable) {
          this.$store.commit("clearAvailable");
        }
        this.$store.commit("secondClick", {
          blockVal: this.blockPos,
          blockMove: this.myMove
        });
      }
    }
  }
};
</script>

<style scoped lang="scss">
#ChessBlock {
  float: left;
  width: 80px;
  height: 80px;
  font-size: 50px;
  text-align: center;
  display: table-cell;
  vertical-align: middle;
}

#inner {
  position: absolute;
  width: 80px;
  height: 80px;
}
</style>
