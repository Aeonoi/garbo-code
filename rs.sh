#!/bin/bash
# rs.sh

cat rs.rs
rustc rs.rs -o rs_bin
./rs_bin
