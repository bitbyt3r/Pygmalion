all: run

run: venv/bin/pygmalion-server conf.json
	venv/bin/pygmalion-server -c conf.json

venv/bin/pygmalion-server: venv
	venv/bin/python setup.py develop

conf.json:
	cp contrib/conf.json conf.json
	echo "Copied default conf.json from /contrib/conf.json. Feel free to edit it to your heart's content."

tables: venv
	venv/bin/python ./create-table-migration.py

venv: venv/bin/activate

venv/bin/activate: requirements.txt
	python3 -m venv venv
	venv/bin/pip install -Ur requirements.txt
	touch venv/bin/activate

clean:
	-rm -rf venv
	-find . -name \*.pyc -delete
	-find . -name __pycache__ -delete
	-rm -rf dist
	-rm *.rpm
	-rm -rf pygmalion.egg-info/
	-rm -rf build/

venv/bin/crossbar: venv
	venv/bin/pip install crossbar
	venv/bin/crossbar init

crossbar: venv/bin/crossbar
	venv/bin/crossbar start
