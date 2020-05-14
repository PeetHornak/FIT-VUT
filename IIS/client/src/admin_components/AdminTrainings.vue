<template>
  <div class="container">
    <div class="row">
      <div class="col-sm-10 mx-auto">
        <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
          {{showAlert}}
        </b-alert>
        <h1>Trainings</h1>
        <hr><br><br>
        <button type="button" class="btn btn-success btn-sm" @click="redirectToTraining" >
          Add Training
        </button>
        <br><br>
        <table class="table table-hover">
          <thead>
          <tr>
            <th scope="col">Name</th>
            <th scope="col">Type</th>
            <th scope="col">Description</th>
            <th></th>
          </tr>
          </thead>
          <tbody>
          <tr v-for="(training, index) in trainings" :key="index">
            <td>{{ training.name}}</td>
            <td>{{ training.type}}</td>
            <td>{{ training.description}}</td>
            <td>
              <div class="btn-group" role="group">
                <button
                  type="button" class="btn btn-danger btn-sm" @click="onDeleteTraining(training)">
                  Delete
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
      trainings: [],
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    getTrainings() {
      Axios.get(this.$endpoints.training)
        .then((res) => {
          this.trainings = res.data.items;
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    redirectToTraining() {
      this.$router.push('/admin/new_training');
    },
    onDeleteTraining(training) {
      Axios.delete(`${this.$endpoints.training}/${training.id}`)
        .then(() => this.getTrainings())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
  },
  created() {
    this.getTrainings();
  },
};

</script>

<style scoped>

</style>
