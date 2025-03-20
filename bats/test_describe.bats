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

# Tests for houses
# ---------------

@test "kover describe runs correctly on a scene with 1 house" {
  run kover describe < "$examples_dir"/1h.scene
  assert_success
  assert_line --index 0 "A scene with 1 house"
  assert_line --index 1 "  house h1 at 0 0 with dimensions 1 1"
}

@test "kover describe runs correctly on a scene with 2 houses" {
  run kover describe < "$examples_dir"/2h.scene
  assert_success
  assert_line --index 0 "A scene with 2 houses"
  assert_line --index 1 "  house h1 at 0 0 with dimensions 1 1"
  assert_line --index 2 "  house h2 at 5 8 with dimensions 2 3"
}

@test "kover describe runs correctly on a scene with mixed buildings and houses" {
  run kover describe < "$examples_dir"/mixed_bh.scene
  assert_success
  assert_line --index 0 "A scene with 1 building, 1 house and 1 antenna"
  assert_line --index 1 "  building b1 at 0 0 with dimensions 1 2"
  assert_line --index 2 "  house h1 at 5 8 with dimensions 2 3"
  assert_line --index 3 "  antenna a1 at 2 3 with range 4"
}