<!-- This component was created by Peter Horňák (xhorna14) as part of project to subject ITU -->

<template>
  <b-img
    v-bind="mainProps"
    :src="mySrc"
    fluid
  ></b-img>
</template>

<script>
export default {
  name: "pawn",
  props: ["color"],
  data: function() {
    return {
      myColor: this.color,
      mainProps: {
        width: 80,
        height: 80,
        "z-index": 2
      }
    };
  },
  computed: {
    mySrc() {
      return require("../assets/BlackQueen.png");
    }
  }
};
</script>

<style scoped></style>
