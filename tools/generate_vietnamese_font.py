#!/usr/bin/env python3
"""Vietnamese font generator for rsvpnano.

Generates EmbeddedVietnameseSerifFont.h with glyphs covering the full
Vietnamese Unicode block (U+1EA0-U+1EF9) plus O-horn/U-horn.

Slot strategy: reuses existing custom-slot bytes (0x01-0x1F, 0x80-0xBF,
0xD7, 0xF7) and repurposes eight Latin-1 bytes not used by Vietnamese
(0xC3 Ã, 0xC4 Ä, 0xC5 Å, 0xC6 Æ, 0xC7 Ç, 0xCB Ë, 0xCE Î, 0xCF Ï).
Keeps 0x8A/0x8B (Ă/ă) and 0xBC/0xBD (Đ/đ) from the standard mapping.
"""

from __future__ import annotations

import argparse
import math
import os
import pathlib
import subprocess
import tempfile

DEFAULT_FONT_NAME = "DejaVuSans"
DEFAULT_FONT_SEARCH_PATHS = [
    str(pathlib.Path.home() / "Library" / "Fonts"),
    "/Library/Fonts",
    "/System/Library/Fonts",
    "/usr/share/fonts/truetype/noto",
]
DEFAULT_POINT_SIZE = 52
CANVAS_WIDTH = 112
CANVAS_HEIGHT = 128
ORIGIN_X = 10
BASELINE_Y = 76
ALPHA_THRESHOLD = 16
FONT_TOP_PADDING = 4
FONT_BOTTOM_PADDING = 2
DEFAULT_FIRST_CHAR = 1
DEFAULT_LAST_CHAR = 255
DEFAULT_OUTPUT_PATH = pathlib.Path("src/display/EmbeddedVietnameseSerifFont.h")
DEFAULT_SYMBOL_PREFIX = "EmbeddedVietnameseSerif"

CUSTOM_GLYPH_CODEPOINTS = {
    0x01: 0x01A0, 0x02: 0x01A1, 0x03: 0x01AF, 0x04: 0x01B0,
    0x05: 0x1EA0, 0x06: 0x1EA1, 0x07: 0x1EA2, 0x08: 0x1EA3,
    0x0E: 0x1EA4, 0x0F: 0x1EA5, 0x10: 0x1EA6, 0x11: 0x1EA7,
    0x12: 0x1EA8, 0x13: 0x1EA9, 0x14: 0x1EAA, 0x15: 0x1EAB,
    0x16: 0x1EAC, 0x17: 0x1EAD,
    0x18: 0x1EAE, 0x19: 0x1EAF, 0x1A: 0x1EB0, 0x1B: 0x1EB1,
    0x1C: 0x1EB2, 0x1D: 0x1EB3, 0x1E: 0x1EB4, 0x1F: 0x1EB5,
    0x80: 0x1EB6, 0x81: 0x1EB7, 0x82: 0x1EB8, 0x83: 0x1EB9,
    0x84: 0x1EBA, 0x85: 0x1EBB, 0x86: 0x1EBC, 0x87: 0x1EBD,
    0x88: 0x1EBE, 0x89: 0x1EBF,
    0x8A: 0x0102, 0x8B: 0x0103,
    0x8C: 0x1EC0, 0x8D: 0x1EC1, 0x8E: 0x1EC2, 0x8F: 0x1EC3,
    0x90: 0x1EC4, 0x91: 0x1EC5, 0x92: 0x1EC6, 0x93: 0x1EC7,
    0x94: 0x1EC8, 0x95: 0x1EC9, 0x96: 0x1ECA, 0x97: 0x1ECB,
    0x98: 0x1ECC, 0x99: 0x1ECD, 0x9A: 0x1ECE, 0x9B: 0x1ECF,
    0x9C: 0x1ED0, 0x9D: 0x1ED1, 0x9E: 0x1ED2, 0x9F: 0x1ED3,
    0xA1: 0x1ED4, 0xA2: 0x1ED5, 0xA3: 0x1ED6, 0xA4: 0x1ED7,
    0xA5: 0x1ED8, 0xA6: 0x1ED9,
    0xA7: 0x1EDA, 0xA8: 0x1EDB, 0xA9: 0x1EDC, 0xAA: 0x1EDD,
    0xAB: 0x1EDE, 0xAC: 0x1EDF,
    0xAE: 0x1EE0, 0xAF: 0x1EE1, 0xB0: 0x1EE2, 0xB1: 0x1EE3,
    0xB2: 0x1EE4, 0xB3: 0x1EE5, 0xB4: 0x1EE6, 0xB5: 0x1EE7,
    0xB6: 0x1EE8, 0xB7: 0x1EE9, 0xB8: 0x1EEA, 0xB9: 0x1EEB,
    0xBA: 0x1EEC, 0xBB: 0x1EED,
    0xBC: 0x0110, 0xBD: 0x0111,
    0xBE: 0x1EEE, 0xBF: 0x1EEF,
    0xC3: 0x1EF2, 0xC4: 0x1EF3, 0xC5: 0x1EF4, 0xC6: 0x1EF5,
    0xC7: 0x1EF6, 0xCB: 0x1EF7, 0xCE: 0x1EF8, 0xCF: 0x1EF9,
    0xD7: 0x1EF0, 0xF7: 0x1EF1,
}

_NAME_PAIRS = [
    (0x01A0, "Ohorn"), (0x01A1, "ohorn"), (0x01AF, "Uhorn"), (0x01B0, "uhorn"),
    (0x1EA0, "Adotbelow"), (0x1EA1, "adotbelow"), (0x1EA2, "Ahookabove"), (0x1EA3, "ahookabove"),
    (0x1EA4, "Acircumflexacute"), (0x1EA5, "acircumflexacute"),
    (0x1EA6, "Acircumflexgrave"), (0x1EA7, "acircumflexgrave"),
    (0x1EA8, "Acircumflexhook"), (0x1EA9, "acircumflexhook"),
    (0x1EAA, "Acircumflextilde"), (0x1EAB, "acircumflextilde"),
    (0x1EAC, "Acircumflexdotbelow"), (0x1EAD, "acircumflexdotbelow"),
    (0x1EAE, "Abreveacute"), (0x1EAF, "abreveacute"),
    (0x1EB0, "Abrevegrave"), (0x1EB1, "abrevegrave"),
    (0x1EB2, "Abrevehook"), (0x1EB3, "abrevehook"),
    (0x1EB4, "Abrevetilde"), (0x1EB5, "abrevetilde"),
    (0x1EB6, "Abrevedotbelow"), (0x1EB7, "abrevedotbelow"),
    (0x1EB8, "Edotbelow"), (0x1EB9, "edotbelow"),
    (0x1EBA, "Ehookabove"), (0x1EBB, "ehookabove"),
    (0x1EBC, "Etilde"), (0x1EBD, "etilde"),
    (0x1EBE, "Ecircumflexacute"), (0x1EBF, "ecircumflexacute"),
    (0x1EC0, "Ecircumflexgrave"), (0x1EC1, "ecircumflexgrave"),
    (0x1EC2, "Ecircumflexhook"), (0x1EC3, "ecircumflexhook"),
    (0x1EC4, "Ecircumflextilde"), (0x1EC5, "ecircumflextilde"),
    (0x1EC6, "Ecircumflexdotbelow"), (0x1EC7, "ecircumflexdotbelow"),
    (0x1EC8, "Ihookabove"), (0x1EC9, "ihookabove"),
    (0x1ECA, "Idotbelow"), (0x1ECB, "idotbelow"),
    (0x1ECC, "Odotbelow"), (0x1ECD, "odotbelow"),
    (0x1ECE, "Ohookabove"), (0x1ECF, "ohookabove"),
    (0x1ED0, "Ocircumflexacute"), (0x1ED1, "ocircumflexacute"),
    (0x1ED2, "Ocircumflexgrave"), (0x1ED3, "ocircumflexgrave"),
    (0x1ED4, "Ocircumflexhook"), (0x1ED5, "ocircumflexhook"),
    (0x1ED6, "Ocircumflextilde"), (0x1ED7, "ocircumflextilde"),
    (0x1ED8, "Ocircumflexdotbelow"), (0x1ED9, "ocircumflexdotbelow"),
    (0x1EDA, "Ohornacute"), (0x1EDB, "ohornacute"),
    (0x1EDC, "Ohorngrave"), (0x1EDD, "ohorngrave"),
    (0x1EDE, "Ohornhook"), (0x1EDF, "ohornhook"),
    (0x1EE0, "Ohorntilde"), (0x1EE1, "ohorntilde"),
    (0x1EE2, "Ohorndotbelow"), (0x1EE3, "ohorndotbelow"),
    (0x1EE4, "Udotbelow"), (0x1EE5, "udotbelow"),
    (0x1EE6, "Uhookabove"), (0x1EE7, "uhookabove"),
    (0x1EE8, "Uhornacute"), (0x1EE9, "uhornacute"),
    (0x1EEA, "Uhorngrave"), (0x1EEB, "uhorngrave"),
    (0x1EEC, "Uhornhook"), (0x1EED, "uhornhook"),
    (0x1EEE, "Uhorntilde"), (0x1EEF, "uhorntilde"),
    (0x1EF0, "Uhorndotbelow"), (0x1EF1, "uhorndotbelow"),
    (0x1EF2, "Ygrave"), (0x1EF3, "ygrave"),
    (0x1EF4, "Ydotbelow"), (0x1EF5, "ydotbelow"),
    (0x1EF6, "Yhookabove"), (0x1EF7, "yhookabove"),
    (0x1EF8, "Ytilde"), (0x1EF9, "ytilde"),
    (0x0102, "Abreve"), (0x0103, "abreve"),
    (0x0110, "Dcroat"), (0x0111, "dcroat"),
]
CUSTOM_GLYPH_NAMES = dict(_NAME_PAIRS)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate Vietnamese embedded font header.")
    parser.add_argument("--point-size", type=int, default=DEFAULT_POINT_SIZE)
    parser.add_argument("--font-name", default=DEFAULT_FONT_NAME)
    parser.add_argument("--output", type=pathlib.Path, default=DEFAULT_OUTPUT_PATH)
    parser.add_argument("--symbol-prefix", default=DEFAULT_SYMBOL_PREFIX)
    parser.add_argument("--font-search-path", action="append", default=[])
    parser.add_argument("--first-char", type=int, default=DEFAULT_FIRST_CHAR)
    parser.add_argument("--last-char", type=int, default=DEFAULT_LAST_CHAR)
    return parser.parse_args()


def escape_postscript_char(ch: str) -> str:
    if ch in ("\\", "(", ")"):
        return "\\" + ch
    code = ord(ch)
    if code < 32 or code > 126:
        return f"\\{code:03o}"
    return ch


def latin1_font_setup(font_name: str, point_size: int) -> str:
    return (
        f"/CodexLatin1Font /{font_name} findfont dup length dict begin "
        "{1 index /FID ne {def} {pop pop} ifelse} forall "
        "/Encoding ISOLatin1Encoding def "
        "currentdict end definefont pop "
        f"/CodexLatin1Font findfont {point_size} scalefont setfont "
    )


def glyph_name_for_codepoint(codepoint: int) -> str:
    return CUSTOM_GLYPH_NAMES.get(codepoint, f"uni{codepoint:04X}")


def glyph_script_for_codepoint(codepoint: int) -> str:
    if codepoint <= 0xFF:
        escaped = escape_postscript_char(chr(codepoint))
        return f"({escaped}) show"
    return f"/{glyph_name_for_codepoint(codepoint)} glyphshow"


def display_codepoint_for_slot(slot: int) -> int:
    return CUSTOM_GLYPH_CODEPOINTS.get(slot, slot)


def glyph_comment_for_slot(slot: int) -> str:
    mapped_codepoint = CUSTOM_GLYPH_CODEPOINTS.get(slot)
    if mapped_codepoint is None:
        return ascii(chr(slot)) if 32 <= slot <= 126 else f"0x{slot:02X}"
    return f"slot 0x{slot:02X} -> U+{mapped_codepoint:04X}"


def render_glyph(tmp_dir, slot, font_name, point_size, font_search_paths):
    output = tmp_dir / f"{slot:03d}.pgm"
    codepoint = display_codepoint_for_slot(slot)
    program = (
        "1 setgray clippath fill "
        "0 setgray "
        f"{latin1_font_setup(font_name, point_size)}"
        f"{ORIGIN_X} {BASELINE_Y} moveto "
        f"{glyph_script_for_codepoint(codepoint)} showpage"
    )
    command = [
        "gs", "-q", "-dNOPAUSE", "-dBATCH",
        "-dTextAlphaBits=4", "-dGraphicsAlphaBits=4",
        "-sDEVICE=pgmraw", "-r72",
        f"-g{CANVAS_WIDTH}x{CANVAS_HEIGHT}",
        f"-sOutputFile={output}",
    ]
    existing_paths = [p for p in font_search_paths if pathlib.Path(p).is_dir()]
    if existing_paths:
        command.append(f"-sFONTPATH={os.pathsep.join(existing_paths)}")
    command += ["-c", program]
    subprocess.run(command, check=True, capture_output=True, text=True)
    return output


def advance_width_for_glyph(slot, font_name, point_size, font_search_paths):
    codepoint = display_codepoint_for_slot(slot)
    command = ["gs", "-q", "-dNODISPLAY"]
    existing_paths = [p for p in font_search_paths if pathlib.Path(p).is_dir()]
    if existing_paths:
        command.append(f"-sFONTPATH={os.pathsep.join(existing_paths)}")
    command += ["-c", (
        f"{latin1_font_setup(font_name, point_size)}"
        "0 0 moveto "
        f"{glyph_script_for_codepoint(codepoint)} "
        "currentpoint pop == quit"
    )]
    result = subprocess.run(command, check=True, capture_output=True, text=True)
    lines = [l.strip() for l in result.stdout.splitlines() if l.strip()]
    if not lines:
        raise RuntimeError(f"Failed advance width for slot 0x{slot:02X}")
    return max(1, int(math.floor(float(lines[-1]) + 0.5)))


def parse_pgm(path):
    data = path.read_bytes()
    if not data.startswith(b"P5\n"):
        raise ValueError(f"Unexpected PGM header in {path}")
    parts = data.split(b"\n")
    index = 1
    while parts[index].startswith(b"#"):
        index += 1
    width, height = map(int, parts[index].split())
    max_value = int(parts[index + 1])
    if max_value != 255:
        raise ValueError(f"Unexpected max value {max_value} in {path}")
    raster = b"\n".join(parts[index + 2:])
    expected_length = width * height
    if len(raster) != expected_length:
        raise ValueError(f"Unexpected raster length in {path}: {len(raster)} != {expected_length}")
    return width, height, raster


def alpha_at(raster, width, x, y):
    return 255 - raster[y * width + x]


def main():
    args = parse_args()
    font_search_paths = list(DEFAULT_FONT_SEARCH_PATHS) + args.font_search_path
    glyph_images = {}
    global_top = CANVAS_HEIGHT
    global_bottom = -1

    total = args.last_char - args.first_char + 1
    with tempfile.TemporaryDirectory(prefix="viet_font_") as tmp:
        tmp_dir = pathlib.Path(tmp)
        for i, code in enumerate(range(args.first_char, args.last_char + 1)):
            if (i + 1) % 32 == 0 or i == 0:
                print(f"  Rendering glyph {i+1}/{total} (slot 0x{code:02X})...")
            pgm_path = render_glyph(tmp_dir, code, args.font_name, args.point_size, font_search_paths)
            width, height, raster = parse_pgm(pgm_path)
            glyph_images[code] = (width, height, raster)
            for y in range(height):
                found = False
                for x in range(width):
                    if alpha_at(raster, width, x, y) > ALPHA_THRESHOLD:
                        global_top = min(global_top, y)
                        global_bottom = max(global_bottom, y)
                        found = True
                if found:
                    continue

    if global_bottom < global_top:
        raise RuntimeError("Failed to detect any font pixels")

    crop_top = max(0, global_top - FONT_TOP_PADDING)
    crop_bottom = min(CANVAS_HEIGHT - 1, global_bottom + FONT_BOTTOM_PADDING)
    font_height = crop_bottom - crop_top + 1
    bitmap_bytes = []
    glyph_entries = []

    for code in range(args.first_char, args.last_char + 1):
        width, height, raster = glyph_images[code]
        min_x = width
        max_x = -1
        for y in range(crop_top, crop_bottom + 1):
            for x in range(width):
                if alpha_at(raster, width, x, y) > ALPHA_THRESHOLD:
                    min_x = min(min_x, x)
                    max_x = max(max_x, x)

        bitmap_offset = len(bitmap_bytes)
        if max_x >= min_x:
            glyph_width = max_x - min_x + 1
            for y in range(crop_top, crop_bottom + 1):
                for x in range(min_x, max_x + 1):
                    alpha = alpha_at(raster, width, x, y)
                    if alpha <= ALPHA_THRESHOLD:
                        alpha = 0
                    bitmap_bytes.append(alpha)
            x_offset = min_x - ORIGIN_X
            x_advance = advance_width_for_glyph(code, args.font_name, args.point_size, font_search_paths)
        else:
            x_offset = 0
            glyph_width = 0
            x_advance = advance_width_for_glyph(code, args.font_name, args.point_size, font_search_paths)

        glyph_entries.append(
            "    " + "{" + f"{bitmap_offset}, {x_offset}, {glyph_width}, {x_advance}" + "}, "
            + f"// {glyph_comment_for_slot(code)}"
        )

    lines = [
        "#pragma once",
        "",
        "#include <Arduino.h>",
        "",
        "// Vietnamese font: NotoSans with full U+1EA0-U+1EF9 block plus O-horn/U-horn.",
        "// Slot mapping defined in src/text/VietnameseText.h — keep in sync.",
        f"// Source font: {args.font_name} at {args.point_size} pt",
        "",
        f"struct {args.symbol_prefix}Glyph " + "{",
        "  uint32_t bitmapOffset;",
        "  int8_t xOffset;",
        "  uint8_t width;",
        "  uint8_t xAdvance;",
        "};",
        "",
        f"constexpr uint8_t k{args.symbol_prefix}FirstChar = {args.first_char};",
        f"constexpr uint8_t k{args.symbol_prefix}LastChar = {args.last_char};",
        f"constexpr uint8_t k{args.symbol_prefix}Height = {font_height};",
        "",
        f"static const uint8_t k{args.symbol_prefix}Bitmaps[] PROGMEM = " + "{",
    ]
    for offset in range(0, len(bitmap_bytes), 16):
        chunk = bitmap_bytes[offset:offset + 16]
        lines.append("    " + ", ".join(f"{v:3d}" for v in chunk) + ",")
    lines += [
        "};",
        "",
        f"static const {args.symbol_prefix}Glyph k{args.symbol_prefix}Glyphs[] PROGMEM = " + "{",
        *glyph_entries,
        "};",
        "",
    ]

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"Written {args.output}  ({len(bitmap_bytes)} bitmap bytes, {total} glyphs, height={font_height})")


if __name__ == "__main__":
    main()
