<template>
  <div class="container">
    <div class="row">
      <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
        {{showAlert}}
      </b-alert>
      <div class="col-sm-10 mx-auto">
        <h1>Exercises</h1>
        <hr><br><br>
        <button type="button" class="btn btn-success btn-sm" v-b-modal.exercise-modal>
          Add Exercise
        </button>
        <br><br>
        <table class="table table-hover">
          <thead>
          <tr>
            <th scope="col">Name</th>
            <th scope="col">Muscles</th>
            <th scope="col">Body part</th>
            <th scope="col">Description</th>
            <th scope="col">Multi Joint</th>
            <th></th>
          </tr>
          </thead>
          <tbody>
          <tr v-for="(exercise, index) in exercises" :key="index">
            <td>{{ exercise.name}}</td>
            <td>{{ exercise.muscles.join(',')}}</td>
            <td>{{ exercise.body_part}}</td>
            <td>{{ exercise.description}}</td>
            <td>
              <span v-if="exercise.multi_joint">Yes</span>
              <span v-else>No</span>
            </td>
            <td>
              <div class="btn-group" role="group">
            <button
              type="button" class="btn btn-danger btn-sm" @click="onDeleteExercise(exercise)">
              Delete
            </button>
              </div>
            </td>
          </tr>
          </tbody>
        </table>
      </div>
    </div>
    <b-modal ref="addExerciseModal"
             id="exercise-modal"
             title="Add a new exercise"
             hide-footer>
      <b-form @submit="onSubmit"  class="w-100">
        <b-form-group id="form-muscle-group"
                      label="Name:"
                      label-for="form-muscle-input">
          <b-form-input id="form-muscle-input"
                        type="text"
                        v-model="addExercise.name"
                        maxlength="10"
                        required
                        placeholder="Enter name:">
          </b-form-input>
        </b-form-group>

        <b-form-group id="form-descr-group"
                      label="Description:"
                      label-for="form-descr-input">
          <b-form-input id="form-title-input"
                        type="text"
                        v-model="addExercise.description"
                        maxlength="30"
                        placeholder="Enter description:">
          </b-form-input>
        </b-form-group>
        <b-form-group id="form-bodypart-group"
                      label="Body part:"
                      label-for="form-bodypart-input">
          <b-form-input id="form-title-input"
                        type="text"
                        maxlength="15"
                        v-model="addExercise.body_part"
                        placeholder="Enter bodypart:">
          </b-form-input>
        </b-form-group>
        <b-form-group id="form-read-group">
          <b-form-checkbox-group v-model="addExercise.multi_joint" id="form-checks">
            <b-form-checkbox value="true">Multi-joint?</b-form-checkbox>
          </b-form-checkbox-group>
        </b-form-group>
        <b-input-group label="Muscles:" label-for="muscles-group">
          <multiselect id="muscles-group"
                       v-model="addExercise.muscles"
                       :options="muscle_options"
                       placeholder="Choose muscles"
                       multiple
          />
        </b-input-group>
        <br>
        <b-button type="submit" variant="primary">Submit</b-button>
      </b-form>
    </b-modal>
  </div>
</template>
<script>
import Axios from 'axios';
import Multiselect from 'vue-multiselect';

export default {
  name: 'Exercise.vue',
  components: { Multiselect },
  data() {
    return {
      exercises: [],
      addExercise: {
        description: '',
        type: '',
        muscles: [],
        body_part: '',
        multi_joint: [],
        name: '',
      },
      muscle_options: ['biceps', 'triceps', 'chest', 'soleoid'],
      showDismissibleAlert: false,
      showAlert: '',
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
    onSubmit(event) {
      event.preventDefault();
      const req = Axios.create({
        withCredentials: true,
      });
      let multiJoint = false;
      if (this.addExercise.multi_joint[0]) multiJoint = true;
      this.addExercise.multi_joint = multiJoint;
      req.post(this.$endpoints.exercise, JSON.stringify(this.addExercise), {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.$refs.addExerciseModal.hide();
          this.getExercises();
        })
        .catch((error) => {
          this.$refs.addExerciseModal.hide();
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
    onDeleteExercise(exercise) {
      Axios.delete(`${this.$endpoints.exercise}/${exercise.id}`)
        .then(() => this.getExercises())
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
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
