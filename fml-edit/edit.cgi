#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cgi
import os
import hashlib
import sys

from xml.sax.saxutils import * #for escape


FOLDER = ["/home/example/fml/spool/ml/member-ml/"]

MEMBERS="members"
ACTIVES="actives"
BK="./bk"


def getEditHTML(id):
    folder_name = FOLDER[id]
    body = open(folder_name + ACTIVES).read()
    hashtxt = hashlib.md5(body).hexdigest()
    ret = """<?xml version="1.0" encoding="UTF-8"?>
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja" lang="ja">
    <head profile="http://www.w3.org/2005/10/profile"> 
    <title>編集</title> 
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    </head> 
<body>
    <h1>"%s"を編集</h1>
    1行に1つメールアドレスを書く．
    「#」以降にはコメントを書ける．
    <form action="https://www.ito-foundation.or.jp/miyabikai/ml/edit" method="post">
            <textarea name="content" rows="50" cols="120">%s</textarea>
            <br />
            HASH : %s <br />
            <input type="hidden" name="hash" value="%s" />
            <input type="hidden" name="id" value="%s" />
            <br />
            <input type="submit" value="送信" />
    </form>
    </body>
</html>
""" % (folder_name, escape(body), hashtxt, hashtxt, id)
    return ret


def update(query):
    id = query.getvalue('id')
    org_hash = query.getvalue('hash')
    content = query.getvalue('content')
    folder_name = FOLDER[int(id)]

    body = open(folder_name + ACTIVES).read()
    hashtxt = hashlib.md5(body).hexdigest()
    
    if org_hash != hashtxt : 
        return "元ファイルが変更されています org_hash => hashtxt " % (org_hash, hashtxt)

    try:
        bk = "%s/%s/%s" % (BK, id, hashtxt)
        f = open(bk, 'w')    
        f.write(body)
        f.close()

        f = open(folder_name + ACTIVES, 'w')    
        f.write(content)
        f.close()

        f = open(folder_name + MEMBERS, 'w')    
        f.write(content)
        f.close()


    except:
        return "エラー発生"


    ret = """<?xml version="1.0" encoding="UTF-8"?>
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja" lang="ja">
    <head profile="http://www.w3.org/2005/10/profile"> 
    <title>完了</title> 
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    </head> 
<body>
    <h1>"%s"を上書きしました</h1>

    <a href=".">戻る</a>
</body>
</html>
""" % (folder_name)
    return ret


def choose():
    msg = ""
    for i,k in enumerate(FOLDER):
        msg += """
        <a href="%s?id=%d">%s</a> <br />
         - <a href="%s/%d">backup</a> <br />
        <br />
        <br />
        """ % (__file__, i, k, BK, i)

    ret = """<?xml version="1.0" encoding="UTF-8"?>
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja" lang="ja">
    <head profile="http://www.w3.org/2005/10/profile"> 
    <title>編集</title> 
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    </head> 
<body>
    <h1>編集するMLを選択してください</h1>

    %s


        <br />
    <a href=".">戻る</a>
</body>
</html>
""" % (msg)
    return ret




if __name__=='__main__':
    if ( os.environ['REQUEST_METHOD'] == "GET" ):
        query_string_key = 'QUERY_STRING'
        if query_string_key in os.environ:
            query = cgi.parse_qs(os.environ[query_string_key])
        else:
            query = {}

        id = query.get('id', None)
        if not id:
            ret = choose()
        else:
            id = int(id[0])
            try:
                ret = getEditHTML(id)
            except:
                ret = "ERROR OCCUED BY ID:%s" % id
    elif ( os.environ['REQUEST_METHOD'] == "POST" ):
        query = cgi.FieldStorage()
        ret = update(query)


    sys.stdout.write("""Content-type: text/html; charset=utf-8\n\n""")
    print ret

