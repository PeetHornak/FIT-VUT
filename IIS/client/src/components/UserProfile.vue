<template>
  <div class="row">
    <div class="col-md-9">
      <div class="row row-no-gutters mb-5 mt-3">
        <b-col class="col-md-4">
          <b-img fluid
                 width="150"
                 height="150" :src="image" rounded="circle" alt="Profile photo"></b-img>
        </b-col>
        <b-container class="col-md-4 float-left">
          <div class="column">
            <div class="segment">
              <h2 class="header" >{{ nickname }}</h2>
              <a :href="'mailto:' + email">{{ email }}</a>
            </div>
          </div>
        </b-container>

      </div>
      <div class="row row-no-gutters my-3">
        <b-container>
          <b-row class="text-left">
            <b-col class="md-4"></b-col>
              <b-col cols="8">

                  <b-nav tabs fill>
                    <b-nav-item @click="infoTab">Info</b-nav-item>
                    <b-nav-item @click="passwordTab">Password</b-nav-item>
                    <b-nav-item @click="imageTab">Image</b-nav-item>
                  </b-nav>
                  <b-row class="text-center mt-4" align-h="center">
                    <component v-bind:is="currentComponent"></component>
                  </b-row>

              </b-col>
          </b-row>
        </b-container>
      </div>
    </div>
    <div class="col-3 pr-5">
      <div class="row">
        <div class="col-sm-10 mx-auto">
          <h4>Trainers requests</h4>
          <br>
          <table class="table table-hover">
            <thead>
            <tr>
              <th scope="col">Name</th>
              <th></th><th></th>
            </tr>
            </thead>
            <tbody>
            <tr v-for="(trainer_name, index) in trainers" :key="index">
              <td>{{ trainer_name }}</td>
              <td>
                <div class="btn-group" role="group">
                  <button
                    @click="acceptHim(trainer_name)"
                    type="button"
                    class="btn btn-success btn-sm"
                  >Accept
                  </button>
                  <button
                    @click="rejectHim(trainer_name)"
                    type="button"
                    class="btn btn-danger btn-sm"
                  >Reject
                  </button>
                </div>
              </td>
            </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
<!--    <div class="col-1"></div>-->
  </div>
</template>

<script>
import axios from 'axios';
import updateInfo from './UpdateInfo.vue';
import updatePassword from './UpdatePassword.vue';
import updateImage from './UpdateImage.vue';

export default {
  name: 'UserProfile',
  components: {
    updateInfo,
    updatePassword,
    updateImage,
  },
  data() {
    return {
      birthdate: '',
      email: '',
      name: '',
      nickname: '',
      image: '',
      currentComponent: '',
      trainers: [],
    };
  },
  created() {
    this.updateUser();
  },
  methods: {
    updateUser() {
      axios.get(this.$endpoints.user)
        .then((res) => {
          this.birthdate = res.data.items.birthdate;
          this.email = res.data.items.email;
          this.name = res.data.items.name;
          this.nickname = res.data.items.nickname;
          this.trainers = res.data.trainers;
        })
        .catch((error) => {
          console.error(error);
        });
      axios.get(this.$endpoints.image)
        .then((res) => {
          this.image = `data:image/jpg;base64,${res.data}`;
        })
        .catch((error) => {
          console.log(error);
        });
    },
    infoTab() {
      this.updateUser();
      this.currentComponent = updateInfo;
    },
    acceptHim(name) {
      const req = axios.create({
        withCredentials: true,
      });
      req.put(this.$endpoints.doTrainer, JSON.stringify(
        {
          trainer_nickname: name,
          accept: true,
        },
      ),
      {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.updateUser();
        })
        .catch((error) => {
          console.log(`UserProfile.vue:acceptHim: ${error}`);
        });
    },
    rejectHim(name) {
      const req = axios.create({
        withCredentials: true,
      });
      req.put(this.$endpoints.doTrainer, JSON.stringify(
        {
          trainer_nickname: name,
          accept: false,
        },
      ),
      {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.updateUser();
        })
        .catch((error) => {
          console.log(`UserProfile.vue:acceptHim: ${error}`);
        });
    },
    passwordTab() {
      this.currentComponent = updatePassword;
    },
    imageTab() {
      this.currentComponent = updateImage;
    },
  },
};
</script>
