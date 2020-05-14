<template>
  <div>
    <div v-if="isTrainer === null">
    </div>
    <div v-else-if="isTrainer === true" class="container">
      <b-alert v-model="showAlert" :variant="status" dismissible>
        {{ errorMsg }}
      </b-alert>
      <div class="row">
        <div class="col-sm-10 mx-auto">
          <h1>Trainer</h1>
          <hr>
          <button type="button" class="btn btn-success btn-sm mt-1" v-b-modal.addUser-modal>
            Add Trainee
          </button>
          <br><br>
          <table class="table table-hover">
            <thead>
            <tr>
              <th scope="col">Trainee</th>
              <th scope="col">Manage</th>
            </tr>
            </thead>
            <tbody>
            <tr v-for="(traineeName, index) in myTrainees" :key="index">
              <td>{{ traineeName }}</td>
              <td>
                <div class="btn-group" role="group">
                  <button
                    type="button"
                    class="btn btn-success btn-sm"
                    @click="selectUser(traineeName)"
                    v-b-modal.plan-modal>Add plan</button>
                  <button
                    @click="removeTrainee(traineeName)"
                    type="button"
                    class="btn btn-danger btn-sm">Remove</button>
                </div>
              </td>
            </tr>
            </tbody>
          </table>
        </div>
      </div>

      <b-modal ref="addPlanModal"
               id="plan-modal"
               title="Add a new plan"
               hide-footer>
        <b-form @submit="onSubmitPlan"  class="w-100">
          <b-container>
            <b-row>
              <b-col>
                <multiselect id="training-group"
                             v-model="addPlan.training"
                             :options="trainings"
                             placeholder="Choose training"
                             :custom-label="selectLabel"></multiselect>
              </b-col>
            </b-row>
            <br>
            <b-row>
              <b-col>
                <multiselect id="daysweek-group"
                             v-model="addPlan.days_in_week"
                             :options="days"
                             placeholder="Choose days"
                             multiple></multiselect>
              </b-col>
            </b-row>
            <br>
            <b-row>
              <b-col>
                <b-form-group id="form-startdate-group"
                              label="Start date:"
                              label-for="form-startdate-input">
                  <b-form-input id="form-startdate-input"
                                type="date"
                                v-model="addPlan.start_date"
                                step="1"
                                required
                                placeholder="In seconds:">
                  </b-form-input>
                </b-form-group>
              </b-col>
              <b-col>
                <b-form-group id="form-enddate-group"
                              label="End date:"
                              label-for="form-enddate-input">
                  <b-form-input id="form-enddate-input"
                                type="date"
                                v-model="addPlan.end_date"
                                step="1"
                                required
                                placeholder="In seconds:">
                  </b-form-input>
                </b-form-group>
              </b-col>
            </b-row>
            <b-button type="submit" variant="primary" >Submit</b-button>
          </b-container>
        </b-form>
      </b-modal>

      <b-modal ref="addUserModal"
               id="addUser-modal"
               title="Add a new user to train"
               hide-footer>
        <b-form @submit="onSubmit"  class="w-100">
          <b-form-group id="form-title-group"
                        label="Username:"
                        label-for="form-title-input">
            <b-form-input id="form-title-input"
                          v-model="traineeName"
                          required
                          placeholder="Enter trainees username">
            </b-form-input>
          </b-form-group>
          <b-button type="submit" variant="primary">Submit</b-button>
        </b-form>
      </b-modal>
    </div>
    <div v-else>
      <button @click="becomeTrainer" type="button" class="btn btn-primary">Become trainer!</button>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import Multiselect from 'vue-multiselect';

export default {
  name: 'Trainer',
  components: { Multiselect },
  data() {
    return {
      days: ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'],
      plans: [],
      addPlan: {
        days_in_week: null,
        start_date: null,
        end_date: null,
        training: '',
        user_nickname: '',
      },
      trainings: [],
      isTrainer: null,
      myTrainees: [],
      traineeName: '',
      showAlert: false,
      errorMsg: '',
      status: 'danger',
    };
  },
  created() {
    this.getTrainings();
    axios.get(this.$endpoints.isTrainer)
      .then(() => {
        this.isTrainer = true;
        axios.get(this.$endpoints.trainer)
          .then((res) => {
            this.myTrainees = res.data.items;
          })
          .catch((error) => {
            console.log(error);
          });
      })
      .catch(() => {
        this.isTrainer = false;
      });
  },
  methods: {
    removeTrainee(name) {
      const req = axios.create({
        withCredentials: true,
      });
      req.put(
        this.$endpoints.removeTrainee,
        JSON.stringify({ trainee_nickname: name }),
        {
          headers: {
            'Content-Type': 'application/json',
          },
        },
      )
        .then(() => {
          axios.get(this.$endpoints.trainer)
            .then((res) => {
              this.myTrainees = res.data.items;
            })
            .catch((error) => {
              console.log(error);
            });
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showAlert = true;
        });
    },
    initForm() {
      this.addPlan.days_in_week = '';
      this.addPlan.end_date = '';
      this.addPlan.start_date = '';
      this.addPlan.training_name = '';
      this.addPlan.user_nickname = '';
    },
    selectLabel({ name, description }) {
      return `${name} - ${description}`;
    },
    selectUser(userToSelect) {
      this.addPlan.user_nickname = userToSelect;
    },
    getTrainings() {
      axios.get(this.$endpoints.training)
        .then((res) => {
          this.trainings = res.data.items;
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showAlert = true;
        });
    },
    onSubmitPlan(event) {
      event.preventDefault();
      const req = axios.create({
        withCredentials: true,
      });
      req.post(
        this.$endpoints.trainerPlan,
        JSON.stringify(this.addPlan),
        {
          headers: {
            'Content-Type': 'application/json',
          },
        },
      )
        .then(() => {
          this.$refs.addPlanModal.hide();
          this.initForm();
          this.getTrainings();
          this.getPlans();
        })
        .catch((error) => {
          this.$refs.addPlanModal.hide();
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    onSubmit(e) {
      e.preventDefault();
      const req = axios.create({
        withCredentials: true,
      });
      req.post(this.$endpoints.trainerPropose, JSON.stringify(
        {
          nickname: this.traineeName,
        },
      ), {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.traineeName = '';
          this.status = 'success';
          this.errorMsg = 'Proposal was successfully send to user!';
        })
        .catch((error) => {
          this.status = 'danger';
          this.errorMsg = error.response.data.error;
        });
      this.showAlert = true;
      this.$refs.addUserModal.hide();
    },
    becomeTrainer() {
      axios.get(this.$endpoints.makeTrainer)
        .then(() => {
          this.isTrainer = true;
        })
        .catch(() => {
          this.isTrainer = false;
        });
    },
  },
};
</script>

<style src="vue-multiselect/dist/vue-multiselect.min.css"></style>
<style scoped>

</style>
