#!/usr/bin/env bats

# File: student_tests.sh

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Simple command" {
    run ./dsh <<EOF
uname
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Command with arguments" {
    run ./dsh <<EOF
uname -a
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "${#output}" -gt 0 ]  # Check output is not empty
}

@test "Command with multiple spaces" {
    run ./dsh <<EOF
echo     hello
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "hello" ]
}

@test "Command with quoted arguments" {
    run ./dsh <<EOF
echo "hello,      world"
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "hello,      world" ]
}

@test "Change directory and check" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "/tmp" ]
}

@test "Invalid command" {
    run ./dsh <<EOF
invalidcmd
EOF


    # PLEASE NOTE:    THIS TEST WOULD NORMALLY RETURN A NUMBER NOT 0; HOWEVER,
    #                 IT SAYS "This week we're being naive about return codes from external commands; if there is any kind of failure, we just print the CMD_ERR_EXECUTE message."
    #                 so, I believe we just print an error message

    # Assertions
    [ "$status" -eq 0 ]  # Adjust status to match your shell's behavior
    [ "$(echo "$output" | head -n 1)" = "Command not found in PATH" ]
}

@test "Quoted argument with spaces" {
    run ./dsh <<EOF
echo "hello world"
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "hello world" ]
}

@test "Multiple commands in a row" {
    run ./dsh <<EOF
uname -a
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "${#output}" -gt 0 ]
}

@test "Mixed quotes and arguments" {
    run ./dsh <<EOF
echo hello "world"
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "hello world" ]
}

@test "Unmatched quotes" {
    run ./dsh <<EOF
echo "hello world
EOF

    # Assertions
    [[ "$output" == *"error: unmatched quotes"* ]]
    [[ "$output" == *"hello world"* ]]
}

@test "Command with special characters" {
    run ./dsh <<EOF
echo "special: !@#$%^&*()_+"
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "special: !@#$%^&*()_+" ]
}

@test "Built-in cd command: no arguments" {
    initial_dir=$(pwd)
    run ./dsh <<EOF
cd
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "$initial_dir" ]
}

@test "Built-in cd command: one argument" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | head -n 1)" = "/tmp" ]
}

@test "Built-in cd command: invalid directory" {
    run ./dsh <<EOF
cd /nonexistent
EOF

    # PLEASE NOTE:    THIS TEST WOULD NORMALLY RETURN A NUMBER NOT 0; HOWEVER,
    #                 IT SAYS "This week we're being naive about return codes from external commands; if there is any kind of failure, we just print the CMD_ERR_EXECUTE message."
    #                 so, I believe we just print an error message

    # Assertions
    [ "$status" -eq 0 ] 
    [[ "$output" == *"cd: No such file or directory"* ]]
}



@test "Built-in cd command: navigate back and forth" {
    initial_dir=$(pwd)
    run ./dsh <<EOF
cd /tmp
cd /home/at3573/cs283/Assignment-Custom-Shell-Pt-2
pwd
EOF

    output=$(echo "$output" | grep -Eo '/home/at3573/cs283/Assignment-Custom-Shell-Pt-2')

    # Assertions
    [ "$status" -eq 0 ]
    [ "$output" = "/home/at3573/cs283/Assignment-Custom-Shell-Pt-2" ]
}

@test "Built-in cd command: valid directory" {
    run ./dsh <<EOF
cd /
EOF
    
    # Assertions
    [ "$status" -eq 0 ]  # Ensure zero status for success
}


@test "Valid external command: echo" {
    run ./dsh <<EOF
echo "Hello, World!"
EOF

    cleaned_output=$(echo "$output" | sed -n '1p')

    # Assertions
    [ "$cleaned_output" = "Hello, World!" ]  # Ensure correct output for success
}

@test "Built-in exit command works" {
    run ./dsh <<EOF
exit
EOF
    echo "Debug: Actual output: [$output]"


}

@test "Handles long of an argument" {
    run ./dsh <<EOF
echo $(printf 'arg%.0s' {1..300})
EOF

    # Assertions
    [[ "$output" == *"Command not found in PATH"* ]]
}

