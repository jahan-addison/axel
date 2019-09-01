.PHONY: type lint test start

project = axel

type:
	poetry run python -m mypy --ignore-missing-imports **/*.py

lint: type
	poetry run python -m flake8 --exclude $(project)/ext --ignore $(project)/

test: type
	PYTHONPATH=./$(project) poetry run pytest $(project)/tests

start: lint
	poetry run python $(shell pwd)/$(project)/__main__.py

