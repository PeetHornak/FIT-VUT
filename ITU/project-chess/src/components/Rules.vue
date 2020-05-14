<!-- This component was created by Matej Janček (xjance00) as part of project to subject ITU -->
<template>
  <div class="rules container">
    <div class="row" align="center">
      <div class="col" v-for="item in items" :key="item.title">
        <NewGameTab
          :title="item.title"
          :content="item.content"
          :routed_path="item.routed_path"
          v-b-modal="item.modal"
        ></NewGameTab>
      </div>
    </div>
    <b-modal
      id="arrangePiecesModal"
      size="md"
      class="align-items-center"
      title="Rozloženie figúrok"
      hide-footer
    >
      <div class="justify-content-center">
        <p>
          Prvý rad obsahuje zľava doprava tieto figúrky: veža, jazdec, strelec,
          dáma, král, strelec, jazdec a vežu. Druhý rad obsahuje všetkých 8
          pešiakov.
        </p>
        <p>
          Súper má figúrky rozostavané rovnako. Dáma vždy stojí na poli svojej
          farby. Rozmiestnenie figúrok je pri každej hre rovnaké.
        </p>
        <div>
          <b-img
            id="imageInModal"
            center
            :src="require('../assets/defaultBoard.png')"
          ></b-img>
        </div>
      </div>
    </b-modal>
    <b-modal
      id="takePieceModal"
      size="md"
      class="align-items-center"
      title="Zajatie súperovej figúrky"
      hide-footer
    >
      <div class="justify-content-center">
        <p>
          Ak je hráč na ťahu a niektorá z jeho figúrok môže dosiahnuť pole, na
          ktorom je súperová figúrka, smie túto figúrku zajať.
        </p>
        <p>
          Následne hráč odstráni súperovú figúrku z hracieho pola a na jej
          miesto presunie svoju figúrku.
        </p>
        <p>
          Výnimka je pri figúrke pešiak. Pešiak môže zajať súperovú figúrku, ak
          sa nachádza pred ním o jedno poličko po diagonále.
        </p>
        <div>
          <b-img
            id="imageInModal"
            center
            :src="require('../assets/pawnTakeBoard.png')"
          ></b-img>
        </div>
      </div>
    </b-modal>
    <b-modal
      id="defenseKingModal"
      size="md"
      class="align-items-center"
      title="Ohrozenie krála"
      hide-footer
    >
      <div class="justify-content-center">
        <p>
          Ak hráč svojim ťahom ohrozí súperového krála, teda presunie figúrku
          tak, že nasledujúcim ťahom by mohol zajať krála, hovorí sa tomu
          <i>šach</i>.
        </p>
        <p>
          Súper je povinný tejto hrozbe predísť presunutím krála alebo inej
          figúrky zablokovať cestu ku královi.
        </p>
        <p>
          Keď neexistuje taký ťah, ktorý by zabránil šachum, znamená to
          <i>mat</i> a teda koniec hry.
        </p>
      </div>
    </b-modal>
  </div>
</template>

<script>
import NewGameTab from "./NewGameTab";
export default {
  name: "Rules",
  components: { NewGameTab },
  data: function() {
    return {
      items: [
        {
          title: "Rozmiestnenie figúrok",
          content: "Ako rozmiestniť figúrky na začiatku hry?",
          routed_path: "",
          modal: "arrangePiecesModal"
        },
        {
          title: "Zajatie súperovej figúrky",
          content: "Ako zajať súperové figúrky a tak vyhrať hru?",
          routed_path: "",
          modal: "takePieceModal"
        },
        {
          title: "Ohrozenie krála",
          content: "Ako si brániť svojho krála a útočiť? Kedy je koniec hry?",
          routed_path: "",
          modal: "defenseKingModal"
        }
      ]
    };
  }
};
</script>

<style scoped>
#imageInModal {
  max-width: 80%;
}
</style>
