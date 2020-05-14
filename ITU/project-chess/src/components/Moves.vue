<!-- This component was created by Matej Janček (xjance00) as part of project to subject ITU -->
<template>
  <div class="container">
    <div class="row" align="center">
      <div class="col p-3" v-for="item in items" :key="item.title">
        <NewGameTab
          :title="item.title"
          :routed_path="item.routed_path"
          v-b-modal="`${item.info}modal`"
        ></NewGameTab>
      </div>
    </div>
    <div class="row" align="center">
      <div class="col p-3" v-for="item in items2" :key="item.title">
        <NewGameTab
          :title="item.title"
          :routed_path="item.routed_path"
          v-b-modal="`${item.info}modal`"
        ></NewGameTab>
      </div>
    </div>
    <b-modal
      v-for="item in items"
      :key="item.title"
      :id="item.info + 'modal'"
      size="md"
      class="align-items-center"
      :title="item.title"
      hide-footer
    >
      <b-img-lazy
        :src="require('../assets/' + item.info + 'Move.png')"
        center
        fluid
      >
      </b-img-lazy>
      <div class="justify-content-center">
        <p></p>
        <p>{{ item.content }}</p>
      </div>
    </b-modal>
    <b-modal
      v-for="item in items2"
      :key="item.title"
      :id="item.info + 'modal'"
      size="md"
      class="align-items-center"
      :title="item.title"
      hide-footer
    >
      <b-img-lazy
        :src="require('../assets/' + item.info + 'Move.png')"
        center
        fluid
      >
      </b-img-lazy>
      <div class="justify-content-center">
        <p></p>
        <p>{{ item.content }}</p>
      </div>
    </b-modal>
  </div>
</template>

<script>
import NewGameTab from "./NewGameTab";
export default {
  name: "Rules",
  components: {
    NewGameTab
  },
  data: function() {
    return {
      items: [
        {
          title: "Veža",
          content:
            "Veža sa pohybuje o ľubovoľný počet polí po riadkoch alebo stĺpcoch.",
          routed_path: "",
          info: "rook"
        },
        {
          title: "Strelec",
          content: "Strelec sa pohybuje o ľubovoľný počet polí po diagonálach.",
          routed_path: "",
          info: "bishop"
        },
        {
          title: "Jazdec",
          content:
            "Jazdec ako jediný môže skákať cez ostatné figúrky. Pohybuje sa v tvare písmena L. Dve polia po riadku alebo stĺpci a jedno pole druhým smerom.",
          routed_path: "",
          info: "knight"
        }
      ],
      items2: [
        {
          title: "Dáma",
          content:
            "Dáma sa pohybuje do ľubovoľného smeru o ľubovoľný počet polí.",
          routed_path: "",
          info: "queen"
        },
        {
          title: "Kráľ",
          content:
            "Kráľ sa pohybuje o jedno pole do ľubovoľného smeru. Svojim ťahom sa nesmie dostať na pole, kde by bol v ohrození.",
          routed_path: "",
          info: "king"
        },
        {
          title: "Pešiak",
          content:
            "Pešiak sa pohybuje len vo svojom stĺpci, po jednom poli. Výnimka je len pri pohybe z východiskovej pozície. V tom prípade sa môže posunúť až o 2 políčka. Súperovú figúrku môže zajať, ak sa nachádza políčko pred ním diagonálne.",
          routed_path: "",
          info: "pawn"
        }
      ]
    };
  }
};
</script>

<style scoped>
.b-tab {
  background-color: white;
}
/deep/ .new-game-menu {
  height: 60px;
}
</style>
