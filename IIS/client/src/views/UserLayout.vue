<template>
  <div id="nav">
    <router-link to="/user/exercise">Exercises</router-link> |
    <router-link to="/user/weights">Weights</router-link> |
    <router-link to="/user/profile">Profile</router-link> |
    <router-link to="/user/body_params">BodyParams</router-link> |
    <router-link to="/user/trainer">Trainer</router-link> |
    <router-link to="/user/trainings">Trainings</router-link> |
    <router-link to="/user/plans">Plans</router-link> |
    <b-button @click="logoutMe" variant="success">LOGOUT</b-button>
    <br>
    <br>
    <br>
    <hr>
    <router-view></router-view>
  </div>
</template>

<script>
export default {
  name: 'UserLayout',
  methods: {
    logoutMe() {
      document.cookie = 'FITSESSIONID=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';
      this.$router.push('/login');
    },
  },
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
