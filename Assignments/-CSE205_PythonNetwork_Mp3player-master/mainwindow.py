# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '1.ui'
#
# Created by: PyQt5 UI code generator 5.9.1
#
# WARNING! All changes made in this file will be lost!
import socket
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtMultimedia import *
from os.path import expanduser
import struct
import pickle
import hashlib
import os
import time


def md5ID(fileOrstring):
    # get md5 from a file object
    if os.path.isfile(fileOrstring):
        f = open(fileOrstring, 'rb')
        md5_file = hashlib.md5()
        md5_file.update(f.read())
        hash_code = md5_file.hexdigest()
        f.close()
        md5 = str(hash_code).lower()
    else:
        # get md5 from a string object
        my_md5 = hashlib.md5()
        my_md5.update(fileOrstring.encode("utf8"))
        hash_code = my_md5.hexdigest()
        md5 = str(hash_code).lower()
    return md5


class client():
    host = "localhost"
    port = 4567

    def getSong(s, songID):
        try:
            getCommand = Command()
            getCommand.command = "GetSong"
            getCommand.payload = songID
            packedData = pickle.dumps(getCommand)
            totalLen = len(packedData)

            s.sendall(struct.pack("i", totalLen))
            s.sendall(packedData)

            replyLen = struct.unpack("i", s.recv(4))[0]
            data = bytearray()
            while (replyLen > len(data)):
                data += s.recv(replyLen - len(data))
            replyCommand = pickle.loads(data)
            # the file is stored with name of md5
            # so that there are no duplicate files
            if replyCommand.payload is not None:
                f = open('./clientmusic/' + songID + '.mp3', "wb")
                f.write(replyCommand.payload)
                f.close()
                return './clientmusic/' + songID + '.mp3'
            else:
                return None
        except BaseException:
            print("wrong")

    def sendSong(s, song, songName, playlist):
        try:
            sendCommand = Command()
            sendCommand.command = "sendSong" + "." + songName + '.' + playlist
            if os.path.isfile(song):
                # Open the file, read it in, and use it as the payload
                f = open(song, 'rb')
                print("Sending file: ", song)
                sendCommand.payload = f.read()
                f.close()
                packedData = pickle.dumps(sendCommand)
                totalLen = len(packedData)

                s.sendall(struct.pack("i", totalLen))
                s.sendall(packedData)
                replyLen = struct.unpack("i", s.recv(4))[0]
                data = bytearray()
                while (replyLen > len(data)):
                    data += s.recv(replyLen - len(data))
                replyCommand = pickle.loads(data)
                return replyCommand.payload == 'ok'
        except Exception as e:
            print("Error occured: ", e)
            return False

    def listSongList(s):
        try:

            getCommand = Command()
            getCommand.command = 'listOfSongList'
            packedData = pickle.dumps(getCommand)
            totalLen = len(packedData)

            s.sendall(struct.pack("i", totalLen))
            s.sendall(packedData)

            replyLen = struct.unpack("i", s.recv(4))[0]
            data = bytearray()
            while (replyLen > len(data)):
                data += s.recv(replyLen - len(data))
            replyCommand = pickle.loads(data)
            return replyCommand.payload
        except Exception as e:
            print("Error occured: ", e)

    def GETPLAYLIST(s, playlistID):
        try:
            getCommand = Command()
            getCommand.command = "GETPLAYLIST" + '.' + playlistID
            packedData = pickle.dumps(getCommand)
            totalLen = len(packedData)

            s.sendall(struct.pack("i", totalLen))
            s.sendall(packedData)

            replyLen = struct.unpack("i", s.recv(4))[0]
            data = bytearray()
            while (replyLen > len(data)):
                data += s.recv(replyLen - len(data))
            replyCommand = pickle.loads(data)
            return replyCommand.payload
        except Exception as e:
            print("Error occured: ", e)

    def createplaylist(s, listName):
        try:
            CreateCommand = Command()
            CreateCommand.command = 'CREATEPLAYLIST'
            CreateCommand.payload = listName
            packedData = pickle.dumps(CreateCommand)
            totalLen = len(packedData)

            s.sendall(struct.pack("i", totalLen))
            s.sendall(packedData)

            replyLen = struct.unpack("i", s.recv(4))[0]
            data = bytearray()
            while (replyLen > len(data)):
                data += s.recv(replyLen - len(data))
            replyCommand = pickle.loads(data)
            return replyCommand.payload == 'ok'
        except Exception as e:
            print("Error occured: ", e)

    def deleteSong(s, item, playlist):
        try:
            deleteCommand = Command()
            deleteCommand.command = "deleteSong" + '.' + playlist
            deleteCommand.payload = item
            packedData = pickle.dumps(deleteCommand)
            totalLen = len(packedData)

            s.sendall(struct.pack("i", totalLen))
            s.sendall(packedData)

            replyLen = struct.unpack("i", s.recv(4))[0]
            data = bytearray()
            while (replyLen > len(data)):
                data += s.recv(replyLen - len(data))
            replyCommand = pickle.loads(data)
            # the server will trans a md5
            print(replyCommand.payload + '.mp3')
            if os.path.exists(
                './clientmusic/' +
                replyCommand.payload +
                    '.mp3'):
                os.remove('./clientmusic/' + replyCommand.payload + '.mp3')
                return True
            else:
                return False
        except Exception as e:
            print("Error occured: ", e)
            return False


class Command:
    command = ""
    payload = ""


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(653, 420)
        # player
        self.player = QMediaPlayer()
        self.nowPlaylist = QMediaPlaylist()
        # allow users to seek position
        self.player.positionChanged.connect(self.sliderchange)

        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout_7 = QtWidgets.QVBoxLayout(self.centralwidget)
        self.verticalLayout_7.setObjectName("verticalLayout_7")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.verticalLayout_5 = QtWidgets.QVBoxLayout()
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName("label_3")
        self.verticalLayout_5.addWidget(self.label_3)
        self.listWidget = QtWidgets.QListWidget(self.centralwidget)
        self.listWidget.setObjectName("listWidget")
        self.verticalLayout_5.addWidget(self.listWidget)
        self.horizontalLayout_2.addLayout(self.verticalLayout_5)
        spacerItem = QtWidgets.QSpacerItem(
            17,
            274,
            QtWidgets.QSizePolicy.Minimum,
            QtWidgets.QSizePolicy.Expanding)
        self.horizontalLayout_2.addItem(spacerItem)
        self.verticalLayout_6 = QtWidgets.QVBoxLayout()
        self.verticalLayout_6.setObjectName("verticalLayout_6")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setAlignment(QtCore.Qt.AlignCenter)
        self.label_4.setObjectName("label_4")
        self.verticalLayout_6.addWidget(self.label_4)
        self.listWidget_2 = QtWidgets.QListWidget(self.centralwidget)
        self.listWidget_2.setObjectName("listWidget_2")
        self.verticalLayout_6.addWidget(self.listWidget_2)
        self.horizontalLayout_2.addLayout(self.verticalLayout_6)
        self.verticalLayout_4 = QtWidgets.QVBoxLayout()
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.pushButton_5 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_5.setCheckable(False)
        self.pushButton_5.setObjectName("pushButton_5")
        self.verticalLayout_4.addWidget(self.pushButton_5)
        self.pushButton_4 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_4.setObjectName("pushButton_4")
        self.verticalLayout_4.addWidget(self.pushButton_4)
        self.pushButton_6 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_6.setObjectName("pushButton_6")
        self.verticalLayout_4.addWidget(self.pushButton_6)
        self.pushButton_3 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_3.setObjectName("pushButton_3")
        self.verticalLayout_4.addWidget(self.pushButton_3)
        self.horizontalLayout_2.addLayout(self.verticalLayout_4)
        self.horizontalLayout_2.setStretch(0, 2)
        self.horizontalLayout_2.setStretch(2, 5)
        self.verticalLayout_7.addLayout(self.horizontalLayout_2)
        self.horizontalLayout_4 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        spacerItem1 = QtWidgets.QSpacerItem(
            20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)
        spacerItem2 = QtWidgets.QSpacerItem(
            40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.verticalLayout.addItem(spacerItem2)
        self.lcdNumber = QtWidgets.QLCDNumber(self.centralwidget)
        self.lcdNumber.setSmallDecimalPoint(False)
        self.lcdNumber.setProperty("value", 0.0)
        self.lcdNumber.setProperty("intValue", 0)
        self.lcdNumber.setObjectName("lcdNumber")
        self.verticalLayout.addWidget(self.lcdNumber)
        self.horizontalLayout_3.addLayout(self.verticalLayout)
        self.dial = QtWidgets.QDial(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(
            QtWidgets.QSizePolicy.Expanding,
            QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            self.dial.sizePolicy().hasHeightForWidth())
        self.dial.setSizePolicy(sizePolicy)
        self.dial.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.dial.setOrientation(QtCore.Qt.Horizontal)
        self.dial.setObjectName("dial")
        self.horizontalLayout_3.addWidget(self.dial)
        self.horizontalLayout_3.setStretch(0, 1)
        self.horizontalLayout_3.setStretch(1, 4)
        self.horizontalLayout_4.addLayout(self.horizontalLayout_3)
        self.verticalLayout_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setContentsMargins(-1, 9, 0, 11)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalSlider = QtWidgets.QSlider(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(
            QtWidgets.QSizePolicy.Expanding,
            QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            self.horizontalSlider.sizePolicy().hasHeightForWidth())
        self.horizontalSlider.setSizePolicy(sizePolicy)
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.verticalLayout_2.addWidget(self.horizontalSlider)
        self.label = QtWidgets.QLabel(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(
            QtWidgets.QSizePolicy.Preferred,
            QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        self.verticalLayout_2.addWidget(self.label)
        self.verticalLayout_2.addWidget(self.label)
        self.verticalLayout_3.addLayout(self.verticalLayout_2)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setContentsMargins(0, 0, -1, 9)
        self.horizontalLayout.setSpacing(28)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(
            QtWidgets.QSizePolicy.Expanding,
            QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            self.pushButton.sizePolicy().hasHeightForWidth())
        self.pushButton.setSizePolicy(sizePolicy)
        self.pushButton.setObjectName("pushButton")
        self.horizontalLayout.addWidget(self.pushButton)
        self.pushButton_2 = QtWidgets.QPushButton(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(
            QtWidgets.QSizePolicy.Expanding,
            QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            self.pushButton_2.sizePolicy().hasHeightForWidth())
        self.pushButton_2.setSizePolicy(sizePolicy)
        self.pushButton_2.setShortcut("")
        self.pushButton_2.setObjectName("pushButton_2")
        self.horizontalLayout.addWidget(self.pushButton_2)
        self.verticalLayout_3.addLayout(self.horizontalLayout)
        self.horizontalLayout_4.addLayout(self.verticalLayout_3)
        self.horizontalLayout_4.setStretch(0, 1)
        self.horizontalLayout_4.setStretch(1, 3)
        self.verticalLayout_7.addLayout(self.horizontalLayout_4)
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.listWidget.itemDoubleClicked['QListWidgetItem*'].connect(
            self.getsongsfromlist)
        self.dial.setValue(self.player.volume())
        self.lcdNumber.display(self.player.volume())
        self.dial.valueChanged['int'].connect(self.lcdNumber.display)
        self.horizontalSlider.setTracking(False)
        self.horizontalSlider.valueChanged['int'].connect(self.sliderchange)
        self.horizontalSlider.sliderMoved.connect(self.changePosition)
        self.listWidget_2.itemDoubleClicked['QListWidgetItem*'].connect(
            self.pushButton.click)

        self.pushButton.clicked.connect(self.downloadplay)
        self.pushButton_2.clicked.connect(self.stop)
        self.pushButton_3.clicked.connect(self.newlist)
        self.pushButton_4.clicked.connect(self.delete)
        self.pushButton_5.clicked.connect(self.addnewsong)
        self.pushButton_6.clicked.connect(self.getAllsongLists)
        self.dial.valueChanged.connect(self.volumechange)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def newlist(self):

        text, ok = QInputDialog.getText(self.pushButton_3, 'Input Dialog',
                                        'Enter the list name:')
        if ok:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((client.host, client.port))
            if client.createplaylist(s, str(text)):
                self.listWidget.addItem(str(text) + '.txt')
            else:
                self.statusbar.showMessage('error,this name may be used')

    def getsongsfromlist(self):
        self.listWidget_2.clear()
        self.nowPlaylist.clear()
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((client.host, client.port))
        songs = client.GETPLAYLIST(
            s, md5ID(
                self.listWidget.item(
                    self.listWidget.currentRow()).text()))
        if songs is not None:
            for item in songs:
                self.listWidget_2.addItem(item['Name'])
            self.statusbar.showMessage('downloading completed')
        else:
            self.statusbar.showMessage("")
        s.close()

    def sliderchange(self):
        self.horizontalSlider.setRange(0, self.player.duration())
        self.horizontalSlider.setValue(self.player.position())
        self.label.setText('%d:%02d' %
                           (int(self.player.position() /
                                60000), int((self.player.position() /
                                             1000) %
                                            60)))

    def volumechange(self):
        a = self.dial.value()
        self.player.setVolume(a)

    def stop(self):
        if self.player.state() == QMediaPlayer.PlayingState:
            self.player.stop()
        elif self.player.state() == QMediaPlayer.StoppedState:
            pass

    def getAllsongLists(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((client.host, client.port))
        self.listWidget.clear()
        list = client.listSongList(s)
        if list is not None:
            for item in list:
                self.listWidget.addItem(item)
        else:
            self.statusbar.showMessage('error occur')
        s.close()

    def delete(self):
        self.player.stop()
        self.nowPlaylist.clear()
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((client.host, client.port))
        if self.listWidget_2.currentRow() >= 0:
            playlist = self.listWidget.item(self.listWidget.currentRow()).text()
            if client.deleteSong(
                    s,
                    self.listWidget_2.takeItem(
                        self.listWidget_2.currentRow()).text(),
                    playlist):
                self.statusbar.showMessage('Delete compeleted')
            else:
                self.statusbar.showMessage(
                    'Delete error, this file may not exist in server')
        s.close()

    def downloadplay(self):
        self.player.stop()
        self.nowPlaylist.clear()
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((client.host, client.port))
        if self.listWidget_2.currentRow()>=0:
            playlist = client.GETPLAYLIST(
                s, md5ID(
                    self.listWidget.item(
                        self.listWidget.currentRow()).text()))
            s.close()
            for item in playlist:
                if item['Name'] == self.listWidget_2.item(
                        self.listWidget_2.currentRow()).text():
                    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    s.connect((client.host, client.port))
                    self.statusbar.showMessage('Downloading')
                    url = client.getSong(s, item['ID'])
                    if url is not None:
                        self.nowPlaylist.addMedia(QMediaContent(QtCore.QUrl(url)))
                        time.sleep(1)
                        s.close()
                        self.player.setPlaylist(self.nowPlaylist)
                        self.statusbar.showMessage('Playing-- '+self.listWidget_2.item(
                        self.listWidget_2.currentRow()).text())
                    else:
                        self.statusbar.showMessage('')
        self.player.play()

    def addnewsong(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((client.host, client.port))
        if self.listWidget.currentRow() >= 0:
            fileChoosen = QFileDialog.getOpenFileUrl(
                self.pushButton_5, 'Open Music File', expanduser('~'))
            if fileChoosen is not None:
                self.nowPlaylist.addMedia(QMediaContent(fileChoosen[0]))
                QStringList = fileChoosen[0].toString().split('/')
                if len(QStringList) > 1:
                    self.listWidget_2.addItem(
                        QStringList[len(QStringList) - 1])
                    self.statusbar.showMessage('uploading to server')
                    client.sendSong(
                        s,
                        QStringList[3] +
                        '/' +
                        QStringList[4] +
                        '/' +
                        '/' +
                        QStringList[5],
                        QStringList[5],
                        self.listWidget.item(
                            self.listWidget.currentRow()).text())
                    self.statusbar.showMessage('uploading complete')
            s.close()
        else:
            self.statusbar.showMessage('no available play list')
            s.close()

    def changePosition(self, position):
        if self.player.isSeekable():
            self.player.setPosition(position)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.label_3.setText(_translate("MainWindow", "play lists"))
        self.label_4.setText(_translate("MainWindow", "songs"))
        self.pushButton_5.setText(_translate("MainWindow", "add song(A)"))
        self.pushButton_5.setShortcut(_translate("MainWindow", "A"))
        self.pushButton_4.setText(_translate("MainWindow", "delete song(D)"))
        self.pushButton_4.setShortcut(_translate("MainWindow", "D"))
        self.pushButton_6.setText(_translate("MainWindow", "get all lists"))
        self.pushButton_3.setText(_translate("MainWindow", "new playlist(N)"))
        self.pushButton_3.setShortcut(_translate("MainWindow", "N"))
        self.label.setText(_translate("MainWindow", "timing"))
        self.pushButton.setText(_translate("MainWindow", "Download&&Play(return)"))
        self.pushButton.setShortcut(_translate("MainWindow", "Return"))
        self.pushButton_2.setText(_translate("MainWindow", "stop"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    myshow = QtWidgets.QMainWindow()
    mainwindow = Ui_MainWindow()
    mainwindow.setupUi(myshow)
    myshow.show()
    sys.exit(app.exec_())
