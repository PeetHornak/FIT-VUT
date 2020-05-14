<template>
  <div class="col-md-6 col-md-offset-3 mx-auto">
    <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
      {{showAlert}}
    </b-alert>
    <b-form @submit="onSubmit" >
      <b-form-group
        id="input-group-1"
        label="Username:"
        label-for="input-1"
        description="We'll never share your email with anyone else."
      >
        <b-form-input
          id="input-1"
          v-model="form.username"
          type="text"
          required
          placeholder="Enter username"></b-form-input>
      </b-form-group>

      <b-form-group id="input-group-2" label="Password:" label-for="input-2">
        <b-form-input
          id="input-2"
          v-model="form.password"
          type="password"
          required
          placeholder="Enter password"></b-form-input>
      </b-form-group>
      <b-button type="submit" class="mr-3" variant="primary">Login</b-button>
      <b-button @click="register" variant="primary">Register</b-button>
    </b-form>
    <div class="mt-5">
    </div>
  </div>
</template>

<script>
import Axios from 'axios';

const path = 'http://localhost:5000/login';
export default {
  name: 'Login',
  data() {
    return {
      form: {
        username: '',
        password: '',
      },
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    register() {
      this.$router.push('/register');
    },
    onSubmit(event) {
      event.preventDefault();
      const req = Axios.create({
        withCredentials: true,
      });
      req.post(path, JSON.stringify(this.form), {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then((res) => {
          if (res.data.useRight === 2) {
            this.$router.push('/admin/profile');
          } else {
            this.$router.push('/user/profile');
          }
        })
        .catch((error) => {
          this.showAlert = error.response.data.error;
          this.showDismissibleAlert = true;
        });
    },
  },
};
</script>

<style scoped>

</style>
