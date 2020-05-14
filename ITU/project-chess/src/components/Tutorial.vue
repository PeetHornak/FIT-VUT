<!-- This component was created by Matej Janček (xjance00) as part of project to subject ITU -->
<template>
  <div class="container">
    <div class="row" align="center">
      <div class="col" v-for="item in items" :key="item.title">
        <NewGameTab
          :title="item.title"
          :content="item.content"
          :routed_path="item.routed_path"
        ></NewGameTab>
      </div>
    </div>
  </div>
</template>

<script>
import NewGameTab from "./NewGameTab";
export default {
  name: "tutorial",
  components: { NewGameTab },
  data: function() {
    return {
      items: [
        {
          title: "Pravidlá",
          content: "Pravidlá šachu nie sú zložité. Prečítaj si ich!",
          routed_path: "/rules"
        },
        {
          title: "Pohyb figúrok",
          content:
            "Ako posúvať jednotlivé figúrky po hracej ploche? Všetko na jednom mieste.",
          routed_path: "/moves"
        }
      ]
    };
  }
};
</script>

<style scoped>
/deep/ .new-game-menu {
  height: 150px;
}
</style>
