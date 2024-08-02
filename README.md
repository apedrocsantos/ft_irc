# IRC - Internet Relay Chat

The IRC protocol is a text-based protocol, with the simplest client being any socket program capable of connecting to the server.

## References

https://irssi.org/ (irc client)

https://mbrunel.github.io/ft_irc/index.html

RFCs

- [RFC1459: "Internet Relay Chat Protocol"](https://www.rfc-editor.org/rfc/rfc1459) - Updated by RFC2812
- [RFC2810: "IRC: Architecture"](https://www.rfc-editor.org/rfc/rfc2810)
- [RFC2811: "IRC: Channel Management"](https://www.rfc-editor.org/rfc/rfc2811)
- [RFC2812: "IRC: Client Protocol"](https://www.rfc-editor.org/rfc/rfc2812)
- [RFC2813: "IRC: Server Protocol"](https://www.rfc-editor.org/rfc/rfc2813)

[tutorial](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)

[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)

[Beej's Guide to Network Concepts](https://beej.us/guide/bgnet0/html/#project-multiuser-chat-client-and-server)

[ref project](https://github.com/AhmedFatir/ft_irc)

[IRC Cheatsheet 1](https://gist.github.com/xero/2d6e4b061b4ecbeb9f99)  
[IRC Cheatsheet 2](https://www.cheat-sheets.org/saved-copy/wikiHow_IRC_Cheat_Sheet.pdf)

[The Linux socket API explained](https://www.youtube.com/watch?v=XXfdzwEsxFk)

## RFC1459

A client is distinguished from other clients by a unique nickname having a **maximum length of nine (9) characters**.

In addition to the **nickname**, all servers must have the following information about all clients: the **real name of the host** that the client is running on, the **username** of the client on that host, and the **server** to which the client is connected.

A **channel** is a named group of one or more clients which will all receive messages addressed to that channel.

**Channels** names are strings (beginning with a ’**&**’ or ’**#**’ character) of length **up to 200 characters**. Apart from the the requirement that the first character being either ’&’ or ’#’; the only restriction on a channel name is that it **may not contain any spaces (’ ’), a control G (^G or ASCII 7), or a comma** (’,’ which is used as a list item separator by the protocol).

To create a new channel or become part of an existing channel, a user is required to **JOIN** the channel. If the channel doesn’t exist prior to joining, the **channel is created and the creating user becomes a channel operator**. If the channel already exists, whether or not your request to JOIN that channel is honored depends on the **current modes of the channel**.

As part of the protocol, a user may be a part of several channels at once.

### Channel Operators

The commands which may only be used by channel operators are:

KICK - Eject a client from the channel  
MODE - Change the channel’s mode  
INVITE - Invite a client to an invite-only channel (mode +i)  
TOPIC - Change the channel topic in a mode +t channel

A channel operator is identified by the ’**@**’ symbol next to their nickname whenever it is associated with a channel (ie replies to the NAMES, WHO and WHOIS commands).

### Messages

Servers and clients send eachother messages which may or may not generate a reply. If the message contains a valid command, the client should expect a reply as specified but it is not advised to wait forever for the reply; client to server and server to server communication is essentially asynchronous in nature.

Each IRC message may consist of up to three main parts: the prefix (optional), the **command**, and the **command parameters** (of which there may be **up to 15**).  The prefix, command, and all parameters are **separated by one (or more) ASCII space character(s) (0x20).**

Clients should not use prefix when sending a message from themselves.

The command must either be a valid IRC command or a three (3) digit number represented in ASCII text.

IRC messages are always lines of characters terminated with a **CR-LF** (Carriage Return - Line Feed) pair, and **these messages shall not exceed 512 characters in length, counting all characters including the trailing CR-LF. Thus, there are 510 characters maximum allowed for the command and its parameters**.  There is no provision for continuation message lines.

Empty  messages  are  silently  ignored.

The extracted message is parsed into the components \<prefix>, \<command> and list of parameters matched either by \<middle> or \<trailing> components.

**(See Section 6, Replies)**

## Message details

### Connection Registration

A "PASS" command is not required for either client or server connection to be registered, but it must precede the server message or the latter of the NICK/USER combination. The recommended order for a client to register is as follows:

1.	Pass message
2.	Nick message
3.	User message

#### Password message

Command: PASS
Parameters: \<password>

The PASS command is used to set a ’connection password’.  The password can and must be set before any attempt to register the connection is made.

It is possible to send multiple PASS commands before registering but only the last one sent is used for verification and it may not be changed once registered.

Numeric Replies:
-	ERR_NEEDMOREPARAMS
-	ERR_ALREADYREGISTRED

#### Nick message

Command: NICK
Parameters: \<nickname>

NICK message is used to give user a nickname or change the previous one.

If the server recieves an identical NICK from a client which isdirectly connected, it may issue an ERR_NICKCOLLISION to the local client, drop the NICK command, and not generate any kills.

Numeric Replies:
-	ERR_NONICKNAMEGIVEN
-	ERR_NICKNAMEINUSE
-	ERR_ERRONEUSNICKNAME
-	ERR_NICKCOLLISION

#### User message

Command: USER
Parameters: \<username> \<hostname> \<servername> \<realname>

The USER message is used at the beginning of connection to specify the username, hostname, servername and realname of a new user.

Note that hostname and servername are normally ignored by the IRC server when the USER command comes from a directly connected client (for security reasons).

It must be noted that realname parameter must be the last parameter, because it may contain space characters and must be prefixed with a colon (’:’) to make sure this is recognised as such.

Numeric Replies:

-	ERR_NEEDMOREPARAMS
-	ERR_ALREADYREGISTERED

#### Oper

Command: OPER
Parameters: \<user> \<password>

OPER message is used by a normal user to obtain operator privileges. The combination of \<user> and \<password> are required to gain Operator privileges.

If the client sending the OPER command supplies the correct password for the given user, the server then informs the rest of the network of the new operator by issuing a "MODE +o" for the clients nickname.

Numeric Replies:

-	ERR_NEEDMOREPARAMS
-	ERR_NOOPERHOST
-	RPLY_YOUREOPER
-	ERR_PASSWDMISMATCH

#### Quit

Command: Quit
Parameters: [\<quit message>]

A client session is ended with a quit message.  The server must close the connection to a client which sends a QUIT message. If a "Quit Message" is given, this will be sent instead of the default message, the nickname.

If, for some other reason, a client connection is closed without  the client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs on socket), the server is required to fill in the quit  message  with some sort  of  message  reflecting the nature of the event which caused it to happen.

Numeric Replies:

-	None

### Channel operations

This group of messages is concerned with manipulating channels, their properties (channel modes), and their contents (typically clients). In implementing these, a number of race conditions are inevitable when clients at opposing ends of a network send commands which will ultimately clash.  It is also required that servers keep a nickname history to ensure that wherever a \<nick> parameter is given, the server check its history in case it has recently been changed.

#### Join message

Command: JOIN
Parameters: \<channel>{,\<channel>} [\<key>{,\<key>}]

The JOIN command is used by client to start listening a specific channel. Whether or not a client is allowed to join a channel is checked only by the server the client is connected to;

1.	the user must be invited if the channel is invite-only;
2.	the user’s nick/username/hostname must not match any active bans;
3.	the correct key (password) must be given if it is set.

Once a user has joined a channel, they receive notice about all commands their server receives which affect the channel.  This includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.

If a JOIN is successful, the user is then sent the channel’s topic (using **RPL_TOPIC**) and the list of users who are on the channel (using **RPL_NAMREPLY**), which must include the user joining.

Numeric Replies:

ERR_NEEDMOREPARAMS
ERR_INVITEONLYCHAN
ERR_CHANNELISFULL
ERR_NOSUCHCHANNEL
RPL_TOPIC
ERR_BANNEDFROMCHAN
ERR_BADCHANNELKEY
ERR_BADCHANMASK
ERR_TOOMANYCHANNELS

Examples:

JOIN #foo,#bar fubar - join channel #foo using key "fubar" and #bar using no key.

#### Mode message

The MODE command is a dual-purpose command in IRC.  It allows both usernames and channels to have their mode changed.

#### Channel modes

The MODE command is provided so that users may query and change the characteristics of a channel. Note that there is a maximum limit of three (3) changes per command for modes that take a parameter.

Parameters: \<channel> {[+|-]|i|t|k|o|l} [\<limit>] [\<user>] [\<ban mask>]

-	i - invite-only channel flag;
-	t - topic settable by channel operator only flag;
-	k - set a channel key (password).
-	o - give/take channel operator privileges;
-	l - set the user limit to channel;

When using the ’o’ and ’b’ options, a restriction on a total of three per mode command has been imposed.

Numeric Replies:

ERR_NEEDMOREPARAMS
ERR_NOCHANMODES
ERR_USERNOTINCHANNEL
RPL_CHANNELMODEIS
RPL_BANLIST
RPL_EXCEPTLIST
RPL_INVITELIST
RPL_UNIQOPIS
ERR_KEYSET
ERR_CHANOPRIVSNEEDED
ERR_UNKNOWNMODE
RPL_ENDOFBANLIST
RPL_ENDOFEXCEPTLIST
RPL_ENDOFINVITELIST

#### User modes

Parameters: \<nickname> {[+|-]|i|w|s|o}

he user MODEs are typically changes which affect either how the client is seen by others or what ’extra’ messages the client is sent. A user MODE command may only be accepted if both the sender of the message and the nickname given as a parameter are both the same.

If a user attempts to make themselves an operator using the "+o" flag, the attempt should be ignored.  There is no restriction, however, on anyone ‘deopping’ themselves (using "-o").

Numeric Replies:

ERR_NEEDMOREPARAMS
ERR_CHANOPRIVSNEEDED
ERR_NOTONCHANNEL
RPL_BANLIST
ERR_UNKNOWNMODE
ERR_USERSDONTMATCH
ERR_UMODEUNKNOWNFLAG
RPL_CHANNELMODEIS
ERR_NOSUCHNICK
ERR_KEYSET
RPL_ENDOFBANLIST
ERR_NOSUCHCHANNEL
RPL_UMODEIS

Examples:

`MODE #Finnish +im` - Makes #Finnish channel moderated and ’invite-only’.

`MODE #Finnish +o Kilroy` - Gives ’chanop’ privileges to Kilroy on channel #Finnish.

`MODE #42 +k oulu` - Sets the channel key to "oulu".

`MODE WiZ -o` - WiZ ’deopping’ (removing operator status).  The plain reverse of this command ("MODE WiZ +o") must not be allowed from users since would bypass the OPER command.

#### Topic message

Command: TOPIC
Parameters: \<channel> [\<topic>]

The TOPIC message is used to change or view the topic of a channel. The topic for channel \<channel> is returned if there is no \<topic> given.  If the \<topic> parameter is present, the topic for that channel will be changed, if the channel modes permit this action.

Numeric Replies:

ERR_NEEDMOREPARAMS
RPL_NOTOPIC
ERR_CHANOPRIVSNEEDED
ERR_NOTONCHANNEL
RPL_TOPIC

#### Invite message

Command: INVITE
Parameters: \<nickname> \<channel>

The INVITE message is used to invite users to a channel.  The parameter \<nickname> is the nickname of the person to be invited to the target channel \<channel>.  There is no requirement that the channel the target user is being invited to must exist or be a valid channel.  To invite a user to a channel which is invite only (MODE +i), the client sending the invite must be recognised as being a channel operator on the given channel.

Numeric Replies:

ERR_NEEDMOREPARAMS
ERR_NOTONCHANNEL
ERR_CHANOPRIVSNEEDED
RPL_INVITING
ERR_NOSUCHNICK
ERR_USERONCHANNEL
RPL_AWAY

#### Kick command

Command: KICK
Parameters: \<channel> \<user> [\<comment>]

The KICK command can be  used  to  forcibly  remove  a  user  from  a channel.   It  ’kicks  them  out’  of the channel (forced PART).

Only a channel operator may kick another user out of a  channel.

Numeric Replies:

ERR_NEEDMOREPARAMS
ERR_BADCHANMASK
ERR_NOTONCHANNEL
ERR_NOSUCHCHANNEL
ERR_CHANOPRIVSNEEDED

NOTE:
     It is possible to extend the KICK command parameters to the following:
		`\<channel>{,\<channel>} \<user>{,\<user>} [\<comment>]`

### Sending messages


The main purpose of the IRC protocol is to provide a base for clients to communicate with each other. **PRIVMSG** and **NOTICE** are the only messages available which actually perform delivery of a text message from one client to another - the rest just make it possible and try to ensure it happens in a reliable and structured manner.

#### Private messages

Command: PRIVMSG
Parameters: \<receiver>{,\<receiver>} \<text to be sent>

PRIVMSG is used to send private messages between users.  \<receiver> is the nickname of the receiver of the message.  \<receiver> can also be a list of names or channels separated with commas.

Numeric Replies:

ERR_NORECIPIENT
ERR_CANNOTSENDTOCHAN
ERR_WILDTOPLEVEL
ERR_NOSUCHNICK
RPL_AWAY
ERR_NOTEXTTOSEND
ERR_NOTOPLEVEL
ERR_TOOMANYTARGETS

#### Notice

Command: NOTICE
Parameters: \<nickname> \<text>

The NOTICE message is used similarly to PRIVMSG.  The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message. This rule applies to servers too - they must not send any error reply back to the client on receipt of a notice.

### Command Parsing

To provide useful ’non-buffered’ network IO for clients and servers, each connection is given its own private ’input buffer’ in which the results of the most recent read and parsing are kept.  A buffer size of **512** bytes is used so as to hold 1 full message, although, this will usually hold several commands.  The private buffer is parsed after every read operation for valid messages.  When dealing with multiple messages from one client in the buffer, care should be taken in case one happens to cause the client to be ’removed’.

### Tracking nickname changes

All IRC servers are required to keep a history of recent nickname changes.  This is required to allow the server to have a chance of keeping in touch of things when nick-change race conditions occur with commands which manipulate them.  Commands which must trace nick changes are:
-	KILL (the nick being killed)
-	MODE (+/- o,v)
-	KICK (the nick being kicked)

No other commands are to have nick changes checked for.

### Operators

Storage of oper passwords in configuration files is preferable to hard coding them in and should be stored in a crypted format (ie using **crypt**(3) from Unix) to prevent easy theft.

## RFC2811

### Namespace

Channels names are strings (beginning with a ’**&**’, ’**#**’, ’**+**’ or ’**!**’character) of **length up to fifty (50) characters**.  Channel names are **case insensitive**.

The use of different prefixes effectively creates four (4) distinct namespaces for channel names.  This is important because of the protocol limitations regarding namespaces (in general).

### Channel Scope

Channels with ’&’ as prefix are local to the server where they are created.

### Channel Properties

Channels with ’+’ as prefix do not support channel modes.

### Channel Operators

Since channels starting with the character ’+’ as prefix do not support channel modes, no member can therefore have the status of channel operator.

### Channel lifetime

In regard to the lifetime of a channel, there are typically two groups of channels: standard channels which prefix is either ’&’, ’#’ or ’+’, and "safe channels" which prefix is ’!’.

#### Standard channels

These channels are created implicitly when the first user joins it, and cease to exist when the last user leaves it.  While the channel exists, any client can reference the channel using the name of the channel.

#### Safe channels

Unlike other channels, "safe channels" are not implicitly created.  A user wishing to create such a channel MUST request the creation by sending a special JOIN command to the server in which the channel identifier (then unknown) is replaced by the character ’!’.  The creation process for this type of channel is strictly controlled. The user only chooses part of the channel name (known as the channel "short name"), the server automatically prepends the user provided name with a channel identifier consisting of five (5) characters. The channel name resulting from the combination of these two elements is unique, making the channel safe from abuses based on network splits.

In this case, channel names do not become unavailable: these channels may continue to exist after the last user left. Only the user creating the channel becomes "channel creator", users joining anexisting empty channel do not automatically become "channel creator" nor "channel operator".

### Channel Flags

#### User limit

A user limit may be set on channels by using the channel flag ’l’. When the limit is reached, servers MUST forbid their local users to join the channel.

The value of the limit MUST only be made available to the channel members in the reply sent by the server to a MODE query.

#### Channel Key

When a channel key is set (by using the mode ’k’), servers MUST reject their local users request to join the channel unless this key is given.

The channel key MUST only be made visible to the channel members in the reply sent by the server to a MODE query.

### RFC2813

#### Connection ´Liveness´

To detect when a connection has died or become unresponsive, the server MUST **poll** each of its connections.

If a connection doesn’t respond in time, its connection is closed using the appropriate procedures.

#### Users

When a server successfully registers a new user connection, it is REQUIRED to send to the user unambiguous messages stating: the user identifiers upon which it was registered (**RPL_WELCOME**), the server name and version (**RPL_YOURHOST**), the server birth information (**RPL_CREATED**), available user and channel modes (**RPL_MYINFO**), and it MAY send any introductory messages which may be deemed appropriate.

In particular the server SHALL send the current user/service/server count (as per the LUSER reply) and finally the MOTD (if any, as per the MOTD reply).


## IRC client -> [WeeChat](https://weechat.org/files/doc/weechat/stable/weechat_quickstart.en.html)

### Basic commands

`/server add [server name] [server ip/port]` - add server to list

#### Settings

`/set irc.server.[server name].[option] [value]` - set option  
`/set irc.server.[server name].addresses [address]` - set/change server address  
`/set irc.server.[server name].nicks "[nick]"` - set/change nick  
`/set irc.server.[server name].realname "[realname]"` - set/change real name  
`/set irc.server.[server name].username "[username]"` - set/change user name  
`/set irc.server.[server name].command "/msg [command and settings]"` - set generic command  
`/set irc.server.[server name].command "/msg nickserv identify xxxxxxx"` - identify nick  
`/unset irc.server.[server name].[option]` - remove a value of a server option, and use the default value instead

The **nickname** supplied with NICK is the name that's used to address you on IRC. The nickname must be unique across the network, so you can't use a nickname that's already in use at the time.

The **username** supplied with USER is simply the user part in your user@host hostmask that appears to others on IRC, showing where your connection originates from.

The **realname** supplied with USER is used to populate the real name field that appears when someone uses the WHOIS command on your nick.

#### Connect

`/connect [server name]` - connect to server  
`/connect [server name] -password=[password]` - connect to server using pasword

#### Client commands

`/nick [nickname]` - change nickname
`/join #channel` - join channel  
`/part [quit message]` - leave channel, keeping the buffer open  
`/close` - close a server, channel or private buffer  
`/disconnect` - disconnect from server, on the server buffer

`/query foo this is a message` - open a buffer and send a message to another user (nick foo)

`/kick #[channel] [nickname] ([reason])` - temporarily remove user from channel  
`/invite <nickname> <channel>` - invite to channel  
`/topic <channel> [<topic>]` - The topic for channel &lt;channel&gt; is returned if there is no &lt;topic&gt; given. If the &lt;topic&gt; parameter is present, the topic for that channel will be changed, if the channel modes permit this action.  
`/mode <channel> {[+|-]|i|t|k|o|l} [<user>]` - The user MODEs are typically changes which affect either how the client is seen by others or what 'extra' messages the client is sent.

## Connecting WeeChat to localhost (using nc)

PASS \[password\]
CAP LS 302
NICK \[irc.server.\[server name\].nicks\]
USER \[irc.server.\[server name\].nicks\] 0 \* :\[irc.server.\[server name\].realname\]

## Structs

### addrinfo

Loaded before calling `getaddrinfo()`, that fills out the structure as needed.

``` c++
struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node
};
```

The `struct sockaddr` holds socket address information for many types of sockets.

```c++
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
}; 
```

To deal with `struct sockaddr`, programmers created a parallel structure: `struct sockaddr_in` to be used with IPv4. A pointer to a struct `sockaddr_in` can be cast to a pointer to a `struct sockaddr` and vice-versa. So even though `connect()` wants a `struct sockaddr*`, you can still use a `struct sockaddr_in` and cast it.

``` c++
// (IPv4 only--see struct sockaddr_in6 for IPv6)

struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
```

This structure makes it easy to reference elements of the socket address. Note that sin_zero should be set to all zeros with the function `memset()`.

```c++
// (IPv4 only--see struct in6_addr for IPv6)

// Internet address (a structure for historical reasons)
struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};
````

`ina.sin_addr.s_addr` references the 4-byte IP address (in Network Byte Order).



## Functions

### Socket

**socket** - create an endpoint for communication and returns a descriptor.

- #include &lt;sys/socket.h&gt;
- `int socket(int domain, int type, int protocol);`
    - domain: selects the protocol family which should be used. These families are defined in the include file. Use AF_INET (Internet version 4 protocols).
    - type: specifies the semantics of communication. Currently defined types are:
        - SOCK_STREAM - provides sequenced, reliable, two-way connection based bytes streams (TCP).
        - SOCK_DGRAM - supports datagrams (connectionless, unreliable messages of a fixed maximum length) (UDP).
        - SOCK_RAW - provide access to internal network protocols and interfaces. Available only to the super-user.
    - protocol: specifies a particular protocol to be used with the socket. Normally, only a single protocol exists to support a particular socket type within a given protocol family.

Sockets of type **SOCK_STREAM** are full-duplex byte streams, similar to pipes. A stream socket must be in a connected state before any data may be sent or received on it. A connection to another socket is created with a **connect**(2) or connectx(2) call. Once connected, data may be transferred using **read**(2) and **write**(2) calls or some variant of the send(2) and recv(2) calls. When a session has been completed a **close**(2) may be performed.  
The communications protocols used to implement a **SOCK_STREAM** insure that data is not lost or duplicated. If a piece of data for which the peer protocol has buffer space cannot be successfully transmitted within a reasonable length of time, then the connection is considered broken and calls will indicate an error with **\-1** returns and with **ETIMEDOUT** as the specific code in the global variable errno. The protocols optionally keep sockets “warm” by forcing transmissions roughly every minute in the absence of other activity. An error is then indicated if no response can be elicited on an otherwise idle connection for a extended period (e.g. 5 minutes). A **SIGPIPE** signal is raised if a process sends on a broken stream; this causes naive processes, which do not handle the signal, to exit.

The operation of sockets is controlled by socket level options. These options are defined in the file ⟨sys/socket.h⟩. **Setsockopt**(2) and **getsockopt**(2) are used to set and get options, respectively.