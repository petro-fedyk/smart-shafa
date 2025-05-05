from flask import  Flask
from flask_sqlalchemy import SQLAlchemy
from flask_restful import Resource, Api, reqparse, fields, marshal_with
from datetime import datetime
from flask_cors import CORS


app = Flask(__name__)
CORS(app)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///shafa.db'
db = SQLAlchemy(app)
api = Api(app)

class ShafaModel(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    time = db.Column(db.DateTime)
    unlockMethod = db.Column(db.String)
    isSuccess = db.Column(db.Boolean)
    user = db.Column(db.String)

    def __repr__(self):
        return f"Shafa(time={self.time}, unlockMethod={self.unlockMethod}, isSuccess={self.isSuccess}, user={self.user})"

def parse_datetime(value):
    try:
        return datetime.strptime(value, "%Y.%m.%d.%H:%M")
    except ValueError:
        raise ValueError("time must be in format YYYY.MM.DD.HH:MM")


shafa_args = reqparse.RequestParser()
shafa_args.add_argument('time', type=parse_datetime, required=True)
shafa_args.add_argument('unlockMethod', type=str)
shafa_args.add_argument('isSuccess', type=bool)
shafa_args.add_argument('user', type=str)

shafa_fields = {
    'id': fields.Integer,
    'time': fields.String,
    'unlockMethod': fields.String,
    'isSuccess': fields.Boolean,
    'user': fields.String,
}

class Shafa(Resource):
    @marshal_with(shafa_fields)
    def get(self):
        return ShafaModel.query.all()
    
    @marshal_with(shafa_fields)
    def post(self):
        args = shafa_args.parse_args()
        shaha_data = ShafaModel(
            time=args['time'],
            unlockMethod=args['unlockMethod'],
            isSuccess=args['isSuccess'],
            user=args['user']
        )
        db.session.add(shaha_data)
        db.session.commit()
        return ShafaModel.query.all(), 201
    
api.add_resource(Shafa, '/api/shafa_data/')


@app.route("/")
def home():
    return '<h1>Flask API<h1>'

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    app.run(debug=True, host="0.0.0.0")