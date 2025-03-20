setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

@test "kover validate runs correctly on a valid scene with an empty scene" {
  run kover validate < "$examples_dir"/empty.scene
  assert_success
  assert_output "ok"
}

@test "kover validate runs correctly on a valid scene with 1 building" {
  run kover validate < "$examples_dir"/1b.scene
  assert_success
  assert_output "ok"
}

@test "kover validate runs correctly on a valid scene with 2 buildings" {
  run kover validate < "$examples_dir"/2b.scene
  assert_success
  assert_output "ok"
}

@test "kover validate runs correctly on a valid scene with 1 antenna" {
  run kover validate < "$examples_dir"/1a.scene
  assert_success
  assert_output "ok"
}

@test "kover validate runs correctly on a valid scene with 2 antennas" {
  run kover validate < "$examples_dir"/2a.scene
  assert_success
  assert_output "ok"
}

@test "kover validate runs correctly on a valid scene with 1 building and 1 antenna" {
  run kover validate < "$examples_dir"/1b1a.scene
  assert_success
  assert_output "ok"
}

# Wrong lines
# -----------

@test "kover validate reports an error when first line is invalid" {
  run kover validate < "$examples_dir"/first_line.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: first line must be exactly 'begin scene'"
}

@test "kover validate reports an error when a line is unrecognized" {
  run kover validate < "$examples_dir"/unrecognized_line.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: unrecognized line (line #2)"
}

@test "kover validate reports an error when last line is not 'end scene'" {
  run kover validate < "$examples_dir"/no_end_scene.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: last line must be exactly 'end scene'"
}

# Wrong buildings
# ---------------

@test "kover validate reports an error when two buildings have same id" {
  run kover validate < "$examples_dir"/2b_non_unique_id.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: building identifier b1 is non unique"
}

@test "kover validate reports an error when two buildings are overlapping" {
  run kover validate < "$examples_dir"/2b_overlapping.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: buildings b1 and b2 are overlapping"
}

@test "kover validate reports an error when a building line has a wrong number of arguments" {
  run kover validate < "$examples_dir"/1b_wrong_number_of_arguments.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: building line has wrong number of arguments (line #2)"
}

@test "kover validate reports an error when a building line has an invalid identifier" {
  run kover validate < "$examples_dir"/1b_wrong_id.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid identifier "b^" (line #2)'
}

@test "kover validate reports an error when a building line has an invalid x" {
  run kover validate < "$examples_dir"/1b_wrong_x.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid integer "a" (line #2)'
}

@test "kover validate reports an error when a building line has an invalid y" {
  run kover validate < "$examples_dir"/1b_wrong_y.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid integer "02" (line #2)'
}

@test "kover validate reports an error when a building line has an invalid w" {
  run kover validate < "$examples_dir"/1b_wrong_w.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid positive integer "c" (line #2)'
}

@test "kover validate reports an error when a building line has an invalid h" {
  run kover validate < "$examples_dir"/1b_wrong_h.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid positive integer "-2" (line #2)'
}

# Wrong antennas
# --------------

@test "kover validate reports an error when two antennas have same id" {
  run kover validate < "$examples_dir"/2a_non_unique_id.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: antenna identifier a1 is non unique"
}

@test "kover validate reports an error when two antennas have the same position" {
  run kover validate < "$examples_dir"/2a_same_position.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: antennas a1 and a2 have the same position"
}

@test "kover validate reports an error when an antenna line has the wrong number of arguments" {
  run kover validate < "$examples_dir"/1a_wrong_number_of_arguments.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 "error: antenna line has wrong number of arguments (line #2)"
}

@test "kover validate reports an error when an antenna line has an invalid identifier" {
  run kover validate < "$examples_dir"/1a_wrong_id.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid identifier "a^" (line #2)'
}

@test "kover validate reports an error when a antenna line has an invalid x" {
  run kover validate < "$examples_dir"/1a_wrong_x.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid integer "00" (line #2)'
}

@test "kover validate reports an error when a antenna line has an invalid y" {
  run kover validate < "$examples_dir"/1a_wrong_y.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid integer "2t" (line #2)'
}

@test "kover validate reports an error when a antenna line has an invalid r" {
  run kover validate < "$examples_dir"/1a_wrong_r.invalid
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
  assert_line --index 0 'error: invalid positive integer "-1" (line #2)'
}
