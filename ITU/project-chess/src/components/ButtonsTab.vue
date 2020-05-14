<template>
  <div class="chess-buttons">
    <div class="col text-center buttonsDiv">
      <div class="row myCol">
        <b-img
          :src="require('../assets/bulb.png')"
          @click="this.$parent.getHint"
          class="helpImg mt-3"
        ></b-img>
      </div>
      <div class="row myCol">
        <b-img
          :src="require('../assets/back2.png')"
          @click="this.$parent.doUndo"
          class="helpImg mt-3"
        ></b-img>
      </div>
    </div>
    <div class="col text-center buttonsDiv">
      <div class="row myCol">
        <b-img
          :src="require('../assets/new.png')"
          @click="this.$parent.doRestart"
          class="helpImg mt-3"
        ></b-img>
      </div>
      <div class="row myCol">
        <b-img
          :src="require('../assets/swap2.png')"
          @click="this.$parent.changeSide"
          class="helpImg mt-3"
        ></b-img>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "ButtonsTab"
};
</script>

<style scoped>
a:hover {
  text-decoration: none;
}
.chess-buttons {
  width: 300px;
  height: 200px;
  background-color: whitesmoke;
  border-radius: 25px;
  box-shadow: 5px 10px 18px #000000;
}
.helpImg {
  width: 60px;
  height: 60px;
}

.buttonsDiv {
  display: table;
  width: 100%;
  height: 50%;
}
.myCol {
  display: table-cell;
}
</style>
