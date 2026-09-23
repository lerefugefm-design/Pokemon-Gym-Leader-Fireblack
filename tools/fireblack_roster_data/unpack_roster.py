#!/usr/bin/env python3
"""Decode and validate the derived Fire Black roster payload.

The repository contains only extracted/derived data, never the commercial ROM.
By default this validates the payload and reports its schema. Pass an output path
to materialize the decoded CSV for import tooling or local inspection.

This validator deliberately protects the custom Charmander line: future importers
must exclude Charmander, Charmeleon and Charizard from bulk Fire Black imports.
"""
import base64
import csv
import io
import pathlib
import sys
import zlib

from roster_data_1 import DATA

EXPECTED_SCANNED_ROWS = 411
CUSTOM_LINE = {"CHARMANDER", "CHARMELEON", "CHARIZARD"}

# Fire Black's raw type numbering is not the stock FireRed numbering. Keep raw
# IDs out of generated C data until they have been translated semantically.
KNOWN_FIREBLACK_TYPE_IDS = {
    17: "FAIRY",
    23: "DARK",
}


def decode_roster():
    raw = base64.b85decode(DATA.encode("ascii"))
    text = zlib.decompress(raw).decode("utf-8")
    rows = list(csv.reader(io.StringIO(text)))
    if len(rows) < 2:
        raise SystemExit("decoded roster is empty")
    width = len(rows[0])
    if not width or any(len(row) != width for row in rows):
        raise SystemExit("decoded roster has inconsistent CSV rows")
    if len(rows) - 1 != EXPECTED_SCANNED_ROWS:
        raise SystemExit(
            f"unexpected Fire Black roster size: {len(rows) - 1}; "
            f"expected {EXPECTED_SCANNED_ROWS} scanned species slots"
        )
    if len(set(rows[0])) != width:
        raise SystemExit("decoded roster contains duplicate column names")
    return text, rows


def find_column(header, *candidates):
    normalized = {name.strip().lower(): i for i, name in enumerate(header)}
    for candidate in candidates:
        if candidate in normalized:
            return normalized[candidate]
    return None


def validate_import_guards(rows):
    header = rows[0]
    name_col = find_column(header, "name", "species", "species_name", "pokemon")
    protected = []
    if name_col is not None:
        for row in rows[1:]:
            name = row[name_col].strip().upper().replace("é", "E")
            if name in CUSTOM_LINE:
                protected.append(name)
    return sorted(set(protected))


def main():
    text, rows = decode_roster()
    protected = validate_import_guards(rows)
    print(f"Decoded Fire Black roster: {len(rows) - 1} data rows, {len(rows[0])} columns")
    print("Columns: " + ", ".join(rows[0]))
    print("Raw type translations required: " + ", ".join(
        f"{raw_id}={name}" for raw_id, name in sorted(KNOWN_FIREBLACK_TYPE_IDS.items())
    ))
    if protected:
        print("Protected custom line (exclude from bulk import): " + ", ".join(protected))
    else:
        print("Protected custom line: no named rows detected; importer must still exclude species IDs")

    if len(sys.argv) > 2:
        raise SystemExit(f"usage: {pathlib.Path(sys.argv[0]).name} [output.csv]")
    if len(sys.argv) == 2:
        output = pathlib.Path(sys.argv[1])
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(text, encoding="utf-8")
        print(f"Wrote {output}")


if __name__ == "__main__":
    main()
