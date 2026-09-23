#!/usr/bin/env python3
import base64
import zlib
from roster_data_1 import DATA

raw = base64.b85decode(DATA.encode("ascii"))
text = zlib.decompress(raw).decode("utf-8")
rows = text.splitlines()\nprint(f"Decoded Fire Black roster: {len(rows) - 1} data rows")\nif len(rows) < 2:\n    raise SystemExit("decoded roster is empty")
