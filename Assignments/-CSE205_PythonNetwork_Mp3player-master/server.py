import socket
import threading
import struct
import pickle
import os
import hashlib
import sys


class Command:
    command = ""
    payload = ""


def readFile(dir):
    try:
        list = []
        if os.path.exists(dir):
            for line in open(dir):
                list.append(line.strip())
            return list
    except BaseException:
        print('wrong in read file')


def savefile(dir, list):
    try:
        file = open(dir, 'w')
        for i in len(list):
            file.write(list[i] + '\n')
        return True
    except BaseException:
        print('file does not exist')


def savedicfile(dir, list):
    with open(dir, 'wb') as f:
        pickle.dump(list, f)


def readdicfile(dir):
    with open(dir, 'rb') as f:
        try:
            return pickle.load(f)
        except EOFError:
            return None


def md5ID(fileOrstring):
    if os.path.isfile(fileOrstring):
        f = open(fileOrstring, 'rb')
        md5_file = hashlib.md5()
        md5_file.update(f.read())
        hash_code = md5_file.hexdigest()
        f.close()
        md5 = str(hash_code).lower()
    else:
        my_md5 = hashlib.md5()
        my_md5.update(fileOrstring.encode("utf8"))
        hash_code = my_md5.hexdigest()
        md5 = str(hash_code).lower()
    return md5


def getpathlist(path, extension):
    list = []
    if os.path.isdir(path):
        for item in os.listdir(path):
            if item.split('.')[1] == extension:
                list.append(item)
        return list


class SocketThread(threading.Thread):
    # you need to override the constructor, but make sure to call the base
    # constructor
    def __init__(self, socketInstance):
        # make sure you do this or it won't work...
        threading.Thread.__init__(self)
        global songDict
        self.mySocket = socketInstance

    # this is what gets run when you call start()
    def run(self):
        global songDict
        try:
            while (True):
                print("Reading initial length")
                a = self.mySocket.recv(4)
                print("Wanted 4 bytes got " + str(len(a)) + " bytes")

                if len(a) < 4:
                    raise Exception(
                        "Client closed socket, ending client thread")

                messageLength = struct.unpack('i', a)[0]
                print("Message Length: ", messageLength)
                data = bytearray()
                while messageLength > len(data):
                    data += self.mySocket.recv(messageLength - len(data))

                newcommand = pickle.loads(data)
                print("Command is: ", newcommand.command)

                if newcommand.command == "GetSong":
                    flag = 0
                    print("Sending song")
                    musics = getpathlist('./serverData', 'mp3')
                    print(musics)
                    replyCommand = Command()  # Make a new command
                    replyCommand.command = "Song Reply"  # Set the command type to Song Reply
                    md5list = []
                    for item in musics:
                        md5list.append(md5ID(str('./serverData/' + item)))
                    print(md5list)
                    for i in range(len(md5list)):
                        if str(newcommand.payload) == str(md5list[i]):
                            # Open the file, read it in, and use it as the
                            # payload
                            f = open('./serverData/' + musics[i], 'rb')
                            print("Sending file: ", musics[i])
                            replyCommand.payload = f.read()
                            f.close()
                            flag = 1
                    if flag == 0:
                        print("File not found")
                        replyCommand.payload = None

                elif newcommand.command == "listOfSongList":
                    list = getpathlist('./serverData', 'txt')
                    print("Sending song list")
                    replyCommand = Command()
                    replyCommand.command = "listOfSongList"
                    dicts = []
                    for item in list:
                        dict = {'ID': md5ID(item), 'Name': item}
                        dicts.append(dict)
                    print(dicts)
                    savedicfile('./allList.txt', dicts)
                    replyCommand.payload = list
                elif newcommand.command.split('.')[0] == 'GETPLAYLIST':
                    flag = 0
                    print("Sending song list")
                    replyCommand = Command()  # Make a new command
                    replyCommand.command = "songList Reply"  # Set the command type to Song Reply
                    lists = readdicfile('./allList.txt')
                    print(lists)
                    for item in lists:
                        if newcommand.command.split('.')[1] == item['ID']:
                            print("Sending file: ", newcommand.payload)
                            if os.path.exists('./serverData/' + item['Name']):
                                replyCommand.payload = readdicfile(
                                    './serverData/' + item['Name'])
                                flag = 1
                    if flag == 0:
                        raise Exception("File not found")
                        replyCommand.payload = None
                elif newcommand.command.split('.')[0] == "sendSong":
                    f = open('./serverData/' +
                             str(newcommand.command.split('.')[1] +
                                 '.mp3'), "wb")
                    f.write(newcommand.payload)
                    f.close()
                    replyCommand = Command()
                    replyCommand.command = "get a new song " + \
                        newcommand.command.split('.')[1] + '.mp3'
                    list = readdicfile(
                        './serverData/' +
                        newcommand.command.split('.')[3] +
                        '.txt')
                    md5 = md5ID(
                        './serverData/' +
                        newcommand.command.split('.')[1] +
                        '.mp3')
                    Dic = {'ID': md5, 'Name': newcommand.command.split('.')[
                        1] + '.mp3'}
                    if list is None:
                        list = [Dic]
                    else:
                        list.append(Dic)
                    print(Dic)
                    print(newcommand.command.split('.')[3] + '.txt\n')
                    savedicfile(
                        './serverData/' +
                        newcommand.command.split('.')[3] +
                        '.txt',
                        list)
                    print("get a new song ")
                    replyCommand.payload = 'ok'

                elif newcommand.command.split('.')[0] == "deleteSong":
                    print(os.getcwd())
                    print("delete" + newcommand.payload + '\n')
                    replyCommand = Command()
                    replyCommand.command = "delete a  song "
                    list = readdicfile(
                        './serverData/' +
                        newcommand.command.split('.')[1] +
                        '.txt')
                    for item in list:
                        if item['Name'] == newcommand.payload:
                            list.remove(item)
                    savedicfile(
                        './serverData/' +
                        newcommand.command.split('.')[1] +
                        '.txt',
                        list)
                    if os.path.exists('./serverData/' + newcommand.payload):
                        md5 = md5ID('./serverData/' + newcommand.payload)
                        os.remove('./serverData/' + newcommand.payload)
                        replyCommand.payload = md5

                elif newcommand.command == "CREATEPLAYLIST":
                    print("create playlist")
                    replyCommand = Command()
                    replyCommand.command = "PLAYLISTCREATED"
                    if os.path.exists(
                            './serverData/' + newcommand.payload + '.txt'):
                        replyCommand.payload = 'wrong name'
                    else:
                        f = open(
                            './serverData/' +
                            newcommand.payload +
                            '.txt',
                            'wb')
                        f.close()
                        dicts = readdicfile('./allList.txt')
                        dicts.append(
                            {'ID': md5ID(newcommand.payload + '.txt'), 'Name': newcommand.payload + '.txt'})
                        savedicfile('./allList.txt', dicts)
                        replyCommand.payload = 'ok'
                # Serialize the class to a binary array
                packedData = pickle.dumps((replyCommand))
                # Length of the message is just the length of the array
                self.mySocket.sendall(struct.pack("i", len(packedData)))
                self.mySocket.sendall(packedData)
        except Exception as e:
            print("Closing socket")
            print(e)

        # start our main....


host = "localhost"
port = 4567


def Connect(host, port):
    Socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        Socket.bind((host, port))
        Socket.listen(5)
    except BaseException:
        pass
    return Socket


print("Listening...")
serverSocket = Connect(host, port)
while True:
    try:
        (clientSocket, address) = serverSocket.accept()
        print("\n\nGot incoming connection")
        # make a new instance of our thread class to handle requests
        newThread = SocketThread(clientSocket)
        newThread.start()  # start the thread running....
    except socket.error:
        print(socket.error)
