#!/usr/bin/env python3
import base64
import zlib
from roster_data_1 import DATA

raw = base64.b85decode(DATA.encode("ascii"))
text = zlib.decompress(raw).decode("utf-8")
print(text)
