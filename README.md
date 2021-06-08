<p align="center">
  <img src="media/icon.png" alt="Girl in a jacket" width="225">
</p>

# ircgod

ircgod is a plug and play irc server built from scratch in cpp which aim to provide all basic fonctionnality of an irc server.  
It supports multi server, service and most clients

<p align="center">
  <img src="media/preview.png" alt="Girl in a jacket" width="675">
</p>

# Install 
linux  
`sudo apt install -y libssl-dev && make`

42  
`brew install openssl && export CPPFLAGS=-I$HOME/.brew/opt/openssl/include && make 42`

# Run

Generate SSL certs for tls communication (optional):  
`make certs`  
and then run:

```
./ircserv <config_filename>
```
Preconfigured config file can already be found under the conf directory but we advise you to fork sample.conf and modify it to your convenience.
