<!-- This component was created by Martin Koči (xkocim05) as part of project to subject ITU -->
<template>
  <div class="chess-settings text-center">
    <h4>Nastavenia</h4>
    <input type="radio" name="Difficulty" value="Nový hráč"> Nový Hráč<br>
    <input type="radio" checked="checked" name="Difficulty" value="Nový hráč"> Začiatočník<br>
    <input type="radio" name="Difficulty" value="Nový hráč"> Pokročilý<br>
    <br>
    <input type="checkbox" name="Hints" value="Hints" checked> Hints<br>
    <input type="checkbox" name="Analysis" value="Analysis" checked> Analysis<br>
  </div>
</template>

<script>
export default {
  name: "GameSettingsTab"
};
</script>

<style scoped>
.chess-settings {
  width: 250px;
  height: 250px;
  background-color: white;
  border-radius: 25px;
  display: table-cell;
  vertical-align: middle;
  box-shadow: 5px 10px 18px #000000;
}
</style>
