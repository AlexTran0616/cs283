1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_I believe that it's important that all child processes complete before the shell continues accepting user input because we want to get the return status of the children before the parent continues executing. If we don't, some children may be finished executing, but are still waiting to report thier status to the parent. This can lead to a problem when it comes to additional resources being wasted._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_dup2 allows us to correctly redirect input from one pipe to another. The reason why we need to close this is because we don't want to leave pipes open as it can make us leak resources. This becomes an issue the more pipes that are left open._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_We implemented cd as a build in command instead of an external command because it changes the cwd of the directory of the shell. This allwos them to modify the shell's internal state; however, if we left this as an external process, we would create a new child to change the directory. This would only effect the child's process and not the parent's. This would make the cd command basically useless._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_We could use a dynamically sized array to store the commands. We can just resize the array whenever we need (like doubling the size whenever we reach the limit). This would allow us to directly have control over memory and free it whenever it's no longer used. The downside to this could be performance because of constant resizing. This could also lead to a lot of complexity in the code because of needing to deal with potential memory leaks._
