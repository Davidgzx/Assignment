# -CSE205_PythonNetwork_Mp3player
This program is based on low-level socket programming and PyQt5.
Sever-client music player.(not support streaming)
Player only can play after downloading.

## environment and required library: 
 Windows 10 
 Python 3.6.3 
 PyQt5 (5.9.1) 
 sip (4.19.5)
## Run both files: 
- Python mainwindow.py 
- Python server.py
- ** Before running, please create two folds called "clientmusic" and "serverData"(put some mp3 files in it) in the same path of .py files
## Known bugs(commonly)
1. [Errno 10053] An established connection was aborted by the software in 
your host machine 
2. [Errno 10054] An existing connection was forcibly closed by the remote 
host 
3. When player play some of mp3 files, it may occur this error 
DirectShowPlayerService::doRender: Unresolved error code 0x80040266 
(IDispatch error #102) 
....
....
....
and many many so on
