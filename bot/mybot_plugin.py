# -*- coding: utf-8 -*-
from irc3.plugins.command import command
import irc3


@irc3.plugin
class Plugin:

    def __init__(self, bot):
        self.bot = bot

    @irc3.event(irc3.rfc.PRIVMSG)
    def myevent(self, mask=None, event=None, target=None, data=None, tags=None):
        """Quoi ? feur"""
        if mask.nick != self.bot.nick:
            if "quoi" in data or "qwa" in data:
                self.bot.privmsg(target, 'feur')
    
    @command(permission='view')
    def echo(self, mask, target, args):
        """Echo

            %%echo <message>...
        """
        yield ' '.join(args['<message>'])
