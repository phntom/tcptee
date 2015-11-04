This tool merges multiple (two or more) incoming TCP connections to help reverse forwarding listening connections.
For example, you can forward an SSH server using netcat to tcptee, and connect putty to the same port, once both are connected putty will think it's talking to the real server.
This should be very useful to reverse binding connections though restrictive firewalls.

**Usage:**
```
tcptee [bindport]
tcptee [bindhost] [bindport]
tcptee [bindhost] [bindport] [max connections]
```

**Example:**
SSH server allows only outbound connections. We start tcptee on 192.168.0.1:1337.
We connect our client to the merge point, and using netcat tell the server to export his outbound connection to the merge point and to bridge to the local SSH port.
Once both the server and the client connect to the merge point they talk to each other.
```
tcptee 192.168.0.1 1337 # create a new merge point and bind to 192.168.0.1:1337
ssh 192.168.0.1 1337    # connect client to 192.168.0.1:1337
# before the second client connects data is being buffered
ncat 192.168.0.1 1337 -c "ncat 127.0.0.1 22" # bridge localhost:22 to 192.168.0.1:1337
```

**Compile on Linux:**
```
sudo apt-get install subversion
svn checkout http://tcptee.googlecode.com/svn/trunk/ tcptee
sudo apt-get install libqt4-dev build-essential
cd tcptee
qmake -config release
make
./tcptee
```