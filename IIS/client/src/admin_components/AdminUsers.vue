<template>
  <div class="container">
    <div class="row">
      <div class="col-sm-10 mx-auto">
        <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
          {{showAlert}}
        </b-alert>
        <h1>Users</h1>
        <hr><br><br>
        <br><br>
        <table class="table table-hover">
          <thead>
          <tr>
            <th scope="col">Nickname</th>
            <th scope="col">Email</th>
            <th scope="col">Active</th>
            <th></th>
          </tr>
          </thead>
          <tbody>
          <tr v-for="(user, index) in users" :key="index">
            <td>{{ user.nickname}}</td>
            <td>{{ user.email}}</td>
            <td>{{ user.active}}</td>
            <td>
              <div class="btn-group mr-1" role="group">
                <button
                  type="button" class="btn btn-danger btn-sm" @click="onDeleteUser(user)">
                  Delete
                </button>
              </div>
              <div class="btn-group" role="group">
                <button
                  type="button" class="btn btn-success btn-sm" @click="onActivateUser(user)">
                  Activate
                </button>
              </div>
            </td>
          </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>
<script>
import Axios from 'axios';

export default {
  name: 'Exercise.vue',
  data() {
    return {
      users: [],
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    getUsers() {
      Axios.get(this.$endpoints.users)
        .then((res) => {
          this.users = res.data.items;
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    onDeleteUser(user) {
      Axios.delete(`${this.$endpoints.user}/${user.id}`)
        .then(() => this.getUsers())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    onActivateUser(user) {
      Axios.put(`${this.$endpoints.user}/${user.id}`)
        .then(() => this.getUsers())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
  },
  created() {
    this.getUsers();
  },
};

</script>

<style scoped>

</style>
