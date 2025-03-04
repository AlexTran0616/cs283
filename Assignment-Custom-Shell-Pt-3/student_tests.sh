#!/usr/bin/env bats

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF
ls
EOF

    [ "$status" -eq 0 ]
}

@test "Built-in command: exit" {
    run ./dsh <<EOF
exit
EOF

    [ "$status" -eq 0 ]
}

@test "Built-in command: cd with valid path" {
    run ./dsh <<EOF
cd ..
EOF

    [ "$status" -eq 0 ]
}

@test "Built-in command: cd with invalid path" {
    run ./dsh <<EOF
cd awjfofjo
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"cd: awjfofjo: No such file or directory"* ]]
}

@test "Error: unmatched quotes" {
    run ./dsh <<EOF
echo "unmatched
EOF

    [[ "$output" == *"error: unmatched quotes"* ]]
}

@test "Echo with single quotes" {
    run ./dsh <<EOF
echo 'hello world' > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"hello world"* ]]
}

@test "Echo with double quotes" {
    run ./dsh <<EOF
echo "hello world" > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"hello world"* ]]
}

@test "Input redirection" {
    run ./dsh <<EOF
echo "hello world" > input.txt
cat < input.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"hello world"* ]]
}

@test "Output redirection" {
    run ./dsh <<EOF
echo "hello world" > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"hello world"* ]]
}

@test "Append redirection" {
    run ./dsh <<EOF
echo "first line" > append.txt
echo "second line" >> append.txt
cat append.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"first line"* ]]
    [[ "${output}" == *"second line"* ]]
}

@test "Pipe command" {
    run ./dsh <<EOF
echo "hello world" | tr '[:lower:]' '[:upper:]'
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"HELLO WORLD"* ]]
}

@test "Empty input" {
    run ./dsh <<EOF

EOF

    [ "$status" -eq 0 ]
}

@test "Multiple spaces" {
    run ./dsh <<EOF
echo    'hello    world' > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"hello    world"* ]]
}

@test "Multiple pipes" {
    run ./dsh <<EOF
echo "hello" | tr '[:lower:]' '[:upper:]' | rev
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"OLLEH"* ]]
}

@test "Invalid command" {
    run ./dsh <<EOF
invalid_command
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"execvp: No such file or directory"* ]]
}

@test "Redirection with pipes" {
    run ./dsh <<EOF
echo "hello world" | tr '[:lower:]' '[:upper:]' > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"HELLO WORLD"* ]]
}

# Combination of piping and redirection
@test "Combination of piping and redirection" {
    run ./dsh <<EOF
echo "hello world" | tr '[:lower:]' '[:upper:]' | rev > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"DLROW OLLEH"* ]]
}

# Built-in command with piping
@test "Built-in command with piping" {
    run ./dsh <<EOF
cd .. | ls
EOF

    [ "$status" -eq 0 ]
}

# Special characters in arguments
@test "Special characters in arguments" {
    run ./dsh <<EOF
echo 'hello @#\$%^&*()_+' > out.txt
cat out.txt
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"hello @#\$%^&*()_+"* ]]
}

@test "Too many pipes used" {
    run ./dsh <<EOF
echo "cmd1" | echo "cmd2" | echo "cmd3" | echo "cmd4" | echo "cmd5" | echo "cmd6" | echo "cmd7" | echo "cmd8" | echo "cmd9"
EOF

    [ "$status" -eq 0 ]
    [[ "${output}" == *"error: piping limited to 8 commands"* ]]
}

