<template>
  <div class="container">
    <div class="row">
      <div class="col-sm-10 mx-auto">
        <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
          {{showAlert}}
        </b-alert>
        <h1>Weights</h1>
        <hr><br><br>
        <button type="button" class="btn btn-success btn-sm" v-b-modal.weight-modal>
          Add Weight
        </button>
        <br><br>
        <table class="table table-hover">
          <thead>
          <tr>
            <th scope="col">Weight</th>
            <th scope="col">Date</th>
            <th></th>
          </tr>
          </thead>
          <tbody>
          <tr v-for="(body_weight, index) in body_weights" :key="index">
            <td>{{ body_weight.weight}}</td>
            <td>{{new Date(body_weight.date).toDateString()}}</td>
            <td>
              <div class="btn-group" role="group">
                <button
                  type="button" class="btn btn-danger btn-sm" @click="onDeleteWeight(body_weight)">
                  Delete
                </button>
              </div>
            </td>
          </tr>
          </tbody>
        </table>
      </div>
    </div>
    <b-modal ref="addWeightModal"
             id="weight-modal"
             title="Add a new weight"
             hide-footer>
      <b-form @submit="onSubmit"  class="w-100">
        <b-form-group id="form-title-group"
                      label="Weight:"
                      label-for="form-title-input">
          <b-form-input id="form-title-input"
                        type="number"
                        step="0.01"
                        v-model="addWeight.weight"
                        required
                        placeholder="Enter weight in kilograms">
          </b-form-input>
        </b-form-group>
        <b-button type="submit" variant="primary">Submit</b-button>
      </b-form>
    </b-modal>
  </div>
</template>
<script>
import axios from 'axios';

export default {
  name: 'BodyWeight.vue',
  data() {
    return {
      body_weights: [],
      addWeight: {
        weight: '',
      },
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    getWeights() {
      axios.get(this.$endpoints.body_weight)
        .then((res) => {
          this.body_weights = res.data.items;
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    initForm() {
      this.addWeight.weight = null;
    },
    postWeight() {
      const req = axios.create({
        withCredentials: true,
      });
      // eslint-disable-next-line radix
      this.addWeight.weight = parseInt(this.addWeight.weight);
      req.post(this.$endpoints.body_weight, JSON.stringify(this.addWeight), {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.getWeights();
          this.initForm();
        })
        .catch((error) => {
          this.initForm();
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    onSubmit(event) {
      event.preventDefault();
      this.$refs.addWeightModal.hide();
      this.postWeight();
      this.initForm();
    },
    onDeleteWeight(bodyWeight) {
      axios.delete(`${this.$endpoints.body_weight}/${bodyWeight.id}`)
        .then(() => this.getWeights())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
  },
  created() {
    this.getWeights();
  },
};

</script>

<style scoped>

</style>
