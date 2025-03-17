1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_The remove client determines when a command's output is fully received from the server when ti recieves the EOF marker. One way to handle partial reads is to use a marker, so we know the size of the command. So, we would know when the entire command has been parsed through. That's one way we can do it._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_A networked shell protocol can be define the beginning and end of commands by using start and end markers (like how we use EOF). The chalenges of this when it's not handled correctly is that data may be lost somewhere or data could just end up corrupted overall._

3. Describe the general differences between stateful and stateless protocols.

_stateful protocols maintain a state across muiltiple client-servers and any previous inputs can influence how things work later on. However, stateless protocol treats every request made as if independent with no retained context; it's just itself._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_We use UDP when we're okay with some error or when iot's critical that we want low latency/quick responses. So, UDP can be much faster._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_The interface/abstraction that is provided by the operating system to enable applications to use network comms. is socket. This allows us to use functions like connect() and bind(), allowing us to send() and recv() data from a server and client._
