from flask import Flask, jsonify, request
from flask_cors import CORS
import turingapi
from turingapi.Request import TuringError

app = Flask(
    __name__,
    static_url_path="",
    static_folder="frontend/build",
    template_folder="frontend/build",
)
CORS(app, support_credentials=True)

turing = turingapi.Turing("localhost:6666")


@app.route("/api/get_status")
def get_status():
    return jsonify({"running": turing.running})


@app.route("/api/list_available_databases")
def list_available_databases():
    try:
        dbs = turing.list_available_databases()
    except TuringError as err:
        return jsonify({"failed": True, "error": {"details": err.details}})

    return jsonify(
        {
            "failed": False,
            "db_names": list(dbs),
        }
    )


@app.route("/api/list_loaded_databases")
def list_loaded_databases():
    try:
        dbs = turing.list_loaded_databases()
    except TuringError as err:
        return jsonify({"failed": True, "error": {"details": err.details}})

    return jsonify(
        {
            "failed": False,
            "db_names": list(dbs),
        }
    )


@app.route("/api/is_db_loaded", methods=["GET"])
def is_db_loaded():
    try:
        loaded_dbs = turing.list_loaded_databases()
        return jsonify({"loaded": request.args.get("db_name") in loaded_dbs})

    except TuringError as err:
        return jsonify({"failed": True, "error": {"details": err.details}})


@app.route("/api/load_database", methods=["POST"])
def load_database():
    db_name = request.get_json()["db_name"]
    try:
        db = turing.load_db(db_name=db_name)
    except TuringError as err:
        return jsonify({"failed": True, "error": {"details": err.details}})

    return jsonify(
        {
            "db_name": db_name,
            "db_id": db.id,
        }
    )

@app.route("/api/get_database", methods=["POST"])
def get_database():
    db_name = request.get_json()["db_name"]
    try:
        db = turing.get_db(db_name=db_name)
    except TuringError as err:
        return jsonify({"failed": True, "error": {"details": err.details}})

    return jsonify(
        {
            "db_name": db_name,
            "db_id": db.id,
        }
    )



if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0")
