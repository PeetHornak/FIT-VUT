<!-- This component was created by Martin Koči (xkocim05) as part of project to subject ITU -->
<template>
  <div class="container-fluid">
    <div class="row">
      <div class="col p-3">
        <h1 class="text-center text-white">Šach</h1>
      </div>
    </div>
    <div class="row">
      <div class="col">
        <h2 class="text-center text-white">Vytvorenie novej hry!</h2>
        <p class="text-center text-white">
          Zahrajte si šach s našou umelou inteligenciu a zistite čo môžete robiť
          lepšie pomocou našej analýzy
        </p>
      </div>
    </div>
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
import NewGameTab from "./NewGameTab.vue";
export default {
  name: "MainMenu",
  components: {
    NewGameTab
  },
  data: function() {
    return {
      items: [
        {
          title: "Nový Hráč",
          content:
            "Naučete sa základy hrania šachu pomocou nášho návodu ako hrať šach a nauče sa hrať hru pomocou našej analýzy.",
          routed_path: "/tutorial"
        },
        {
          title: "Začiatočník",
          content:
            "Ak už viete základy pustite sa rovno do hrania hry a zdokonalute sa aby ste sa jedneho dňa stali profesionály.",
          routed_path: "/board"
        },
        {
          title: "Pokročilý",
          content:
            "Viete už dosť a cítite sa na to aby ste porazili našu najťažšiu obtiažnosť? Tak do toho!",
          routed_path: "/board"
        }
      ]
    };
  }
};
</script>

<style>
#app {
  background-image: url("../assets/main_menu.png");
  height: 100%;
  width: 100%;
  background-position: center;
  background-repeat: no-repeat;
  background-size: cover;
  overflow-y: auto;
  height: 100vh;
  display: flex;
  align-items: center;
}
</style>

<style scoped>
p {
  font-size: 18px;
}
.row {
  height: 200px;
}
.col {
  min-width: 350px;
  min-height: 220px;
}
h1 {
  font-size: 60px;
}
</style>
