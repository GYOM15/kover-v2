setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

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
