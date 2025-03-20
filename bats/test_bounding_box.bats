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

# Test for houses
# --------------

@test "kover bounding-box runs correctly on a scene with 1 house" {
  run kover bounding-box < "$examples_dir"/1h.scene
  assert_success
  assert_output "bounding box [-1, 1] x [-1, 1]"
}

@test "kover bounding-box runs correctly on a scene with 2 houses" {
  run kover bounding-box < "$examples_dir"/2h.scene
  assert_success
  assert_output "bounding box [-1, 7] x [-1, 11]"
}

@test "kover bounding-box runs correctly on a scene with mixed buildings and houses" {
  run kover bounding-box < "$examples_dir"/mixed_bh.scene
  assert_success
  assert_output "bounding box [-2, 7] x [-2, 11]"
}
