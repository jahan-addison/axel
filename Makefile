.PHONY: type lint test start

project = axel

type:
	poetry run python -m mypy --strict --ignore-missing-imports $(project)/*.py

lint: type
	poetry run python -m flake8 --ignore $(project)/*.py

test:
	PYTHONPATH=. poetry run pytest -s

start: lint
	poetry run python $(shell pwd)/$(project)/__main__.py

