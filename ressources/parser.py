def parse(data, port, origin):
    if origin == 'server':
        return
    if port == 5555: #c'est impossible en l'occurence mais c'est pour l'exemple
        return
    else:
        print "[" + origin + ":" + str(port) + "]" + data

#il y a un lf ( + un cr) en fin de data ducoup c'est pour ca que ca print sur 2 lignes, faites un bail du genre pour print sur 1:
#print ("[" + origin + ":" + str(port) + "]" + data, end='') je c meme pas si ca marche mais c'est un bail du genre pour sur