from connexion import FlaskApp
from flask_cors import CORS


def create_app():
    app = FlaskApp(__name__, specification_dir="openapi/")
    CORS(app.app, resources={r"*": {"origins": "http://localhost:8080"}}, supports_credentials=True)
    app.add_api("api.yaml")
    return app
