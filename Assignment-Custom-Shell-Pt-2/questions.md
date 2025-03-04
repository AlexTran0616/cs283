1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  _The reason we use fork() before execvp() is because it allows us to keep running after a command is executed. We are able to create a child and then continue the command from there; however, the parent stays in tact; meaning we can just continue from where we left off._

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  _If the fork fails, it returns a value of -1. In my code; the way I dealt with this is just by testing if pid isn’t >= 0 and if it isn’t, we know there’s a fork error as it must’ve returned a -1. Than, an error is printed.
_

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  _execvp() finds the command to execute by looking for a “/” or searches the directories that are in the PATH environment variable.
_

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  _We use wait because we want to be able to retrieve its exit status. This will eventually be important during the next week's implementation because we want to implement pipes. This will be useful for stdin and stdout. If we didn’t call it we could leave the child there without freeing it as we don’t know the exit status. This means we’d likely leave resources hanging.
_

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  _The WEXITSTAUTUS gets the exit status of a child who closes. This is good because again, we can see if the child ran successfully or not. Again, this can be important because we can use it later for pipes. We can give an error if the exit status wasn’t 0._

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  _My implementation of build_cmd_buff() handles quoted arguments by having an in_quote flag. This basically just tests if we’re inside a quote, and if we meet another quote we know that we can end the quote here. We needed this because otherwise we wouldn’t be able to keep the quote in tact in the echo command. An example of this is my original implementation using strtok not working because strtok separated them by “ “. _

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  _To be honest, in terms of actually changing my parsing, it’s not very different from the previous assignment. Most things including even the quote flag I already had previously; so, I didn’t really need too much adjusting._

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _The purpose of signals in the linux system is basically to notify that something’s happening. It’s like in class when we gave the example of the call while shopping; it’s basically a notification to tell you if something’s happening and we can decide what to do based on the type of signal. They differ from other forms of interprocess communications because they can interrupt any process at any time as long as it’s activated. It’s doesn’t need to apply to the linear format of execution we’re familiar with._

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _3 signals are sigkill, sigterm, and sigint. These are all able to stop processes and are used like that; however, they do it differently. Kill forcefully terminates a process, term requests a termination, and sigint stop a process through the user._

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _SIGSTOP normally stops the process and it will no longer execute anything until given the go ahead. This program wont continue until SIGCONT. I’m pretty sure SIGSTOP can’t just be caught or ignored because this is meant to halt the process. This is meant to pause it temporarily and ensure the process can’t be resumed unless directly specified using the cont command._
