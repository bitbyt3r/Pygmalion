import json
import base64
import six
import uuid
from json import scanner
from json.decoder import scanstring
import aiopg
import types

conversions = {
    uuid.UUID: str,
    aiopg.sa.result.ResultProxy: list,
    aiopg.sa.result.RowProxy: lambda i: dict(i.items()),
    types.GeneratorType: list,
}

class _WAMPJsonEncoder(json.JSONEncoder):

    def default(self, obj):
        if isinstance(obj, six.binary_type):
            return u'\x00' + base64.b64encode(obj).decode('ascii')
        elif type(obj) in conversions:
            return conversions[type(obj)](obj)
        else:
            return json.JSONEncoder.default(self, obj)

#
# the following is a hack. see http://bugs.python.org/issue29992
#

def _parse_string(*args, **kwargs):
    s, idx = scanstring(*args, **kwargs)
    if s and s[0] == u'\x00':
        s = base64.b64decode(s[1:])
    return s, idx


class _WAMPJsonDecoder(json.JSONDecoder):

    def __init__(self, *args, **kwargs):
        json.JSONDecoder.__init__(self, *args, **kwargs)
        self.parse_string = _parse_string

        # we need to recreate the internal scan function ..
        self.scan_once = scanner.py_make_scanner(self)

        # .. and we have to explicitly use the Py version,
        # not the C version, as the latter won't work
        # self.scan_once = scanner.make_scanner(self)


def _loads(s):
    return json.loads(s, cls=_WAMPJsonDecoder)


def _dumps(obj):
    return json.dumps(obj,
                      separators=(',', ':'),
                      ensure_ascii=False,
                      sort_keys=False,
                      cls=_WAMPJsonEncoder)

def serialize(self, obj):
    s = _dumps(obj)
    if isinstance(s, six.text_type):
      s = s.encode('utf8')
    if self._batched:
      return s + b'\30'
    else:
      return s

def unserialize(self, payload):
    if self._batched:
      chunks = payload.split(b'\30')[:-1]
    else:
      chunks = [payload]
    if len(chunks) == 0:
      raise Exception("batch format error")
    return [_loads(data.decode('utf8')) for data in chunks]
