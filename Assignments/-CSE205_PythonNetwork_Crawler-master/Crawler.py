## Crawler
## recommend website: bilibili.com, sina.com.cn, 
import os
import threading
from queue import Queue
import gzip
import socket
from html.parser import HTMLParser
try:
    import ssl
except ImportError:
    pass
global errortimes
socket.setdefaulttimeout(45)
def get_http(ulr,host):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        if host != '' or host != None:
            s.connect((host, 80))
            if ulr == None or ulr=='':
                s.send(b'GET / HTTP/1.1\r\nHost:' + bytes(host,encoding="utf-8") +b'\r\nUser-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36'+ b'\r\nConnection: close\r\n\r\n')
            else:
                s.send(b'GET '+bytes(ulr, encoding="utf-8")+b' HTTP/1.1\r\nHost:' +bytes(host, encoding = "utf-8")+b'\r\nUser-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36'+b'\r\nConnection: close\r\n\r\n')
            buffer = []
            try:
                while True:
                    d = s.recv(512)
                    if d:
                        buffer.append(d)
                    else:
                        break
                data = b''.join(buffer)
                s.close()
            except Exception as e:
                print('socket recv fails: '+host+'/'+ulr)
                print(e)
            else:
                if data != None:
                    return processdata(data)
    except Exception as e:
##        global errortimes
##        errortimes += 1
        print('socket connection error')
        print([ulr,host])
        print(e)
def get_https(ulr,host):
    try:
        if host !='' or host !=None:
            context = ssl.create_default_context()
            conn = context.wrap_socket(socket.socket(socket.AF_INET),server_hostname=host)
            conn.connect((host, 443))
            if ulr == None or ulr =='':
                conn.sendall(b'GET / HTTP/1.1\r\nHost:' + bytes(host,encoding="utf-8") +b'\r\nUser-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36' +b'\r\nConnection: close\r\n\r\n')
            else:
                conn.sendall(b'GET '+bytes(ulr, encoding = "utf-8")+b' HTTP/1.1\r\nHost:' +bytes(host, encoding = "utf-8")+b'\r\nUser-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36'+b'\r\nConnection: close\r\n\r\n')
            buffer = []
            try:
                while True:
                    d = conn.recv(512)
                    if d:
                        buffer.append(d)
                    else:
                        break
                data = b''.join(buffer)
                conn.close()
            except Exception as e:
##                errortimes += 1
                print('socket recv fails: '+host+'/'+ulr)
                print(e)
            else:
                if data != None:
                    return processdata(data)
    except Exception as e:
##        global errortimes
##        errortimes += 1
        print('socket connection error')
        print([ulr,host])
        print(e)
def processdata(Hdata):
    try:
        try:
            header, data = Hdata.split(b'\r\n\r\n', 1)
            headerlist = header.split(b'\r\n')
        except:
            return None
        if b'200' in headerlist[0]:
            return [header, data]
        elif b'301' or b'302' b'303' in headerlist[0]:
            for item in headerlist:
                if b'Location' in item:
                    try:
                        locationlist = gzip.decompress(item).decode('utf-8').replace(' ', '').split(':')
                    except:
                        locationlist = item.decode('utf-8').replace(' ', '').split(':')
                    link = locationlist[len(locationlist) - 1]
                    [url, host] = get_host(link)
                    if 'https' in locationlist:
                        return get_https(url, host)
                    if 'http' in locationlist:
                        return get_http(url, host)
        else:
            print(headerlist)
    except Exception as e:
        print('process data fail')
        print(e)
class imgParser(HTMLParser):
        def __init__(self):
            HTMLParser.__init__(self)
            self.imgs = []
        def handle_starttag(self, tags, attrs):
            if 'img' in tags:
                for attr in attrs:
                    for t in attr:
                        if t !='' and t != None:
                            if 'png' in t.lower():
                                self.imgs.append(t)
                            elif'gif' in t.lower():
                                self.imgs.append(t)
                            elif 'webq' in t.lower():
                                self.imgs.append(t)
                            elif 'jpeg' in t.lower():
                                self.imgs.append(t)
                            elif 'jpg' in t.lower():
                                self.imgs.append(t)
                            else:
                                    pass
        def get_imgs(self):
            imgs=list(set(self.imgs))
            return imgs


class hrefParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.hrefs = []

    def handle_starttag(self, tags, attrs):
        TLD = ['.com.cn', '.cn', '.com', '.net', '.org', '.net.cn', '.hk', '.com.hk', '.eu', '.edu.cn']
        if 'a' in tags:
            for attr in attrs:
                for t in attr:
                    if t!=''and t!=None:
                        for item in TLD:
                            if item in t.lower():
                                self.hrefs.append(t)
                        if ('html' in t.lower() and 'html?' not in t.lower() and 'html;' not in t.lower()):
                            self.hrefs.append(t)
                        else:
                            pass

    def get_hrefs(self):
        hrefs = list(set(self.hrefs))
        return hrefs

def getImgList(ulr,host):
    parser = imgParser()
    try:
        html=get_http(ulr, host)
        try:
            htmlDATA=gzip.decompress(html[1]).decode("utf-8", "ignore")
        except:
            htmlDATA=html[1].decode("utf-8", "ignore")
        parser.feed(htmlDATA)
        try:
            imglist = imgParser.get_imgs(parser)
            return list(set(imglist))
        except:
            print('getImgList failed:')
    except Exception as e:
        print('get_http failed:')
        print(e)


def getHrefs(ulr,host):
    if host!='' and host != None:
        parser = hrefParser()
        try:
            html=get_http(ulr, host)
            if html != [] and html !=None:
                try:
                    htmlDATA=gzip.decompress(html[1]).decode("utf-8", "ignore")
                except:
                    htmlDATA=html[1].decode("utf-8", "ignore")
                parser.feed(htmlDATA)
                try:
                    hreflist = hrefParser.get_hrefs(parser)
                    if hreflist !=None:
                        return list(set(hreflist))
                except:
                    print('getHrefs failed:')
        except Exception as e:
            print('get-http failed:')
            print(e)

def getname(ulr):
    list =ulr.split('/')
    for post in ['.png', '.jpg', '.gif', '.jpeg', '.webq']:
        for item in list:
            if post in item.lower():
                name = item.replace('/','').replace('\\','').replace(':','').replace('*','')\
        .replace('?','').replace('<','').replace('>','').replace('|','')
    namelist = name.split('.',1)
    for item in ['png', 'jpg', 'gif', 'jpeg', 'webq']:
        if item in namelist[1].lower():
            namelist[1]=item
            return namelist[0]+'.'+namelist[1]
            break

def get_host(url):
    # flag=0
    TLD = ['.com.cn','.cn','.com','.net','.org','.net.cn','.hk','.com.hk','.eu','.edu.cn']
    list=url.split('/')
    if list!=[] and list !=None and len(list)>0:
        for item in list:
                if item !='' and item !=None:
                    for hostPost in TLD:
                        if hostPost in item.lower():
                            host = item
                            return [url.replace('http://','').replace('https://','').replace(item,'').replace('//',''),host]
                            break


class downloader(threading.Thread):
    def __init__(self, queue,host):
        threading.Thread.__init__(self)
        self.queue = queue
        self.host = host
    def run(self):
        while True:
            try:
                if (self.queue.empty)!=True:
                    link=self.queue.get()
                    if link != '' and link !=None:
                        try:
                            [ulr,host]=get_host(link)
                        except:
                            [ulr,host]=[link,self.host]
                        if host != None:
                            try:
                                data = get_http(ulr, host)
                                name = getname(ulr)
                            except:
                                print('downloading error:')
                            else:
                                with lock:
                                    if data != None:
                                        try:
                                            try:
                                                os.mkdir(host)
                                            except:
                                                pass
                                            if os.path.exists('./'+host+'/'+name)!=True:
                                                f = open('./'+host+'/'+name, 'wb')
                                                f.write(data[1])
                                                f.close()
                                        except:
                                            print('writting files error')
            finally:
                self.queue.task_done()

class imgCrawler (threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue
    def run(self):
        # global errortimes
        while True:
            try:
                start = time.clock()
                if (self.queue.empty) != True:
                    link=self.queue.get()
                    if link !=[] and link !=None and len(link)>0:
                        try:
                            [ulr, host] = get_host(link)
                            if host != None:
                                imglist = getImgList(ulr,host)
                                queue1 = Queue()
                                for i in range(0, 6):
                                    Downloader = downloader(queue1,host)
                                    Downloader.daemon = True
                                    Downloader.start()                                    
                                # if errortimes>50:
                                #     print('sleep 120 seconds')
                                #     time.sleep(120)
                                #     errortimes=0
                                if imglist != [] and imglist != None:
                                    while len(imglist)>0:
                                        queue1.put(imglist.pop())
                                queue1.join()
                                duration = time.clock() - start
                                print('One web finish: '+host+ulr+' $$time used: '+'%f' % (duration) + ' seconds\n')
                        except Exception as e:
                            print(e)
            finally:
                self.queue.task_done()

class linksCrawler (threading.Thread):
    def __init__(self, queue,List):
        threading.Thread.__init__(self)
        self.queue = queue
        self.linklist=List
    def run(self):
        while True:
            try:
                if (self.queue.empty)!=True:
                    try:
                        b=get_host(self.queue.get())
                    except Exception as e:
                        print('get host failed')
                        print(e)
                    if b!=[] and b!=None and len(b)>0:
                        a= getHrefs(b[0], b[1])
                        if a!=None:
                            self.linklist+=a
            finally:
                self.queue.task_done()

def getNextDepthLinks(links):
    List=[]
    queue2=Queue()
    for i in range(0, 6):
        linksLoader = linksCrawler(queue2,List)
        linksLoader.daemon = True
        linksLoader.start()
    for item in links:
        if item != '' and item != None:
                queue2.put(item)
    queue2.join()
    return list(set(List))
def depthLinks(URL,depth):
    listlinks=[URL]
    if depth > 1:
        nextdepthlinks = getNextDepthLinks(listlinks)
        for item in nextdepthlinks:
            listlinks+=depthLinks(item,depth-1)
    else:
        a=list(set(listlinks))
        while '' in a:
            a.remove('')
        return a
    a = list(set(listlinks))
    while '' in a:
        a.remove('')
    return a
def crawl(URL,depth):
    global errortimes
    errortimes =0
    linkslist=depthLinks(URL,depth)
    [url,host]=get_host(URL)
    try:
        os.mkdir(host)
    except OSError:
        pass
    os.chdir(host)
    print('Get links:')
    print('Crawl %d links'%(len(linkslist)))
    for item in linkslist:
         print(item)
    queue=Queue()
    for i in range(0,8):
        worker = imgCrawler(queue)
        worker.daemon = True
        worker.start()
    while len(linkslist)>0:
        queue.put(linkslist.pop())
    queue.join()

lock=threading.Lock()
import time
start = time.clock()

crawl('jd.com',2) #crawl(link,depth)

elapse=time.clock()-start
print('Time spent: %f'%(elapse)+' seconds')
