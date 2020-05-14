<template>
  <b-form @submit="updateUser">
    <b-form-group
      id="input-group-1"
      label="Email address:"
      label-for="input-1"
      description="We'll never share your email with anyone else."
    >
      <b-form-input
        id="input-1"
        v-model="newEmail"
        type="email"
        required
        placeholder="Enter email"
      ></b-form-input>
    </b-form-group>

    <b-form-group id="input-group-2" label="Your Name:" label-for="input-2">
      <b-form-input
        id="input-2"
        v-model="newName"
        required
        placeholder="Enter new name"
      ></b-form-input>
    </b-form-group>
    <b-button type="submit" variant="success">Submit</b-button>
  </b-form>
</template>

<script>
import axios from 'axios';

export default {
  name: 'UpdateInfo',
  data() {
    return {
      newName: this.$parent.name,
      newEmail: this.$parent.email,
    };
  },
  methods: {
    updateUser(e) {
      e.preventDefault();
      const req = axios.create({
        withCredentials: true,
      });
      const requestData = this.getRequestData();
      req.put(this.$endpoints.user, requestData, {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(() => {
          this.$parent.updateUser();
        })
        .catch((error) => {
          console.log(`UserProfile.vue:acceptHim: ${error}`);
        });
    },
    getRequestData() {
      const request = {};
      if (this.newName !== '') {
        request.name = this.newName;
      }
      if (this.newEmail !== '') {
        request.email = this.newEmail;
      }
      return JSON.stringify(request);
    },
  },
};
</script>

<style scoped>

</style>
