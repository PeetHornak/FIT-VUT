<template>
  <div class="container">
    <div class="row">
      <div class="col-sm-10 mx-auto">
        <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
          {{showAlert}}
        </b-alert>
        <h1>Body Parameters</h1>
        <hr><br><br>
        <button type="button" class="btn btn-success btn-sm" v-b-modal.param-modal>
          Add Body parameters
        </button>
        <br><br>
        <table class="table table-hover">
          <thead>
          <tr>
            <th scope="col">Waist</th>
            <th scope="col">Neck</th>
            <th scope="col">Biceps</th>
            <th scope="col">Date</th>
            <th></th>
          </tr>
          </thead>
          <tbody>
          <tr v-for="(body_param, index) in body_params" :key="index">
            <td>{{ body_param.waist}}</td>
            <td>{{ body_param.neck}}</td>
            <td>{{ body_param.biceps}}</td>
            <td>{{ new Date(body_param.date).toDateString()}}</td>
            <td>
              <div class="btn-group" role="group">
                <button type="button" class="btn btn-danger btn-sm" @click="onDelete(body_param)">
                  Delete
                </button>
              </div>
            </td>
          </tr>
          </tbody>
        </table>
      </div>
    </div>
    <b-modal ref="addParamModal"
             id="param-modal"
             title="Add a new body params"
             hide-footer>
      <b-form @submit="onSubmit"  class="w-100">
        <b-form-group id="form-waist-group"
                      label="Waist:"
                      label-for="form-waist-input">
          <b-form-input id="form-waist-input"
                        type="number"
                        step="0.01"
                        v-model="addBodyParams.waist"
                        required
                        placeholder="Enter size in centimeters">
          </b-form-input>
        </b-form-group>
        <b-form-group id="form-neck-group"
                      label="Weight:"
                      label-for="form-neck-input">
          <b-form-input id="form-neck-input"
                        type="number"
                        step="0.01"
                        v-model="addBodyParams.neck"
                        required
                        placeholder="Enter size in centimeterss">
          </b-form-input>
        </b-form-group>
        <b-form-group id="form-title-group"
                      label="Weight:"
                      label-for="form-title-input">
          <b-form-input id="form-title-input"
                        type="number"
                        step="0.01"
                        v-model="addBodyParams.biceps"
                        required
                        placeholder="Enter size in centimeters">
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
  name: 'BodyParams',
  data() {
    return {
      body_params: [],
      addBodyParams: {
        waist: '',
        neck: '',
        biceps: '',
      },
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    getParams() {
      axios.get(this.$endpoints.body_params)
        .then((res) => {
          this.body_params = res.data.items;
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    initForm() {
      this.addBodyParams.waist = '';
      this.addBodyParams.biceps = '';
      this.addBodyParams.neck = '';
    },
    postBodyParams() {
      this.addBodyParams.biceps = parseFloat(this.addBodyParams.biceps);
      this.addBodyParams.waist = parseFloat(this.addBodyParams.waist);
      this.addBodyParams.neck = parseFloat(this.addBodyParams.neck);
      const req = axios.create({
        withCredentials: true,
      });
      req.post(this.$endpoints.body_params, JSON.stringify(this.addBodyParams), {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.$refs.addParamModal.hide();
          this.getParams();
        })
        .catch((error) => {
          this.$refs.addParamModal.hide();
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    onSubmit(event) {
      event.preventDefault();
      this.postBodyParams();
      this.initForm();
    },
    onDelete(bodyParam) {
      axios.delete(`${this.$endpoints.body_params}/${bodyParam.id}`)
        .then(() => this.getParams())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
  },
  created() {
    this.getParams();
  },


};


</script>

<style scoped>

</style>
