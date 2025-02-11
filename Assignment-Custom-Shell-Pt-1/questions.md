1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  _I believe that fgets() is good for this assignment because it reads up till the new like '\n'. This means it'll read just 1 line at a time, which is exactly what we wanted. I guess it's also good because we know "A terminating null byte ('\0') is stored after the last character in the buffer"; this means that implementation is pretty easy as we know the end of the line will be a null terminator._

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  _There are two reasons. First, I believe it helps with scalability in the case that we have extremely long commands. A fixed array would need to be adjusted each time we get a larger command; however, this gives us more flexibility. Second, I believe it gives us better control over our resources as we decide when to free the buffer (when it's no longer needed)._


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  _Trimming the leading and trailing spaces is necessary because of the chance of misinterpretation. Commands could be read wrong and produce the wrong output if we don't handle this. A quick example could be a command being read as "cmd" vs. "cmd ". This would cause an issue as this blank space would be considered a proper command._

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  _some redirection examples are ">>, 2>&1, >, |, <, etc". Some challenges we might have when implementing them is correctly redirecting input. Another challenge we may have is writing information so some information isn't overridden. This could occur due to two files redirecting to the same file at the same time. EX: what if we wanted to put "file1" and "file2" in "file3" at the same time? could we do it or would we just need to input them seperately?_

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  _The difference is that redirection takes an input and or output and kind of decides where it should be. Piping on the other hand takes the output of one command and slaps it into the input of another. An example could be redirecting input from file1 to file2; while with piping we can think of getting an output 4 and inserting it into the new input x+2._

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  _It's important to keep them seperate mainly for clarity. This allows us to have clear view of when errors occur and when normal output occurs. This can also be good for debugging as it shows you the difference between normal output vs. error handling._

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  _We should handle our errors from commands that fail by retrieving both the STDOUT and STDERR; then if an error occurs we can have an exit status. If there is no exit status, we know no error occured. I guess it could also be usful to merge STDERR and STDOUT because it could make it easier to see both the errors occur and regular output together. A way we can do this is probably by redirection. We can take the input and direct it to the same location._
