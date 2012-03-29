#!/usr/bin/python
# -*- coding: utf-8 -*- 

"""
gmailからメールを送るプログラム
起動時に引数を1つ与えると，それを件名として，
2つ与えると，それを件名と本文して送信する．
"""

__author__ = 'Yuta Hayashibe' 
__version__ = ""
__copyright__ = ""
__license__ = "Public Domain"


import smtplib
from email.MIMEText import MIMEText
from email.Header import Header
from email.Utils import formatdate

ADDR = "あなたのGMAILのメールアドレス"
PASS = "あなたのGMAILのパスワード"
from_address = "あなたのGMAILのメールアドレス"
to_address   = "送信先のメールアドレス"

def send_via_gmail(from_addr, to_addr, msg):
    s = smtplib.SMTP('smtp.gmail.com', 587)
    s.ehlo()
    s.starttls()
    s.ehlo()
    s.login(ADDR, PASS)
    s.sendmail(from_addr, [to_addr], msg.as_string())
    s.close()

if __name__ == "__main__":
    import sys
    argvs = sys.argv  # コマンドライン引数を格納したリストの取得
    argc = len(argvs) # 引数の個数
    INCODE = 'utf-8'

    if (argc == 1):
        subject = u"実験終了通知"
    	text    = u"実験が終了したようです"
    elif (argc == 2):
        subject = unicode(argvs[1], INCODE)
    	text    = u"実験が終了したようです"
    elif (argc == 3):
        subject = unicode(argvs[1], INCODE)
        text = unicode(argvs[2], INCODE)

    import socket
    text += u"\n実験監理システム\nHost:" + socket.gethostname()
    charset = "ISO-2022-JP"
    
    msg = MIMEText(text.encode(charset),"plain",charset)
    msg["Subject"] = Header(subject,charset)
    msg["From"]    = from_address
    msg["To"]      = to_address
    msg["Date"]    = formatdate(localtime=True)

    send_via_gmail(from_address, to_address, msg)

