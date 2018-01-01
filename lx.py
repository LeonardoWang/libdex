import pymysql
import oss2

from json import dumps
import os.path


_conns = { }


def connect_db(db = 'main', user = 'dev', password = 'Oslab1435go'):
    _conns[db] = pymysql.connect(
        host = 'rm-2ze710p770jn5k96o.mysql.rds.aliyuncs.com',
        user = user,
        password = password,
        db = db,
        charset = 'utf8'
    )
    return _conns[db]


def query(sql, args = None, db = 'main'):
    cursor = _conns[db].cursor()
    if args is None:
        n = cursor.execute(sql)
    else:
        n = cursor.execute(sql, args)
    return [ cursor.fetchone() for i in range(n) ]


def file_path(script_path, file_name):
    relative = os.path.dirname(script_path)
    absolute = os.path.abspath(relative)
    return os.path.join(absolute, file_name)


def open_file(script_path, file_name, mode = 'r'):
    return open(file_path(script_path, file_name), mode)


def oss(bucket = 'lxapk'):
    auth = oss2.Auth('LTAI19YfqOSkHpRW', 'pmxBQkjnHYmnTmoExeG5w7Vdk4laMK')
    return oss2.Bucket(auth, 'vpc100-oss-cn-beijing.aliyuncs.com', bucket)


def json(obj, pretty = False):
    if pretty:
        return dumps(obj, ensure_ascii = False, indent = 2, sort_keys = True)
    else:
        return dumps(obj, ensure_ascii = False, separators = (',', ':'))
