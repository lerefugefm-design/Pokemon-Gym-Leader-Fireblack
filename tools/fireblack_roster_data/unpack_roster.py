#!/usr/bin/env python3
"""Decode the derived Fire Black roster payload.

The repository contains only extracted/derived data, never the commercial ROM.
By default this validates the payload and reports its row count. Pass an output
path to materialize the decoded CSV for import tooling or local inspection.
"""
import base64
import csv
import io
import pathlib
import sys
import zlib

from roster_data_1 import DATA


def decode_roster():
    raw = base64.b85decode(DATA.encode("ascii"))
    text = zlib.decompress(raw).decode("utf-8")
    rows = list(csv.reader(io.StringIO(text)))
    if len(rows) < 2:
        raise SystemExit("decoded roster is empty")
    width = len(rows[0])
    if not width or any(len(row) != width for row in rows):
        raise SystemExit("decoded roster has inconsistent CSV rows")
    return text, rows


def main():
    text, rows = decode_roster()
    print(f"Decoded Fire Black roster: {len(rows) - 1} data rows, {len(rows[0])} columns")
    print("Columns: " + ", ".join(rows[0]))
    if len(sys.argv) > 2:
        raise SystemExit(f"usage: {pathlib.Path(sys.argv[0]).name} [output.csv]")
    if len(sys.argv) == 2:
        output = pathlib.Path(sys.argv[1])
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(text, encoding="utf-8")
        print(f"Wrote {output}")


if __name__ == "__main__":
    main()
