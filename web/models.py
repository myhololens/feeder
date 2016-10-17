# models.py


import datetime

from app import db


class Feeder(db.Model):
    __tablename__ = 'feeders'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String, nullable=False)
    address = db.Column(db.String, nullable=False)
    type = db.Column(db.String, nullable=False, default='wet')
    port = db.Column(db.String, nullable=False, default='80')

    def __init__(self, name, address, port, type):
        self.name = name
        self.address = address
        self.type = type
        self.port = port


class Voltage(db.Model):
    __tablename__ = 'voltage_data'

    id = db.Column(db.Integer, primary_key=True)
    date_posted = db.Column(db.DateTime, nullable=False)
    value = db.Column(db.Float, nullable=False)

    def __init__(self, value):
        self.value = float(value)
        self.date_posted = datetime.datetime.now()
