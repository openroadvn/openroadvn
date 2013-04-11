#
# CSS Compressor [VERSION]
# [DATE]
# Corey Hart @ http://www.codenothing.com
#
.PHONY: benchmark test
VERSION=temp

all:
	@echo "\n\x1B[1;31mPC_LOAD_LETTER\x1B[0m\n"

test:
	@php unit/start.inc

test-all:
	@php unit/start.inc all

test-focus:
	@php unit/focus.inc

test-file:
	@php unit/file.inc

test-regression:
	@php unit/benchmark/benchmark.inc $(VERSION)

test-cli:
	@php cli/cli.inc -i cli/test/a.css > cli/test/result.css

benchmark:
	@php unit/benchmark/benchmark.inc

clean:
	@sh unit/clean.sh
