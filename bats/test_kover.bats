setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
}

function filter_stderr_out {
  "$@" 2> /dev/null
}

# Wrong usage
# -----------

@test "kover without subcommand reports wrong usage" {
  run kover
  [ "$status" -eq 1 ]
  assert_output "error: subcommand is mandatory"
}

@test "kover without subcommand reports wrong usage on stderr" {
  run filter_stderr_out kover
  assert_output ""
}

@test "kover with unrecognized subcommand reports wrong usage" {
  run kover thing
  [ "$status" -eq 1 ]
  assert_output "error: subcommand 'thing' is not recognized"
}
