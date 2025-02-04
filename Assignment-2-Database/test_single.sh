#!/usr/bin/env bats

setup_file() {
    # Delete the student.db file if it exists
    if [ -f "student.db" ]; then
        rm "student.db"
    fi
}

@test "Add a student 1 to db" {
    run ./sdbsc -a 1 john doe 345
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 1 added to database." ]
}

