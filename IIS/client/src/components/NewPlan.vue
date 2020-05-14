<template>
  <b-container>
    <b-row>
      <b-col>
        <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
          {{showAlert}}
        </b-alert>
        <button type="button" class="btn btn-success btn-sm" v-b-modal.plan-modal>
          Add Plan
        </button>
      </b-col>
    </b-row>
    <br>
    <b-row>
      <b-col>
        <table class="table table-hover">
          <thead>
          <tr>
            <th scope="col">Days in week</th>
            <th scope="col">Start date</th>
            <th scope="col">End date</th>
            <th scope="col">Training </th>
            <th></th>
          </tr>
          </thead>
          <tbody>
          <tr v-for="(plan, index) in plans" :key="index">
            <td>{{ plan.days_in_week.join(',')}}</td>
            <td>{{ new Date(plan.start_date).toDateString()}}</td>
            <td>{{ new Date(plan.end_date).toDateString()}}</td>
            <td>{{ plan.training.name}}</td>
            <td>
              <div class="btn-group" role="group">
                <button type="button" class="btn btn-danger btn-sm" @click="onDeletePlan(plan)">
                  Delete
                </button>
              </div>
            </td>
          </tr>
          </tbody>
        </table>
      </b-col>
    </b-row>
    <b-modal ref="addPlanModal"
             id="plan-modal"
             title="Add a new plan"
             hide-footer>
      <b-form @submit="onSubmit"  class="w-100">
        <b-container>
          <b-row>
            <b-col>
              <multiselect id="training-group"
                           v-model="addPlan.training"
                           :options="trainings"
                           placeholder="Choose training"
                           :custom-label="selectLabel"
              />
            </b-col>
          </b-row>
          <br>
          <b-row>
            <b-col>
              <multiselect id="daysweek-group"
                           v-model="addPlan.days_in_week"
                           :options="days"
                           placeholder="Choose days"
                           multiple
              />
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
  </b-container>
</template>

<script>
import Axios from 'axios';
import Multiselect from 'vue-multiselect';

export default {
  name: 'NewTraining',
  components: { Multiselect },
  data() {
    return {
      days: ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'],
      plans: [],
      addPlan: {
        days_in_week: null,
        start_date: null,
        end_date: null,
        training: null,
      },
      trainings: [],
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    getPlans() {
      Axios.get(this.$endpoints.plan)
        .then((res) => {
          this.plans = res.data.items;
        }).catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
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
    selectLabel({ name, description }) {
      return `${name} - ${description}`;
    },
    onSubmit(event) {
      event.preventDefault();
      if (this.addPlan.training == null || this.addPlan.days_in_week == null) {
        this.showAlert = 'Choose at least one training and day in week';
        this.showDismissibleAlert = true;
        this.$refs.addPlanModal.hide();
        return;
      }
      const req = Axios.create({
        withCredentials: true,
      });
      req.post(
        this.$endpoints.plan,
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
    initForm() {
      this.addPlan.days_in_week = '';
      this.addPlan.end_date = '';
      this.addPlan.start_date = '';
      this.addPlan.training_name = '';
    },
    onDeletePlan(plan) {
      Axios.delete(`${this.$endpoints.plan}/${plan.id}`)
        .then(() => this.getPlans())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
  },
  created() {
    this.getPlans();
    this.getTrainings();
  },
};
</script>

<style src="vue-multiselect/dist/vue-multiselect.min.css"></style>
<style scoped>

</style>
