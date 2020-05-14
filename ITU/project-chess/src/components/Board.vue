<!-- This component was created by Peter Horňák (xhorna14) as part of project to subject ITU -->

<template>
  <div class="container-fluid">
    <div class="row">
      <div class="col ml-3">
        <br />
        <br />
        <br />
        <GameSettingsTab></GameSettingsTab>
      </div>
      <div class="col">
        <div id="board">
          <div v-for="(block, x) in blocks" :key="x">
            <component
              :is="block.myComp"
              :color="block.color"
              :pos="x"
            ></component>
          </div>
        </div>
      </div>
      <div class="col">
        <br />
        <br />
        <div class="row mb-5">
          <GameHistoryTab></GameHistoryTab>
        </div>
        <div class="row mb-5">
          <br />
          <br />
          <br />
          <br />
        </div>
        <div class="row mt-5">
          <ButtonsTab></ButtonsTab>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
const defaultBlocks = [
  {
    a8: { color: "BurlyWood", myComp: ChessBlock },
    b8: { color: "SaddleBrown", myComp: ChessBlock },
    c8: { color: "BurlyWood", myComp: ChessBlock },
    d8: { color: "SaddleBrown", myComp: ChessBlock },
    e8: { color: "BurlyWood", myComp: ChessBlock },
    f8: { color: "SaddleBrown", myComp: ChessBlock },
    g8: { color: "BurlyWood", myComp: ChessBlock },
    h8: { color: "SaddleBrown", myComp: ChessBlock },
    a7: { color: "SaddleBrown", myComp: ChessBlock },
    b7: { color: "BurlyWood", myComp: ChessBlock },
    c7: { color: "SaddleBrown", myComp: ChessBlock },
    d7: { color: "BurlyWood", myComp: ChessBlock },
    e7: { color: "SaddleBrown", myComp: ChessBlock },
    f7: { color: "BurlyWood", myComp: ChessBlock },
    g7: { color: "SaddleBrown", myComp: ChessBlock },
    h7: { color: "BurlyWood", myComp: ChessBlock },
    a6: { color: "BurlyWood", myComp: ChessBlock },
    b6: { color: "SaddleBrown", myComp: ChessBlock },
    c6: { color: "BurlyWood", myComp: ChessBlock },
    d6: { color: "SaddleBrown", myComp: ChessBlock },
    e6: { color: "BurlyWood", myComp: ChessBlock },
    f6: { color: "SaddleBrown", myComp: ChessBlock },
    g6: { color: "BurlyWood", myComp: ChessBlock },
    h6: { color: "SaddleBrown", myComp: ChessBlock },
    a5: { color: "SaddleBrown", myComp: ChessBlock },
    b5: { color: "BurlyWood", myComp: ChessBlock },
    c5: { color: "SaddleBrown", myComp: ChessBlock },
    d5: { color: "BurlyWood", myComp: ChessBlock },
    e5: { color: "SaddleBrown", myComp: ChessBlock },
    f5: { color: "BurlyWood", myComp: ChessBlock },
    g5: { color: "SaddleBrown", myComp: ChessBlock },
    h5: { color: "BurlyWood", myComp: ChessBlock },
    a4: { color: "BurlyWood", myComp: ChessBlock },
    b4: { color: "SaddleBrown", myComp: ChessBlock },
    c4: { color: "BurlyWood", myComp: ChessBlock },
    d4: { color: "SaddleBrown", myComp: ChessBlock },
    e4: { color: "BurlyWood", myComp: ChessBlock },
    f4: { color: "SaddleBrown", myComp: ChessBlock },
    g4: { color: "BurlyWood", myComp: ChessBlock },
    h4: { color: "SaddleBrown", myComp: ChessBlock },
    a3: { color: "SaddleBrown", myComp: ChessBlock },
    b3: { color: "BurlyWood", myComp: ChessBlock },
    c3: { color: "SaddleBrown", myComp: ChessBlock },
    d3: { color: "BurlyWood", myComp: ChessBlock },
    e3: { color: "SaddleBrown", myComp: ChessBlock },
    f3: { color: "BurlyWood", myComp: ChessBlock },
    g3: { color: "SaddleBrown", myComp: ChessBlock },
    h3: { color: "BurlyWood", myComp: ChessBlock },
    a2: { color: "BurlyWood", myComp: ChessBlock },
    b2: { color: "SaddleBrown", myComp: ChessBlock },
    c2: { color: "BurlyWood", myComp: ChessBlock },
    d2: { color: "SaddleBrown", myComp: ChessBlock },
    e2: { color: "BurlyWood", myComp: ChessBlock },
    f2: { color: "SaddleBrown", myComp: ChessBlock },
    g2: { color: "BurlyWood", myComp: ChessBlock },
    h2: { color: "SaddleBrown", myComp: ChessBlock },
    a1: { color: "SaddleBrown", myComp: ChessBlock },
    b1: { color: "BurlyWood", myComp: ChessBlock },
    c1: { color: "SaddleBrown", myComp: ChessBlock },
    d1: { color: "BurlyWood", myComp: ChessBlock },
    e1: { color: "SaddleBrown", myComp: ChessBlock },
    f1: { color: "BurlyWood", myComp: ChessBlock },
    g1: { color: "SaddleBrown", myComp: ChessBlock },
    h1: { color: "BurlyWood", myComp: ChessBlock }
  },
  {
    a1: { color: "BurlyWood", myComp: ChessBlock },
    b1: { color: "SaddleBrown", myComp: ChessBlock },
    c1: { color: "BurlyWood", myComp: ChessBlock },
    d1: { color: "SaddleBrown", myComp: ChessBlock },
    e1: { color: "BurlyWood", myComp: ChessBlock },
    f1: { color: "SaddleBrown", myComp: ChessBlock },
    g1: { color: "BurlyWood", myComp: ChessBlock },
    h1: { color: "SaddleBrown", myComp: ChessBlock },
    a2: { color: "SaddleBrown", myComp: ChessBlock },
    b2: { color: "BurlyWood", myComp: ChessBlock },
    c2: { color: "SaddleBrown", myComp: ChessBlock },
    d2: { color: "BurlyWood", myComp: ChessBlock },
    e2: { color: "SaddleBrown", myComp: ChessBlock },
    f2: { color: "BurlyWood", myComp: ChessBlock },
    g2: { color: "SaddleBrown", myComp: ChessBlock },
    h2: { color: "BurlyWood", myComp: ChessBlock },
    a3: { color: "BurlyWood", myComp: ChessBlock },
    b3: { color: "SaddleBrown", myComp: ChessBlock },
    c3: { color: "BurlyWood", myComp: ChessBlock },
    d3: { color: "SaddleBrown", myComp: ChessBlock },
    e3: { color: "BurlyWood", myComp: ChessBlock },
    f3: { color: "SaddleBrown", myComp: ChessBlock },
    g3: { color: "BurlyWood", myComp: ChessBlock },
    h3: { color: "SaddleBrown", myComp: ChessBlock },
    a4: { color: "SaddleBrown", myComp: ChessBlock },
    b4: { color: "BurlyWood", myComp: ChessBlock },
    c4: { color: "SaddleBrown", myComp: ChessBlock },
    d4: { color: "BurlyWood", myComp: ChessBlock },
    e4: { color: "SaddleBrown", myComp: ChessBlock },
    f4: { color: "BurlyWood", myComp: ChessBlock },
    g4: { color: "SaddleBrown", myComp: ChessBlock },
    h4: { color: "BurlyWood", myComp: ChessBlock },
    a5: { color: "BurlyWood", myComp: ChessBlock },
    b5: { color: "SaddleBrown", myComp: ChessBlock },
    c5: { color: "BurlyWood", myComp: ChessBlock },
    d5: { color: "SaddleBrown", myComp: ChessBlock },
    e5: { color: "BurlyWood", myComp: ChessBlock },
    f5: { color: "SaddleBrown", myComp: ChessBlock },
    g5: { color: "BurlyWood", myComp: ChessBlock },
    h5: { color: "SaddleBrown", myComp: ChessBlock },
    a6: { color: "SaddleBrown", myComp: ChessBlock },
    b6: { color: "BurlyWood", myComp: ChessBlock },
    c6: { color: "SaddleBrown", myComp: ChessBlock },
    d6: { color: "BurlyWood", myComp: ChessBlock },
    e6: { color: "SaddleBrown", myComp: ChessBlock },
    f6: { color: "BurlyWood", myComp: ChessBlock },
    g6: { color: "SaddleBrown", myComp: ChessBlock },
    h6: { color: "BurlyWood", myComp: ChessBlock },
    a7: { color: "BurlyWood", myComp: ChessBlock },
    b7: { color: "SaddleBrown", myComp: ChessBlock },
    c7: { color: "BurlyWood", myComp: ChessBlock },
    d7: { color: "SaddleBrown", myComp: ChessBlock },
    e7: { color: "BurlyWood", myComp: ChessBlock },
    f7: { color: "SaddleBrown", myComp: ChessBlock },
    g7: { color: "BurlyWood", myComp: ChessBlock },
    h7: { color: "SaddleBrown", myComp: ChessBlock },
    a8: { color: "SaddleBrown", myComp: ChessBlock },
    b8: { color: "BurlyWood", myComp: ChessBlock },
    c8: { color: "SaddleBrown", myComp: ChessBlock },
    d8: { color: "BurlyWood", myComp: ChessBlock },
    e8: { color: "SaddleBrown", myComp: ChessBlock },
    f8: { color: "BurlyWood", myComp: ChessBlock },
    g8: { color: "SaddleBrown", myComp: ChessBlock },
    h8: { color: "BurlyWood", myComp: ChessBlock }
  }
];

import ChessBlock from "./ChessBlock.vue";
import GameHistoryTab from "./GameHistoryTab.vue";
import ButtonsTab from "./ButtonsTab.vue";
import GameSettingsTab from "./GameSettingsTab.vue";
import Chess from "chess.js";
import { p4_new_game } from "../engine";

export default {
  name: "board",
  components: {
    ChessBlock,
    GameHistoryTab,
    ButtonsTab,
    GameSettingsTab
  },
  methods: {
    doUndo() {
      this.$store.commit("undoMove");
    },
    getHint() {
      this.$store.commit("findBestMoves", true);
    },
    doRestart() {
      this.chess = new Chess();
      this.engine = p4_new_game();
      this.$store.commit("initChess", {
        chess: this.chess,
        engine: this.engine
      });
      if (this.startingColor === "1") {
        const vm = this;
        setTimeout(function() {
          vm.$store.commit("makeAIMove");
        }, 1000);
      }
    },
    changeSide() {
      if (this.startingColor === "1") {
        this.startingColor = "0";
        this.blocks = defaultBlocks[0];
      } else {
        this.startingColor = "1";
        this.blocks = defaultBlocks[1];
      }
      this.doRestart();
    }
  },
  created: function() {
    this.chess = new Chess();
    this.engine = p4_new_game();
    this.$store.commit("initChess", { chess: this.chess, engine: this.engine });
    this.blocks = defaultBlocks[this.startingColor];
    this.$store.commit("findBestMoves", false);
  },
  mounted: function() {
    if (this.startingColor === "1") {
      const vm = this;
      setTimeout(function() {
        vm.$store.commit("makeAIMove");
      }, 1000);
    }
  },
  data() {
    return {
      chess: null,
      engine: null,
      blocks: {},
      startingColor: 0
    };
  }
};
</script>

<style lang="scss">
#board {
  padding: 25px;
  width: 690px;
  height: 690px;
  margin: auto;
}
</style>
