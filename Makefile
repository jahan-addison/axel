.PHONY: type lint test start

project = axel

type:
	poetry run python -m mypy --strict --ignore-missing-imports $(project)/*.py test/**/*.py

lint: type
	poetry run python -m flake8 --ignore E704,F811 $(project)/*.py test/**/*.py

test: lint
	poetry run pytest --cov-branch --cov=axel --cov-fail-under=90 --disable-warnings -s

start: lint
	poetry run python $(shell pwd)/$(project)/__main__.py

