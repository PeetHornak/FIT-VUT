<!-- This component was created by Martin Koči (xkocim05) as part of project to subject ITU -->
<template>
  <div class="chess-history" style="overflow:scroll;">
    <div class="containter-fluid">
      <div class="row">
        <div class="col text-center">
          <h4 class="mx-center">História</h4>
        </div>
      </div>
      <div class="row">
        <div class="col text-center">
          <div v-for="(item, x) in white" :key="x">
            {{ item.from }} -> {{ item.to }}
          </div>
        </div>
        <div class="col text-center">
          <div class="black" v-for="(item, x) in black" :key="x">
            {{ item.from }} -> {{ item.to }}
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "GameHistoryTab",
  data: function() {
    return {
      white: [],
      black: []
    };
  },
  computed: {
    chessHistory() {
      return this.$store.state.chessHistory;
    }
  },
  watch: {
    chessHistory(newVal) {
      let move = newVal[newVal.length - 1];
      if (move.color === "w") {
        this.white.push(move);
      } else {
        this.black.push(move);
      }
    }
  }
};
</script>

<style scoped>
a:hover {
  text-decoration: none;
}
.chess-history {
  width: 300px;
  height: 200px;
  background-color: white;
  border-radius: 25px;
  box-shadow: 5px 10px 18px #000000;
}
.black {
  background-color: darkgrey;
}
</style>
