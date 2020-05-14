<template>
  <div id="nav">
    <h1>
      Welcome to Fitcado.
      <br>
      <br>
      <br>
      <hr>
      <b-container>
        <b-row>
          <b-col>
          </b-col>
          <b-col>
            <router-link to="/login">Login</router-link>
          </b-col>
          <b-col>
          </b-col>
          <b-col>
            <router-link to="/register">Register</router-link>
          </b-col>
          <b-col>
          </b-col>
        </b-row>
      </b-container>
    </h1>
    <br>
    <br>
    <br>
    <hr>
  </div>
</template>

<script>
export default {
  name: 'Home',
};
</script>

<style scoped>
  #app {
    font-family: 'Avenir', Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
  }

  #nav {
    padding: 30px;
  }

  #nav a {
    font-weight: bold;
    color: #2c3e50;
  }

  #nav a.router-link-exact-active {
    color: #42b983;
  }
</style>
