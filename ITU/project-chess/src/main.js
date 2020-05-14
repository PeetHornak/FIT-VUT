import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'
import BootstrapVue from 'bootstrap-vue'
import Vue from "vue";
import App from "./App.vue";
import store from "./store/index";
import VueRouter from "vue-router";
import MainMenu from "./components/MainPage.vue";
import Tutorial from "./components/Tutorial";
import Moves from "./components/Moves";
import Rules from "./components/Rules";
import Board from "./components/Board";

Vue.use(BootstrapVue);

Vue.use(VueRouter);
const routes = [
  { path: "/", component: MainMenu },
  { path: "/tutorial", component: Tutorial },
  { path: "/moves", component: Moves },
  { path: "/rules", component: Rules },
  { path: "/board", component: Board }
];

const router = new VueRouter({
  routes,
  mode: "history"
});

Vue.config.productionTip = false;

new Vue({
  store,
  render: h => h(App),
  router
}).$mount("#app");
