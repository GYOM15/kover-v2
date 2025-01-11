setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
}

# Wrong usage
# -----------

@test "kover without subcommand reports wrong usage" {
  run kover
  assert_failure
  assert_output "error: subcommand is mandatory"
}
