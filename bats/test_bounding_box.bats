setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

@test "kover bounding-box runs correctly on an empty scene" {
  run kover bounding-box < "$examples_dir"/empty.scene
  assert_success
  assert_output "undefined (empty scene)"
}

@test "kover bounding-box runs correctly on a scene with 1 building" {
  run kover bounding-box < "$examples_dir"/1b.scene
  assert_success
  assert_output "bounding box [-1, 1] x [-1, 1]"
}

@test "kover bounding-box runs correctly on a scene with 2 buildings" {
  run kover bounding-box < "$examples_dir"/2b.scene
  assert_success
  assert_output "bounding box [-1, 7] x [-1, 11]"
}

@test "kover bounding-box runs correctly on a scene with 1 antenna" {
  run kover bounding-box < "$examples_dir"/1a.scene
  assert_success
  assert_output "bounding box [-1, 1] x [-1, 1]"
}

@test "kover bounding-box runs correctly on a scene with 2 antennas" {
  run kover bounding-box < "$examples_dir"/2a.scene
  assert_success
  assert_output "bounding box [-3, 7] x [-2, 8]"
}

@test "kover bounding-box runs correctly on a scene with 1 building and 1 antenna" {
  run kover bounding-box < "$examples_dir"/1b1a.scene
  assert_success
  assert_output "bounding box [-3, 7] x [-2, 8]"
}

# Wrong usage
# -----------

@test "kover bounding-box reports an error when first line is invalid" {
  run kover bounding-box < "$examples_dir"/first_line.invalid
  assert_failure
  assert_output "error: first line must be exactly 'begin scene'"
}

@test "kover bounding-box reports an error when a line is unrecognized" {
  run kover bounding-box < "$examples_dir"/unrecognized_line.invalid
  assert_failure
  assert_output "error: unrecognized line (line #2)"
}

@test "kover bounding-box reports an error when last line is not 'end scene'" {
  run kover bounding-box < "$examples_dir"/no_end_scene.invalid
  assert_failure
  assert_output "error: last line must be exactly 'end scene'"
}

@test "kover bounding-box reports an error when two buildings have same id" {
  run kover bounding-box < "$examples_dir"/2b_non_unique_id.invalid
  assert_failure
  assert_output "error: building identifier b1 is non unique"
}

@test "kover bounding-box reports an error when two buildings are overlapping" {
  run kover bounding-box < "$examples_dir"/2b_overlapping.invalid
  assert_failure
  assert_output "error: buildings b1 and b2 are overlapping"
}
