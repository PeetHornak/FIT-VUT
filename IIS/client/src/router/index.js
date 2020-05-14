import Vue from 'vue';
import VueRouter from 'vue-router';
import Axios from 'axios';
import Exercises from '../views/Exercises.vue';
import Login from '../components/Login.vue';
import Register from '../components/Register.vue';
import BodyWeight from '../components/BodyWeight.vue';
import UserProfile from '../components/UserProfile.vue';
import BodyParams from '../components/BodyParams.vue';
import NewTraining from '../components/NewTraining.vue';
import NewPlan from '../components/NewPlan.vue';
import Trainings from '../components/Trainings.vue';
import Trainer from '../components/Trainer.vue';
import UserLayout from '../views/UserLayout.vue';
import AdminLayout from '../views/AdminLayout.vue';
import AdminExercises from '../admin_components/AdminExercises.vue';
import AdminTrainings from '../admin_components/AdminTrainings.vue';
import Home from '../views/Home.vue';
import AdminUsers from '../admin_components/AdminUsers.vue';

Axios.defaults.withCredentials = true;

Vue.use(VueRouter);

function requireAuth(to, from, next) {
  let username = '';
  let rights = 0;
  if (to.path === '/login' || to.path === '/register') {
    next();
  }
  function proceed() {
    console.log(username);
    console.log(rights);
    if (username !== '') {
      switch (to.meta.permission) {
        case 'user':
          if (rights > 0) {
            next();
          } else {
            next('/login');
          }
          break;
        case 'admin':
          if (rights === 2) {
            next();
          } else {
            next('/login');
          }
          break;
        default:
          next('/login');
      }
    } else {
      next('/login');
    }
  }
  Axios.get('http://localhost:5000/authenticate')
    .then((res) => {
      username = res.data.userName;
      rights = res.data.useRight;
      proceed();
    })
    .catch((error) => {
      console.error(error);
      next('/login');
    });
}


const routes = [
  {
    path: '/user',
    name: 'user',
    component: UserLayout,
    children: [
      {
        path: '/user/exercise',
        name: 'exercise',
        component: Exercises,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/plans',
        name: 'plan',
        component: NewPlan,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/weights',
        name: 'weights',
        component: BodyWeight,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/trainings',
        name: 'trainig',
        component: Trainings,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/trainer',
        name: 'trainer',
        component: Trainer,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/profile',
        name: 'profile',
        component: UserProfile,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/new_training',
        name: 'new_training',
        component: NewTraining,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: '/user/body_params',
        name: 'body_params',
        component: BodyParams,
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
      {
        path: 'user/*',
        name: 'other_routes',
        redirect: '/user',
        beforeEnter: requireAuth,
        meta: {
          permission: 'user',
        },
      },
    ],
    beforeEnter: requireAuth,
    meta: {
      permission: 'user',
    },
  },
  {
    path: '/admin',
    name: 'admin',
    component: AdminLayout,
    children: [
      {
        path: '/admin/exercise',
        name: 'admin_exercise',
        component: AdminExercises,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/plans',
        name: 'admin_plan',
        component: NewPlan,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/weights',
        name: 'admin_weights',
        component: BodyWeight,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/trainings',
        name: 'admin_trainig',
        component: AdminTrainings,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/trainer',
        name: 'admin_trainer',
        component: Trainer,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/profile',
        name: 'admin_profile',
        component: UserProfile,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/new_training',
        name: 'admin_new_training',
        component: NewTraining,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/body_params',
        name: 'admin_body_params',
        component: BodyParams,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: '/admin/users',
        name: 'admin_users',
        component: AdminUsers,
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
      {
        path: 'admin/*',
        name: 'admin_other_routes',
        redirect: '/admin',
        beforeEnter: requireAuth,
        meta: {
          permission: 'admin',
        },
      },
    ],
    beforeEnter: requireAuth,
    meta: {
      permission: 'admin',
    },
  },
  {
    path: '*',
    name: 'other paths',
    redirect: '/home',
  },
  {
    path: '/home',
    name: 'home',
    component: Home,
  },
  {
    path: '/login',
    name: 'login',
    component: Login,
  },
  {
    path: '/register',
    name: 'register',
    component: Register,
  },
];


const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes,
});

export default router;
