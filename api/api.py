from flask import Flask, request, g
from flask_sqlalchemy import SQLAlchemy
from flask_restful import Resource, Api, reqparse, fields, marshal_with
from datetime import datetime
from flask_cors import CORS
from functools import wraps

app = Flask(__name__)
CORS(app)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///shafa_multi_table.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
api = Api(app)


class UserModel(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True, nullable=False)
    pin = db.Column(db.String(4), unique=True, nullable=False) 
    created_at = db.Column(db.DateTime, default=datetime.now)

    def __repr__(self):
        return f"<User {self.username}>"

class DeviceModel(db.Model):
    __tablename__ = 'devices'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), unique=True, nullable=False)
    location = db.Column(db.String(200))

    def __repr__(self):
        return f"<Device {self.name}>"

class UnlockMethodModel(db.Model):
    __tablename__ = 'unlock_methods'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(50), unique=True, nullable=False)

    def __repr__(self):
        return f"<Method {self.name}>"

class AccessLogModel(db.Model):
    __tablename__ = 'access_logs'
    id = db.Column(db.Integer, primary_key=True)
    timestamp = db.Column(db.DateTime, default=datetime.now)
    is_success = db.Column(db.Boolean, nullable=False)
    
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    device_id = db.Column(db.Integer, db.ForeignKey('devices.id'), nullable=False)
    method_id = db.Column(db.Integer, db.ForeignKey('unlock_methods.id'), nullable=False)

    user = db.relationship('UserModel', backref=db.backref('logs', lazy=True))
    device = db.relationship('DeviceModel', backref=db.backref('logs', lazy=True))
    method = db.relationship('UnlockMethodModel', backref=db.backref('logs', lazy=True))

    def __repr__(self):
        return f"<Log ID: {self.id} User: {self.user.username} Success: {self.is_success}>"


def pin_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        pin = request.headers.get('X-Auth-PIN')
        if not pin:
            return {'message': 'PIN-код відсутній у заголовку X-Auth-PIN'}, 401
        
        user = UserModel.query.filter_by(pin=pin).first()
        if not user:
            return {'message': 'Невірний PIN-код або користувач не знайдений'}, 401
        
        g.user = user
        return f(*args, **kwargs)
    return decorated_function


user_fields = {
    'id': fields.Integer,
    'username': fields.String,
    'created_at': fields.DateTime(dt_format='iso8601')
}

device_fields = {
    'id': fields.Integer,
    'name': fields.String,
    'location': fields.String
}

unlock_method_fields = {
    'id': fields.Integer,
    'name': fields.String
}

access_log_fields = {
    'id': fields.Integer,
    'timestamp': fields.DateTime(dt_format='iso8601'),
    'is_success': fields.Boolean,
    'user': fields.Nested(user_fields),
    'device': fields.Nested(device_fields),
    'method': fields.Nested(unlock_method_fields)
}

user_parser = reqparse.RequestParser()
user_parser.add_argument('username', type=str, required=True, help='Ім\'я користувача не може бути пустим')
user_parser.add_argument('pin', type=str, required=True, help='PIN-код не може бути пустим')

device_parser = reqparse.RequestParser()
device_parser.add_argument('name', type=str, required=True, help='Назва пристрою не може бути пустою')
device_parser.add_argument('location', type=str, required=False)

access_log_parser = reqparse.RequestParser()
access_log_parser.add_argument('device_id', type=int, required=True, help='ID пристрою є обов\'язковим')
access_log_parser.add_argument('method_id', type=int, required=True, help='ID методу розблокування є обов\'язковим')
access_log_parser.add_argument('is_success', type=bool, required=True, help='Статус успішності є обов\'язковим')



class UserListResource(Resource):
    @marshal_with(user_fields)
    def get(self):
        return UserModel.query.all()

    @marshal_with(user_fields)
    def post(self):
        args = user_parser.parse_args()
        if len(args['pin']) != 4 or not args['pin'].isdigit():
             return {'message': 'PIN має складатися рівно з 4 цифр'}, 400
        if UserModel.query.filter_by(username=args['username']).first():
            return {'message': f"Користувач з ім'ям '{args['username']}' вже існує"}, 409
        if UserModel.query.filter_by(pin=args['pin']).first():
            return {'message': 'Цей PIN-код вже використовується'}, 409
            
        new_user = UserModel(username=args['username'], pin=args['pin'])
        db.session.add(new_user)
        db.session.commit()
        return new_user, 201

class DeviceListResource(Resource):
    @marshal_with(device_fields)
    def get(self):
        return DeviceModel.query.all()
    
    @marshal_with(device_fields)
    def post(self):
        args = device_parser.parse_args()
        if DeviceModel.query.filter_by(name=args['name']).first():
            return {'message': f"Пристрій з назвою '{args['name']}' вже існує"}, 409

        new_device = DeviceModel(name=args['name'], location=args.get('location'))
        db.session.add(new_device)
        db.session.commit()
        return new_device, 201

class ShafaDataResource(Resource):
    @pin_required
    @marshal_with(access_log_fields)
    def post(self):
        args = access_log_parser.parse_args()

        device = DeviceModel.query.get(args['device_id'])
        if not device:
            return {'message': 'Пристрій з таким ID не знайдено'}, 404

        method = UnlockMethodModel.query.get(args['method_id'])
        if not method:
            return {'message': 'Метод з таким ID не знайдено'}, 404

        new_log = AccessLogModel(
            user_id=g.user.id,
            device_id=device.id,
            method_id=method.id,
            is_success=args['is_success'],
            timestamp=datetime.now()
        )
        db.session.add(new_log)
        db.session.commit()
        return new_log, 201


class AccessLogListResource(Resource):
    @marshal_with(access_log_fields)
    def get(self):
        return AccessLogModel.query.order_by(AccessLogModel.timestamp.desc()).all()

    @pin_required
    @marshal_with(access_log_fields)
    def post(self):
        args = access_log_parser.parse_args()
        
        if not DeviceModel.query.get(args['device_id']):
            return {'message': 'Пристрій з таким ID не знайдено'}, 404
        if not UnlockMethodModel.query.get(args['method_id']):
            return {'message': 'Метод з таким ID не знайдено'}, 404
            
        new_log = AccessLogModel(
            user_id=g.user.id,
            device_id=args['device_id'],
            method_id=args['method_id'],
            is_success=args['is_success'],
            timestamp=datetime.now()
        )
        db.session.add(new_log)
        db.session.commit()
        return new_log, 201


api.add_resource(UserListResource, '/api/users')
api.add_resource(DeviceListResource, '/api/devices')
api.add_resource(AccessLogListResource, '/api/logs')
api.add_resource(ShafaDataResource, '/api/shafa_data/')



@app.route("/")
def home():
    return '<h1>Flask API with 4 tables and PIN auth</h1>'

if __name__ == '__main__':
    with app.app_context():
        db.create_all()

        if not UserModel.query.first():
            test_user = UserModel(username='testuser', pin='1234')
            db.session.add(test_user)
            print("Created test user: testuser (PIN: 1234)")

        if not DeviceModel.query.first():
            test_device = DeviceModel(name='ESP32_Locker', location='Warehouse A')
            db.session.add(test_device)
            print("Created test device: ESP32_Locker")

        if not UnlockMethodModel.query.first():
            print("Creating default unlock methods...")
            db.session.add(UnlockMethodModel(name='PIN'))
            db.session.add(UnlockMethodModel(name='WEB'))

        db.session.commit()

    app.run(debug=True, host="0.0.0.0")
