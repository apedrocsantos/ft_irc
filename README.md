# ft_irc

# IRC - Internet Relay Chat

The IRC protocol is a text-based protocol, with the simplest client being any socket program capable of connecting to the server.

## References

https://irssi.org/ (irc client)

https://mbrunel.github.io/ft_irc/index.html

RFCs

- RFC1459: "Internet Relay Chat Protocol" - Updated by RFC2812
- RFC2810: "IRC: Architecture"
- RFC2811: "IRC: Channel Management"
- RFC2812: "IRC: Client Protocol"
- RFC2813: "IRC: Server Protocol"

[tutorial](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)

[project](https://github.com/AhmedFatir/ft_irc)

[IRC Cheatsheet 1](https://gist.github.com/xero/2d6e4b061b4ecbeb9f99)  
[IRC Cheatsheet 2](https://www.cheat-sheets.org/saved-copy/wikiHow_IRC_Cheat_Sheet.pdf)

## RFC1459

A client is distinguished from other clients by a unique nickname having a maximum length of nine (9) characters.

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
-	ERR_ALREADTREGISTERED

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
-	ERR_PASSWDMISMATH

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

Parameters: \<channel> {[+|-]|i|t|k|o|l} [\<limit>] [\<user>] [\<ban mask>]

-	i - invite-only channel flag;
-	t - topic settable by channel operator only flag;
-	k - set a channel key (password).
-	o - give/take channel operator privileges;
-	l - set the user limit to channel;

When using the ’o’ and ’b’ options, a restriction on a total of three per mode command has been imposed.

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

## Sockets

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