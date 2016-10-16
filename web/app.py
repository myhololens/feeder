# app.py

import logging

import requests
from app_config import BaseConfig
from flask import Flask
from flask import request, render_template, redirect, url_for
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config.from_object(BaseConfig)
db = SQLAlchemy(app)
console = logging.StreamHandler()
log = logging.getLogger("app")
log.addHandler(console)
log.setLevel(logging.DEBUG)

from models import *


@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html', feeders=_get_all_feeders())


@app.route('/feeder', methods=['GET', 'POST'])
@app.route('/feeder/add', methods=['GET', 'POST'])
def feeder():
    if request.method == 'POST':
        log.debug(request.form)
        feeder_type = request.form.get('type')
        address = request.form.get('address')
        name = request.form.get('name')
        port = request.form.get('port')
        db.session.add(Feeder(type=feeder_type, name=name, address=address, port=port))
        db.session.commit()
    return render_template('feeder.html', feeders=_get_all_feeders())


@app.route('/feeder/delete_last', methods=['POST'])
def feeder_delete_last():
    if request.method == 'POST':
        log.debug(request.form)
        db.session.delete(Feeder.query.order_by(Feeder.id.desc()).first())
        db.session.commit()

    return redirect(url_for('index'))


@app.route('/feeder/move', methods=['POST'])
def feeder_move():
    if request.method == 'POST':
        feeder_name = request.form.get('feeder_name')

        direction = request.form.get('direction')
        log.debug(request.form)

        feeder = Feeder.query.filter_by(name=feeder_name).first()
        log.debug(feeder)
        if not feeder:
            redirect(url_for('index'))
        r = requests.get("http://{address}:{port}/?{direction}".format(address=feeder.address,
                                                                       port=feeder.port,
                                                                       direction=direction))
        response_data= r.text
        response_status = r.status_code
        content_type = r.headers['content-type']

        log.debug(response_data)
        log.debug(response_status)

        return redirect(url_for('index'))

def _get_all_feeders():
    return Feeder.query.order_by(Feeder.address).all()

if __name__ == '__main__':
    app.run()
