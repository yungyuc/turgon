#!/bin/bash

mypypath="$(cd "$(dirname "${BASH_SOURCE[0]}")"/../spacetime && pwd)"
export PYTHONPATH="$mypypath:$PYTHONPATH"

jupyter notebook "$@"
