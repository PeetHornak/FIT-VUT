from fitcado.api import api

if __name__ == "__main__":
    APP = api.create_app()
    APP.run(debug=True, port=8080)
