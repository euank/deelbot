# deelbot

Deelbot is a proof-of-concept modular irc bot in C. Via the fun of `dlopen` and `dlsym`, the modules for this bot may be reloaded on the fly.

Currently, this can be done via `/CTCP $botNick RELOAD`.

This code was written as a proof of concept and for my own benefit. It is not actively maintained.

## Dependancies

[libircclient](http://www.ulduzsoft.com/libircclient/)
