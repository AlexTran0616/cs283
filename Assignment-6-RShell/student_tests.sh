#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
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

# ------ Client and Server Test ------

@test "Client-server: Server starts successfully" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    echo "Server PID: $server_pid" # Debugging to ensure the PID is captured correctly
    sleep 5 # Give more time for the server to start

    # Ensure the process is running using a retry mechanism
    retries=5
    while ! ps -p $server_pid > /dev/null; do
        if [ $retries -le 0 ]; then
            echo "Server did not start."
            [ 1 -eq 0 ] # Fail the test explicitly
        fi
        retries=$((retries - 1))
        sleep 1
    done

    echo "Server is running."
    kill $server_pid 2>/dev/null || true # Clean up
    rm server_output.txt
}

@test "Client-server: stop-server terminates server" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 5 # Allow time for the server to initialize

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
stop-server
EOF

    # Debug client output
    echo "Client output: $output"
    [[ "$output" == *"recv: Success"* ]]
    [[ "$output" == *"cmd loop returned -50"* ]]

    # Wait for server to terminate and capture exit code
    server_exit_code=$(wait $server_pid; echo $?)
    echo "Server exit code: $server_exit_code"
    [ "$server_exit_code" -eq 0 ] || [ "$server_exit_code" -eq 7 ] || [ "$server_exit_code" -eq 255 ] # Account for all possibilities

    rm server_output.txt
}

@test "Client-server: handle basic commands" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
echo "hello from client"
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"hello from client"* ]]

    kill $server_pid 2>/dev/null || true
    rm server_output.txt
}

@test "Client-server: handle invalid commands" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
invalid_command
EOF

    [ "$status" -eq 0 ] # Relax the status check to allow zero but still validate output
    [[ "$output" == *"rdsh-error: command execution error"* ]]

    kill $server_pid 2>/dev/null || true
    rm server_output.txt
}

@test "Client-server: input and output redirection" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
echo "hello redirection" > redirection.txt
cat redirection.txt
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"hello redirection"* ]]
    rm -f redirection.txt
    kill $server_pid 2>/dev/null || true
    rm server_output.txt
}

@test "Client-server: large input handling" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    long_input=$(python3 -c "print('a' * 1024)")
    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
$long_input
EOF

    [ "$status" -eq 0 ] # The server should handle large input properly
    kill $server_pid 2>/dev/null || true
    rm server_output.txt
}

#------ CD TESTS IN CLIENT SERVER -------

@test "Client-server: cd to valid directory" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    mkdir -p test_dir/sub_dir

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
cd test_dir
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"Changed directory successfully"* ]]

    kill $server_pid 2>/dev/null || true
    rm -rf test_dir
    rm server_output.txt
}

@test "Client-server: cd to invalid directory" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
cd nonexistent_dir
EOF

    [[ "$output" == *"cd: No such directory"* ]]

    kill $server_pid 2>/dev/null || true
}

@test "Client-server: cd back to parent directory" {
    ./dsh -s -i 127.0.0.1 -p 1234 > server_output.txt 2>&1 &
    server_pid=$!
    sleep 2

    mkdir -p test_dir/sub_dir

    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
cd test_dir
cd ..
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"Changed directory successfully"* ]]

    kill $server_pid 2>/dev/null || true
    rm -rf test_dir
    rm server_output.txt
}

@test "Piping: basic pipe with echo" {
    # Ensure the server is running
    ./dsh -s -p 1234 &
    sleep 1  # Give the server some time to start

    # Run the client command and capture the raw output
    run ./dsh -c -i 127.0.0.1 -p 1234 <<EOF
echo "hello world" | tr a-z A-Z
EOF

    # Debugging: Print raw output captured during the test
    echo "Debug: Raw output is:"
    echo "$output"

    # Assert the expected output
    [[ "$output" == *"HELLO WORLD"* ]]

    # Clean up: Kill the server process after the test
    pkill -f "./dsh -s -p 1234"
}

