#!/usr/bin/env python3
"""Decode, validate and safely stage the derived Fire Black roster payload.

The repository contains only extracted/derived data, never the commercial ROM.
By default this validates the payload and reports its schema. Pass an output path
to materialize a CSV suitable for import tooling.

The custom Charmander line is always excluded from staged bulk imports. This is
also enforced by the canonical Gen III species slots (4, 5 and 6), so the guard
does not depend on the extracted table containing species names.
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
CUSTOM_LINE_SLOTS = {4, 5, 6}

# Known semantic translations discovered while auditing Fire Black. Raw type IDs
# must never be written directly into pokeemerald/pokefirered C tables.
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


def protected_row_indices(rows):
    """Return 1-based species slots that must never be bulk imported."""
    protected = set(CUSTOM_LINE_SLOTS)
    header = rows[0]
    name_col = find_column(header, "name", "species", "species_name", "pokemon")
    if name_col is not None:
        for slot, row in enumerate(rows[1:], 1):
            name = row[name_col].strip().upper().replace("é", "E")
            if name in CUSTOM_LINE:
                protected.add(slot)
    return protected


def write_staged_csv(rows, output):
    """Write derived data with protected custom-line rows removed.

    A source_slot column is prepended so later generators can map every row back
    to its exact FireRed species index without relying on row order after the
    exclusions. This makes subsequent stats/types/abilities imports auditable.
    """
    protected = protected_row_indices(rows)
    with output.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(["source_slot", *rows[0]])
        for slot, row in enumerate(rows[1:], 1):
            if slot not in protected:
                writer.writerow([slot, *row])
    return protected


def main():
    _, rows = decode_roster()
    protected = protected_row_indices(rows)
    print(f"Decoded Fire Black roster: {len(rows) - 1} data rows, {len(rows[0])} columns")
    print("Columns: " + ", ".join(rows[0]))
    print("Raw type translations required: " + ", ".join(
        f"{raw_id}={name}" for raw_id, name in sorted(KNOWN_FIREBLACK_TYPE_IDS.items())
    ))
    print("Protected custom-line species slots: " + ", ".join(map(str, sorted(protected))))

    if len(sys.argv) > 2:
        raise SystemExit(f"usage: {pathlib.Path(sys.argv[0]).name} [staged-output.csv]")
    if len(sys.argv) == 2:
        output = pathlib.Path(sys.argv[1])
        output.parent.mkdir(parents=True, exist_ok=True)
        excluded = write_staged_csv(rows, output)
        print(
            f"Wrote {output}: {len(rows) - 1 - len(excluded)} importable rows; "
            f"excluded slots {', '.join(map(str, sorted(excluded)))}"
        )


if __name__ == "__main__":
    main()
