<template>
  <b-container>
    <b-alert v-model="showDismissibleAlert" :variant="myVariant" dismissible>
      {{showAlert}}
    </b-alert>
    <b-form @submit="onSubmit"  class="w-100">
    <b-row>
      <b-col>
        <button type="submit" class="btn btn-success btn-sm">
          Submit training
        </button>
      </b-col>
    </b-row>
      <hr>
      <br>
      <br>
    <b-row>
      <b-col></b-col>
      <b-col cols="3">
        <b-form-group id="form-name-group"
                      label="Name:"
                      label-for="form-name-input">
          <b-form-input id="form-name-input"
                        type="text"
                        v-model="addTrainig.name"
                        maxlength="20"
                        required
                        placeholder="Training name:">
          </b-form-input>
        </b-form-group>
      </b-col>
      <b-col></b-col>
      <b-col cols="3">
        <b-form-group id="form-type-group"
                      label="Type:"
                      label-for="form-type-input">
          <b-form-input id="form-type-input"
                        type="text"
                        v-model="addTrainig.type"
                        maxlength="15"
                        required
                        placeholder="Training type:">
          </b-form-input>
        </b-form-group>
      </b-col>
      <b-col></b-col>
      <b-col cols="3">
        <b-form-group id="form-descr-group"
                      label="Training description:"
                      label-for="form-descr-input">
          <b-form-input id="form-descr-input"
                        maxlength="35"
                        type="text"
                        v-model="addTrainig.description"
                        placeholder="Description:">
          </b-form-input>
        </b-form-group>
      </b-col>
      <b-col></b-col>
    </b-row>
    </b-form>
    <b-row>
      <b-col>
        <button type="button" class="btn btn-success btn-sm" v-b-modal.series-modal>
          Add Series
        </button>
      </b-col>
    </b-row>
    <br>
  <b-row>
    <b-col>
      <table class="table table-hover">
        <thead>
        <tr>
          <th scope="col">Exercise name</th>
          <th scope="col">Repetitions</th>
          <th scope="col">Weight</th>
          <th scope="col">Break duration</th>
          <th scope="col">Exercise duration</th>
          <th></th>
        </tr>
        </thead>
        <tbody>
        <tr v-for="(serie, index) in series" :key="index">
          <td>{{ serie.exercise.name}}</td>
          <td>{{ serie.repetitions}}</td>
          <td>{{ serie.weight}}</td>
          <td>{{ serie.break_duration_in_seconds}}</td>
          <td>{{ serie.exercise_duration_in_seconds}}</td>
          <td>
            <div class="btn-group" role="group">
              <button type="button" class="btn btn-danger btn-sm" @click="onDeleteSerie(index)">
                Delete
              </button>
            </div>
          </td>
        </tr>
        </tbody>
      </table>
    </b-col>
  </b-row>
  <b-modal ref="addSeriesModal"
           id="series-modal"
           title="Add a new series"
           hide-footer>
    <b-form @submit="onSubmitSeries"  class="w-100">
  <b-container>
    <b-row>
      <b-col>
      <multiselect id="muscles-group"
                   v-model="newSerie.exercise"
                   :options="exercises"
                   placeholder="Choose exercise"
                   :custom-label="selectLabel"
      />
      </b-col>
    </b-row>
    <br>
    <b-row>
      <b-col>
        <b-form-group id="form-duration-group"
                      label="Duration:"
                      label-for="form-duration-input">
          <b-form-input id="form-duration-input"
                        type="number"
                        v-model="newSerie.exercise_duration_in_seconds"
                        step="1"
                        placeholder="In seconds:">
          </b-form-input>
        </b-form-group>
      </b-col>
      <b-col>
        <b-form-group id="form-bduration-group"
                      label="Break:"
                      label-for="form-bduration-input">
          <b-form-input id="form-bduration-input"
                        type="number"
                        v-model="newSerie.break_duration_in_seconds"
                        step="1"
                        placeholder="In seconds:">
          </b-form-input>
        </b-form-group>
      </b-col>
    </b-row>
    <b-row>
      <b-col>
        <b-form-group id="form-weight-group"
                      label="Weight:"
                      label-for="form-weight-input">
          <b-form-input id="form-weight-input"
                        type="number"
                        v-model="newSerie.weight"
                        step="0.01"
                        placeholder="In kilograms:">
          </b-form-input>
        </b-form-group>
      </b-col>
      <b-col>
        <b-form-group id="form-repetitions-group"
                      label="Repetitions:"
                      label-for="form-repetitions-input">
          <b-form-input id="form-repetitions-input"
                        type="number"
                        v-model="newSerie.repetitions"
                        step="1"
                        placeholder="Reps:">
          </b-form-input>
        </b-form-group>
      </b-col>
    </b-row>
    <b-button type="submit" variant="primary">Submit</b-button>
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
      exercises: [],
      addTrainig: {
        name: '',
        description: '',
        type: '',
      },
      series: [],
      newSerie: {
        repetitions: '',
        weight: '',
        break_duration_in_seconds: '',
        exercise_duration_in_seconds: '',
        exercise: {
          name: '',
        },
      },
      showDismissibleAlert: false,
      showAlert: '',
      myVariant: 'danger',
    };
  },
  methods: {
    getExercises() {
      Axios.get(this.$endpoints.exercise)
        .then((res) => {
          this.exercises = res.data.items;
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    selectLabel({ name, muscles }) {
      return `${name} - ${muscles}`;
    },
    onSubmitSeries(event) {
      event.preventDefault();
      // eslint-disable-next-line radix
      this.newSerie.repetitions = parseInt(this.newSerie.repetitions);
      this.newSerie.weight = parseFloat(this.newSerie.weight);
      // eslint-disable-next-line radix
      this.newSerie.break_duration_in_seconds = parseInt(this.newSerie.break_duration_in_seconds);
      // eslint-disable-next-line radix
      this.newSerie.exercise_duration_in_seconds = parseInt(
        this.newSerie.exercise_duration_in_seconds,
      );
      this.series.push(JSON.parse(JSON.stringify(this.newSerie)));
      this.$refs.addSeriesModal.hide();
      this.initForm();
    },
    onSubmit(event) {
      event.preventDefault();
      const req = Axios.create({
        withCredentials: true,
      });
      const body = {
        training: this.addTrainig,
        series: this.series,
      };
      req.post(
        this.$endpoints.training,
        JSON.stringify(body),
        {
          headers: {
            'Content-Type': 'application/json',
          },
        },
      )
        .then(() => {
          this.showAlert = 'Training added';
          this.myVariant = 'success';
          this.showDismissibleAlert = true;
          this.series = [];
        })
        .catch((error) => {
          this.myVariant = 'danger';
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    initForm() {
      this.newSerie.repetitions = '';
      this.newSerie.weight = '';
      this.newSerie.break_duration_in_seconds = '';
      this.newSerie.exercise_duration_in_seconds = '';
      this.newSerie.exercise = {
        name: '',
      };
    },
    onDeleteSerie(index) {
      this.series.splice(index, 1);
    },
  },
  created() {
    this.getExercises();
  },
};
</script>

<style src="vue-multiselect/dist/vue-multiselect.min.css" />
<style scoped>

</style>
