setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  DIR="$(cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd)"
  PATH="$DIR/../bin:$PATH"
}

@test "kover runs with success" {
  kover
}
