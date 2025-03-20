setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

# Test quality command with grade A coverage (all 4 corners covered)
@test "kover quality reports grade A for a building with all corners covered" {
  run kover quality < "$examples_dir"/quality_A.scene
  assert_success
  assert_line --index 0 "building b1: A"
}

# Test quality command with grade B coverage (3 corners covered)
@test "kover quality reports grade B for a building with three corners covered" {
  run kover quality < "$examples_dir"/quality_B.scene
  assert_success
  assert_line --index 0 "building b1: B"
}

# Test quality command with grade C coverage (2 corners covered)
@test "kover quality reports grade C for a building with two corners covered" {
  run kover quality < "$examples_dir"/quality_C.scene
  assert_success
  assert_line --index 0 "building b1: C"
}

# Test quality command with grade D coverage (1 corner covered)
@test "kover quality reports grade D for a building with one corner covered" {
  run kover quality < "$examples_dir"/quality_D.scene
  assert_success
  assert_line --index 0 "building b1: D"
}

# Test quality command with grade E coverage (0 corners covered)
@test "kover quality reports grade E for a building with no corners covered" {
  run kover quality < "$examples_dir"/quality_E.scene
  assert_success
  assert_line --index 0 "building b1: E"
}

# Test quality command with mixed scene (multiple buildings and houses)
@test "kover quality reports correct grades for a mixed scene with buildings and houses" {
  run kover quality < "$examples_dir"/quality_mixed.scene
  assert_success
  assert_line --index 0 "building b1: A"
  assert_line --index 1 "building b2: E"
  assert_line --index 2 "house h1: E"
  assert_line --index 3 "house h2: D"
}

# Test quality command with empty scene (no buildings or houses)
@test "kover quality runs correctly on an empty scene" {
  run kover quality < "$examples_dir"/quality_empty.scene
  assert_success
  [ "${#lines[@]}" -eq 0 ]
}

# Test quality command with multiple antennas for single structure
@test "kover quality reports correct grade with multiple antenna coverage" {
  run kover quality < "$examples_dir"/quality_multiple_antennas.scene
  assert_success
  assert_line --index 0 "building b1: B"
}

# Test quality command with houses only
@test "kover quality reports correct grades for houses only" {
  run kover quality < "$examples_dir"/quality_houses.scene
  assert_success
  assert_line --index 0 "house h1: A"
  assert_line --index 1 "house h2: E"
}

# Test quality command with overlapping antennas
@test "kover quality reports correct grade with overlapping antenna coverage" {
  run kover quality < "$examples_dir"/quality_overlap.scene
  assert_success
  assert_line --index 0 "building b1: A"
}

# Test quality command with partially missing coverage
@test "kover quality reports correct grade with partially missing coverage" {
  run kover quality < "$examples_dir"/quality_partial.scene
  assert_success
  assert_line --index 0 "building b1: B"
}

# Test quality command with invalid scene
@test "kover quality reports an error with invalid scene" {
  run kover quality < "$examples_dir"/quality_invalid.scene
  [ "$status" -eq 1 ]
  assert_line --index 1 "not ok"
}

# Test quality command with large structures
@test "kover quality reports correct grades for large structures" {
  run kover quality < "$examples_dir"/quality_large.scene
  assert_success
  assert_line --index 0 "building b1: A"
  assert_line --index 1 "house h1: E"  
}

# Test quality command with edge cases
@test "kover quality reports correct grade with edge case antenna coverage" {
  run kover quality < "$examples_dir"/quality_edge.scene
  assert_success
  assert_line --index 0 "building b1: C"
}
