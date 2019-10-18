.PHONY: type lint test start

project = axel

type:
	poetry run python -m mypy --strict --ignore-missing-imports $(project)/*.py

lint: type
	poetry run python -m flake8 --ignore E704,F811 $(project)/*.py

test:
	poetry run pytest --disable-warnings -s

start: lint
	poetry run python $(shell pwd)/$(project)/__main__.py

