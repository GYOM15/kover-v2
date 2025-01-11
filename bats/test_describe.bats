setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

@test "kover describe runs correctly on an empty scene" {
  run kover describe < "$examples_dir"/empty.scene
  assert_success
  assert_output "An empty scene"
}

@test "kover describe runs correctly on a scene with 1 building" {
  run kover describe < "$examples_dir"/1b.scene
  assert_success
  assert_line --index 0 "A scene with 1 building"
  assert_line --index 1 "  building b1 at 0 0 with dimensions 1 1"
}

@test "kover describe runs correctly on a scene with 2 buildings" {
  run kover describe < "$examples_dir"/2b.scene
  assert_success
  assert_line --index 0 "A scene with 2 buildings"
  assert_line --index 1 "  building b1 at 0 0 with dimensions 1 1"
  assert_line --index 2 "  building b2 at 5 8 with dimensions 2 3"
}

@test "kover describe runs correctly on a scene with 2 buildings in reverse order" {
  run kover describe < "$examples_dir"/2b_rev.scene
  assert_success
  assert_line --index 0 "A scene with 2 buildings"
  assert_line --index 1 "  building b1 at 0 0 with dimensions 1 1"
  assert_line --index 2 "  building b2 at 5 8 with dimensions 2 3"
}

@test "kover describe runs correctly on a scene with 3 buildings in reverse order" {
  run kover describe < "$examples_dir"/3b_rev.scene
  assert_success
  assert_line --index 0 "A scene with 3 buildings"
  assert_line --index 1 "  building b1 at 0 0 with dimensions 1 1"
  assert_line --index 2 "  building b2 at 5 8 with dimensions 2 3"
  assert_line --index 3 "  building b3 at -2 -2 with dimensions 1 1"
}

@test "kover describe runs correctly on a scene with 1 antenna" {
  run kover describe < "$examples_dir"/1a.scene
  assert_success
  assert_line --index 0 "A scene with 1 antenna"
  assert_line --index 1 "  antenna a1 at 0 0 with range 1"
}

@test "kover describe runs correctly on a scene with 2 antennas" {
  run kover describe < "$examples_dir"/2a.scene
  assert_success
  assert_line --index 0 "A scene with 2 antennas"
  assert_line --index 1 "  antenna a1 at 0 0 with range 1"
  assert_line --index 2 "  antenna a2 at 2 3 with range 5"
}

@test "kover describe runs correctly on a scene with 2 antennas in reverse order" {
  run kover describe < "$examples_dir"/2a_rev.scene
  assert_success
  assert_line --index 0 "A scene with 2 antennas"
  assert_line --index 1 "  antenna a1 at 0 0 with range 1"
  assert_line --index 2 "  antenna a2 at 2 3 with range 5"
}

@test "kover describe runs correctly on a scene with 1 building and 1 antenna" {
  run kover describe < "$examples_dir"/1b1a.scene
  assert_success
  assert_line --index 0 "A scene with 1 building and 1 antenna"
  assert_line --index 1 "  building b1 at 0 0 with dimensions 1 1"
  assert_line --index 2 "  antenna a1 at 2 3 with range 5"
}

# Wrong usage
# -----------

@test "kover describe reports an error when first line is invalid" {
  run kover describe < "$examples_dir"/first_line.invalid
  assert_failure
  assert_output "error: first line must be exactly 'begin scene'"
}

@test "kover describe reports an error when a line is unrecognized" {
  run kover describe < "$examples_dir"/unrecognized_line.invalid
  assert_failure
  assert_output "error: unrecognized line (line #2)"
}

@test "kover describe reports an error when last line is not 'end scene'" {
  run kover describe < "$examples_dir"/no_end_scene.invalid
  assert_failure
  assert_output "error: last line must be exactly 'end scene'"
}

@test "kover describe reports an error when two buildings have same id" {
  run kover describe < "$examples_dir"/2b_non_unique_id.invalid
  assert_failure
  assert_output "error: building identifier b1 is non unique"
}

@test "kover describe reports an error when two buildings are overlapping" {
  run kover describe < "$examples_dir"/2b_overlapping.invalid
  assert_failure
  assert_output "error: buildings b1 and b2 are overlapping"
}
