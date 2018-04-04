import pymysql
import oss2

from datetime import datetime
from json import dumps
import os
import os.path
from pathlib import Path
import random
import shutil
import sys
import traceback

##  SQL database

_conns = { }

def connect_db(db = 'main', user = 'dev', password = 'Oslab1435go'):
    if db not in _conns:
        _conns[db] = pymysql.connect(
            host = 'rm-2ze710p770jn5k96o.mysql.rds.aliyuncs.com',
            user = user,
            password = password,
            db = db,
            charset = 'utf8'
        )
    return _conns[db]

def query(db, sql, args = None):
    cursor = connect_db(db).cursor()
    if args is None:
        cursor.execute(sql)
    else:
        cursor.execute(sql, args)
    ret = [ ]
    while True:
        record = cursor.fetchone()
        if record is None: break
        if len(record) == 1:
            ret.append(record[0])
        else:
            ret.append(record)
    if len(ret) == 0: return None
    if len(ret) == 1: return ret[0]
    return ret

def commit(db, sql, args):
    db = connect_db(db)
    db.cursor().execute(sql, args)
    db.commit()


## Aliyun OSS

_bkts = { }

def oss(bucket = 'lxapk'):
    if bucket not in _bkts:
        auth = oss2.Auth('LTAI19YfqOSkHpRW', 'pmxBQkjnHYmnTmoExeG5w7Vdk4laMK')
        _bkts[bucket] = oss2.Bucket(auth, 'vpc100-oss-cn-beijing.aliyuncs.com', bucket)
    return _bkts[bucket]

def oss_upload(local, remote, bucket = 'lxapk'):
    if type(local) is bytes:
        oss(bucket).put_object(remote, local)
    else:
        oss(bucket).put_object_from_file(remote, local)

def oss_download(remote, local = None, bucket = 'lxapk'):
    if local is None:
        local, f = create_temp_file()
    oss(bucket).get_object_to_file(remote, local)
    return local

def oss_download_apk(pkg, md5, sha256):
    key = pkg + '/' + md5 + '-' + sha256 + '.apk'
    return oss_download(key)


##  local resource file

def res_file_path(script_path, file_name):
    relative = os.path.dirname(script_path)
    absolute = os.path.abspath(relative)
    return os.path.join(absolute, file_name)

def open_res_file(script_path, file_name, mode = 'r'):
    return open(res_file_path(script_path, file_name), mode)


##  file system

def _prepare_path(path):
    Path(os.path.dirname(path)).mkdir(parents = True, exist_ok = True)

def mv(src, dst):
    _prepare_path(dst)
    shutil.move(src, dst)

def rm(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    else:
        os.remove(path)

def create_file(path, data = None):
    _prepare_path(path)
    if type(data) is str:
        data = data.encode('utf8')
    f = open(path, 'wb')
    if data is None:
        return f
    else:
        f.write(data)

def ls(path):
    return sorted(os.listdir(path))

def file_exists(path):
    return os.path.isfile(path)

_global_temp_file_dir = '/tmp/lx/'
_temp_file_idx = 0

def _temp_file_dir():
    return _global_temp_file_dir + str(os.getpid()) + '/'

def temp_file_dir():
    path = _temp_file_dir()
    _prepare_path(path)
    return path

def create_temp_file():
    global _temp_file_idx
    path = _temp_file_dir() + str(_temp_file_idx)
    _temp_file_idx += 1
    f = create_file(path)
    return path, f

def clear_temp_file():
    rm(_temp_file_dir())

def open_file(path, mode = 'r'):
    try:
        return open(path, mode)
    except FileNotFoundError:
        return None

def read_lines(path):
    f = open_file(path)
    if f is None: return None
    return [ l[:-1] for l in f ]


##  misc

def json(obj, pretty = False):
    if pretty:
        return dumps(obj, ensure_ascii = False, indent = 2, sort_keys = True)
    else:
        return dumps(obj, ensure_ascii = False, separators = (',', ':'))

def decision(probability):
    return random.random() <= probability

def exit(code = 0):
    sys.exit(code)

#def fork(n):
#    global _echo_log
#    _echo_log = False
#    for i in range(n - 1):
#        if os.fork() == 0:
#            return


##  log

_log_file = None
_verbose_log = False
_echo = True
_ignore_error = False

def enable_verbose_log():
    global _verbose_log
    _verbose_log = True

def disable_echo():
    global _echo
    _echo = False

def ignore_erro():
    global _ignore_error
    _ignore_error = True

def log(raw_msg):
    msg = _log_msg(raw_msg)
    _save_log(msg)
    if _echo: print(msg, end = '')

def logv(raw_msg):
    msg = _log_msg(raw_msg)
    if _verbose_log: _save_log(msg)
    if _echo: print(msg, end = '')

def error(e):
    msg = _log_msg(e)
    _save_log(msg)
    print(msg, end = '')
    if _ignore_error: return
    if type(e) is int:
        exit(e)
    else:
        exit(1)

def _save_log(msg):
    global _log_file
    if _log_file is None:
        file_name = 'log/%d.txt' % os.getpid()
        _prepare_path(file_name)
        _log_file = open(file_name, 'a')
    _log_file.write(msg)
    _log_file.flush()

def _log_msg(msg):
    if isinstance(msg, Exception):
        msg = str(msg) + '\n' + traceback.format_exc()
    return '[%s] %s\n' % (_time(), msg)

def _time():
    return str(datetime.now()).split('.')[0]
