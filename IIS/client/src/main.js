import 'bootstrap/dist/css/bootstrap.css';
import BootstrapVue from 'bootstrap-vue';
import Multiselect from 'vue-multiselect';
import Vue from 'vue';
import App from './App.vue';
import router from './router';
import store from './store';

Vue.use(Multiselect);
Vue.use(BootstrapVue);
Vue.config.productionTip = false;

Vue.prototype.$api_route = 'http://localhost:5000';
Vue.prototype.$endpoints = {
  user: `${Vue.prototype.$api_route}/user`,
  exercise: `${Vue.prototype.$api_route}/exercise`,
  training: `${Vue.prototype.$api_route}/training`,
  body_weight: `${Vue.prototype.$api_route}/body_weight`,
  login: `${Vue.prototype.$api_route}/login`,
  plan: `${Vue.prototype.$api_route}/plan`,
  trainerPlan: `${Vue.prototype.$api_route}/trainer_plan`,
  body_params: `${Vue.prototype.$api_route}/body_size`,
  update_info: `${Vue.prototype.$api_route}/update_user`,
  update_pwd: `${Vue.prototype.$api_route}/change_password`,
  update_img: `${Vue.prototype.$api_route}/change_image`,
  authenticate: `${Vue.prototype.$api_route}/authenticate`,
  doTrainer: `${Vue.prototype.$api_route}/trainer_propose`,
  password: `${Vue.prototype.$api_route}/password`,
  isTrainer: `${Vue.prototype.$api_route}/is_trainer`,
  makeTrainer: `${Vue.prototype.$api_route}/make_trainer`,
  trainerPropose: `${Vue.prototype.$api_route}/trainer_propose`,
  trainer: `${Vue.prototype.$api_route}/trainer`,
  removeTrainee: `${Vue.prototype.$api_route}/trainee`,
  image: `${Vue.prototype.$api_route}/image`,
  users: `${Vue.prototype.$api_route}/users`,
};

new Vue({
  router,
  store,
  render: h => h(App),
}).$mount('#app');
