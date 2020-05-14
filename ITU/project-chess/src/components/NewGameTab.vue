<!-- This component was created by Martin Koči (xkocim05) as part of project to subject ITU -->
<template>
  <router-link :to="routed_path">
    <div class="new-game-menu ">
      <div class="text-body text-center new_game_menu_title">
        <strong>{{ title }}</strong>
      </div>
      <div class="text-secondary text-center new_game_menu_content">
        {{ content }}
      </div>
    </div>
  </router-link>
</template>

<script>
export default {
  name: "NewGameTab",
  props: ["title", "content", "routed_path"],
  computed: {
    getPath() {
      return this.routed_path;
    }
  }
};
</script>

<style scoped>
a:hover {
  text-decoration: none;
}
.new-game-menu {
  width: 300px;
  height: 200px;
  background-color: white;
  border-radius: 25px;
  box-shadow: 5px 10px 18px #000000;
}
.new_game_menu_title {
  width: 280px;
  height: 40px;
  display: table-cell;
  vertical-align: middle;
  margin: 10px;
  box-shadow: 2px 2px 18px #828282;
  border-radius: 25px;
}

.new_game_menu_content {
  margin: 15px;
}
</style>
