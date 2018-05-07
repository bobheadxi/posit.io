.PHONY: deps

all: deps

deps:
	git submodule update --init --recursive
