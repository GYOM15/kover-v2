setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------
@test "kover summarize runs correctly on an empty scene" {
  run kover summarize < "$examples_dir"/empty.scene
  assert_success
  assert_output "An empty scene"
}

@test "kover summarize runs correctly on a scene with 1 building" {
  run kover summarize < "$examples_dir"/1b.scene
  assert_success
  assert_output "A scene with 1 building"
}

@test "kover summarize runs correctly on a scene with 2 buildings" {
  run kover summarize < "$examples_dir"/2b.scene
  assert_success
  assert_output "A scene with 2 buildings"
}

@test "kover summarize runs correctly on a scene with 1 antenna" {
  run kover summarize < "$examples_dir"/1a.scene
  assert_success
  assert_output "A scene with 1 antenna"
}

@test "kover summarize runs correctly on a scene with 2 antennas" {
  run kover summarize < "$examples_dir"/2a.scene
  assert_success
  assert_output "A scene with 2 antennas"
}

@test "kover summarize runs correctly on a scene with 1 building and 1 antenna" {
  run kover summarize < "$examples_dir"/1b1a.scene
  assert_success
  assert_output "A scene with 1 building and 1 antenna"
}

# Tests for houses
# ---------------

@test "kover summarize runs correctly on a scene with 1 house" {
  run kover summarize < "$examples_dir"/1h.scene
  assert_success
  assert_output "A scene with 1 house"
}

@test "kover summarize runs correctly on a scene with 2 houses" {
  run kover summarize < "$examples_dir"/2h.scene
  assert_success
  assert_output "A scene with 2 houses"
}

@test "kover summarize runs correctly on a scene with mixed buildings and houses" {
  run kover summarize < "$examples_dir"/mixed_bh.scene
  assert_success
  assert_output "A scene with 1 building, 1 house and 1 antenna"
}
