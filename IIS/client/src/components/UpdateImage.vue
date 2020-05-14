<template>
  <div>
  <b-form-file
    accept="image/*"
    v-model="file"
    :state="myState"
    placeholder="Choose a file or drop it here..."
    drop-placeholder="Drop file here..."
    style="text-align: left">
  </b-form-file>
  <b-button @click="updateImage" type="submit" variant="success" class="mt-3">Submit</b-button>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'UpdateImage',
  data() {
    return {
      file: null,
    };
  },
  computed: {
    myState() {
      if (this.file === null) {
        return null;
      }
      return true;
    },
  },
  methods: {
    updateImage(e) {
      if (this.file !== '') {
        axios.post(this.$endpoints.image,
          this.file,
          {
            headers: {
              'Content-Type': 'image/*',
            },
          })
          .then(() => this.$parent.updateUser())
          .catch(error => console.log(error));
      } else {
        e.preventDefault();
      }
    },
  },
};
</script>

<style scoped>

</style>
