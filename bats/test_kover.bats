setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "kover runs with success" {
  kover
}

@test "kover runs correctly on an empty scene" {
  run kover < "$examples_dir"/empty.scene
  assert_success
  assert_output "An empty scene"
}
