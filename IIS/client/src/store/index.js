import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

const store = new Vuex.Store({
  state: {
    user: {
      userName: '',
      permission: 0,
    },
  },
  mutations: {
    logUser(userData) {
      console.log('mutating');
      this.$store.state.user.userName = userData.userName;
      this.$store.state.user.permission = userData.permission;
    },
  },
  actions: {
  },
  modules: {
  },
  getters: {
    getUserName: state => state.user.userName,
    getUserPermission: state => state.user.permission,
  },
});

export default store;
