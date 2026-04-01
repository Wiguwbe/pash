#!/bin/bash

set -euo pipefail

this_file="$(realpath "$0")"
this_dir="$(dirname $this_file)"

PASH="$(realpath "$this_dir/../pash")"

tmpdir=
cleanup() {
	test -d "$tmpdir" && rm -rf "$tmpdir"
}
tmpdir="$(mktemp -d)"
trap cleanup EXIT

export TMPDIR="$tmpdir"

run_single() {
	name=$1

	input_file="$this_dir/$name.in"
	expect_file="$this_dir/$name.yml"

	pretty_output_file="$(mktemp)"
	pretty_expected_file="$(mktemp)"

	>&2 echo "checking $name"

	AST_PRINTER=json "$PASH" "$input_file" 2>/dev/null |
		yq --no-colors --prettyPrint \
			--input-format=json \
			--output-format=yaml \
			>"$pretty_output_file"

	yq --no-colors --prettyPrint --output-format=yaml "$expect_file" \
		>"$pretty_expected_file"

	diff "$pretty_output_file" "$pretty_expected_file" &&
		>&2 echo "OK"
}

if
	test $# -ge 1
then
	for item in "$@"
	do
		run_single "$item"
	done
else
	for in_file in "$this_dir/"*.in
	do
		in_base="$(basename "$in_file")"
		in_name="${in_base%.in}"

		run_single "$in_name"
	done
fi
