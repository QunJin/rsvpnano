#pragma once

#include <Arduino.h>
#include <stdint.h>

// Vietnamese slot mapping — mirrors tools/generate_vietnamese_font.py
// CUSTOM_GLYPH_CODEPOINTS exactly. Keep these two files in sync.
//
// Covers the full Vietnamese Unicode block (U+1EA0-U+1EF9) plus
// O-horn/U-horn (U+01A0/01A1, U+01AF/01B0). Reuses the same custom
// slot range as LatinText.h (0x01-0x1F, 0x80-0xBF, 0xD7, 0xF7) plus
// eight repurposed Latin-1 bytes not used in Vietnamese:
// 0xC3 (was Ã), 0xC4 (was Ä), 0xC5 (was Å), 0xC6 (was Æ),
// 0xC7 (was Ç), 0xCB (was Ë), 0xCE (was Î), 0xCF (was Ï).
//
// Ă/ă (U+0102/0103) and Đ/đ (U+0110/0111) keep the same slots
// (0x8A/0x8B and 0xBC/0xBD) as the standard LatinText.h mapping,
// so those two letters render identically in both modes.

namespace VietnameseText {

inline bool customSlotForCodepoint(uint32_t codepoint, uint8_t &slot) {
  switch (codepoint) {
    case 0x01A0: slot = 0x01; return true;  // Ơ
    case 0x01A1: slot = 0x02; return true;  // ơ
    case 0x01AF: slot = 0x03; return true;  // Ư
    case 0x01B0: slot = 0x04; return true;  // ư
    case 0x1EA0: slot = 0x05; return true;  // Ạ
    case 0x1EA1: slot = 0x06; return true;  // ạ
    case 0x1EA2: slot = 0x07; return true;  // Ả
    case 0x1EA3: slot = 0x08; return true;  // ả
    case 0x1EA4: slot = 0x0E; return true;  // Ấ
    case 0x1EA5: slot = 0x0F; return true;  // ấ
    case 0x1EA6: slot = 0x10; return true;  // Ầ
    case 0x1EA7: slot = 0x11; return true;  // ầ
    case 0x1EA8: slot = 0x12; return true;  // Ẩ
    case 0x1EA9: slot = 0x13; return true;  // ẩ
    case 0x1EAA: slot = 0x14; return true;  // Ẫ
    case 0x1EAB: slot = 0x15; return true;  // ẫ
    case 0x1EAC: slot = 0x16; return true;  // Ậ
    case 0x1EAD: slot = 0x17; return true;  // ậ
    case 0x1EAE: slot = 0x18; return true;  // Ắ
    case 0x1EAF: slot = 0x19; return true;  // ắ
    case 0x1EB0: slot = 0x1A; return true;  // Ằ
    case 0x1EB1: slot = 0x1B; return true;  // ằ
    case 0x1EB2: slot = 0x1C; return true;  // Ẳ
    case 0x1EB3: slot = 0x1D; return true;  // ẳ
    case 0x1EB4: slot = 0x1E; return true;  // Ẵ
    case 0x1EB5: slot = 0x1F; return true;  // ẵ
    case 0x1EB6: slot = 0x80; return true;  // Ặ
    case 0x1EB7: slot = 0x81; return true;  // ặ
    case 0x1EB8: slot = 0x82; return true;  // Ẹ
    case 0x1EB9: slot = 0x83; return true;  // ẹ
    case 0x1EBA: slot = 0x84; return true;  // Ẻ
    case 0x1EBB: slot = 0x85; return true;  // ẻ
    case 0x1EBC: slot = 0x86; return true;  // Ẽ
    case 0x1EBD: slot = 0x87; return true;  // ẽ
    case 0x1EBE: slot = 0x88; return true;  // Ế
    case 0x1EBF: slot = 0x89; return true;  // ế
    case 0x0102: slot = 0x8A; return true;  // Ă (shared with LatinText)
    case 0x0103: slot = 0x8B; return true;  // ă (shared with LatinText)
    case 0x1EC0: slot = 0x8C; return true;  // Ề
    case 0x1EC1: slot = 0x8D; return true;  // ề
    case 0x1EC2: slot = 0x8E; return true;  // Ể
    case 0x1EC3: slot = 0x8F; return true;  // ể
    case 0x1EC4: slot = 0x90; return true;  // Ễ
    case 0x1EC5: slot = 0x91; return true;  // ễ
    case 0x1EC6: slot = 0x92; return true;  // Ệ
    case 0x1EC7: slot = 0x93; return true;  // ệ
    case 0x1EC8: slot = 0x94; return true;  // Ỉ
    case 0x1EC9: slot = 0x95; return true;  // ỉ
    case 0x1ECA: slot = 0x96; return true;  // Ị
    case 0x1ECB: slot = 0x97; return true;  // ị
    case 0x1ECC: slot = 0x98; return true;  // Ọ
    case 0x1ECD: slot = 0x99; return true;  // ọ
    case 0x1ECE: slot = 0x9A; return true;  // Ỏ
    case 0x1ECF: slot = 0x9B; return true;  // ỏ
    case 0x1ED0: slot = 0x9C; return true;  // Ố
    case 0x1ED1: slot = 0x9D; return true;  // ố
    case 0x1ED2: slot = 0x9E; return true;  // Ồ
    case 0x1ED3: slot = 0x9F; return true;  // ồ
    case 0x1ED4: slot = 0xA1; return true;  // Ổ
    case 0x1ED5: slot = 0xA2; return true;  // ổ
    case 0x1ED6: slot = 0xA3; return true;  // Ỗ
    case 0x1ED7: slot = 0xA4; return true;  // ỗ
    case 0x1ED8: slot = 0xA5; return true;  // Ộ
    case 0x1ED9: slot = 0xA6; return true;  // ộ
    case 0x1EDA: slot = 0xA7; return true;  // Ớ
    case 0x1EDB: slot = 0xA8; return true;  // ớ
    case 0x1EDC: slot = 0xA9; return true;  // Ờ
    case 0x1EDD: slot = 0xAA; return true;  // ờ
    case 0x1EDE: slot = 0xAB; return true;  // Ở
    case 0x1EDF: slot = 0xAC; return true;  // ở
    case 0x1EE0: slot = 0xAE; return true;  // Ỡ
    case 0x1EE1: slot = 0xAF; return true;  // ỡ
    case 0x1EE2: slot = 0xB0; return true;  // Ợ
    case 0x1EE3: slot = 0xB1; return true;  // ợ
    case 0x1EE4: slot = 0xB2; return true;  // Ụ
    case 0x1EE5: slot = 0xB3; return true;  // ụ
    case 0x1EE6: slot = 0xB4; return true;  // Ủ
    case 0x1EE7: slot = 0xB5; return true;  // ủ
    case 0x1EE8: slot = 0xB6; return true;  // Ứ
    case 0x1EE9: slot = 0xB7; return true;  // ứ
    case 0x1EEA: slot = 0xB8; return true;  // Ừ
    case 0x1EEB: slot = 0xB9; return true;  // ừ
    case 0x1EEC: slot = 0xBA; return true;  // Ử
    case 0x1EED: slot = 0xBB; return true;  // ử
    case 0x0110: slot = 0xBC; return true;  // Đ (shared with LatinText)
    case 0x0111: slot = 0xBD; return true;  // đ (shared with LatinText)
    case 0x1EEE: slot = 0xBE; return true;  // Ữ
    case 0x1EEF: slot = 0xBF; return true;  // ữ
    case 0x1EF2: slot = 0xC3; return true;  // Ỳ (repurposed from Ã)
    case 0x1EF3: slot = 0xC4; return true;  // ỳ (repurposed from Ä)
    case 0x1EF4: slot = 0xC5; return true;  // Ỵ (repurposed from Å)
    case 0x1EF5: slot = 0xC6; return true;  // ỵ (repurposed from Æ)
    case 0x1EF6: slot = 0xC7; return true;  // Ỷ (repurposed from Ç)
    case 0x1EF7: slot = 0xCB; return true;  // ỷ (repurposed from Ë)
    case 0x1EF8: slot = 0xCE; return true;  // Ỹ (repurposed from Î)
    case 0x1EF9: slot = 0xCF; return true;  // ỹ (repurposed from Ï)
    case 0x1EF0: slot = 0xD7; return true;  // Ự
    case 0x1EF1: slot = 0xF7; return true;  // ự
    default:
      return false;
  }
}

// Uppercase -> lowercase slot pairs, for the Vietnamese-specific slots only.
// Ă/ă, Đ/đ are already handled by LatinText::customLowercaseByte since they
// share the same slot bytes.
inline bool customLowercaseByte(uint8_t value, uint8_t &lowercase) {
  switch (value) {
    case 0x01: lowercase = 0x02; return true;  // Ơ -> ơ
    case 0x03: lowercase = 0x04; return true;  // Ư -> ư
    case 0x05: lowercase = 0x06; return true;  // Ạ -> ạ
    case 0x07: lowercase = 0x08; return true;  // Ả -> ả
    case 0x0E: lowercase = 0x0F; return true;  // Ấ -> ấ
    case 0x10: lowercase = 0x11; return true;  // Ầ -> ầ
    case 0x12: lowercase = 0x13; return true;  // Ẩ -> ẩ
    case 0x14: lowercase = 0x15; return true;  // Ẫ -> ẫ
    case 0x16: lowercase = 0x17; return true;  // Ậ -> ậ
    case 0x18: lowercase = 0x19; return true;  // Ắ -> ắ
    case 0x1A: lowercase = 0x1B; return true;  // Ằ -> ằ
    case 0x1C: lowercase = 0x1D; return true;  // Ẳ -> ẳ
    case 0x1E: lowercase = 0x1F; return true;  // Ẵ -> ẵ
    case 0x80: lowercase = 0x81; return true;  // Ặ -> ặ
    case 0x82: lowercase = 0x83; return true;  // Ẹ -> ẹ
    case 0x84: lowercase = 0x85; return true;  // Ẻ -> ẻ
    case 0x86: lowercase = 0x87; return true;  // Ẽ -> ẽ
    case 0x88: lowercase = 0x89; return true;  // Ế -> ế
    case 0x8C: lowercase = 0x8D; return true;  // Ề -> ề
    case 0x8E: lowercase = 0x8F; return true;  // Ể -> ể
    case 0x90: lowercase = 0x91; return true;  // Ễ -> ễ
    case 0x92: lowercase = 0x93; return true;  // Ệ -> ệ
    case 0x94: lowercase = 0x95; return true;  // Ỉ -> ỉ
    case 0x96: lowercase = 0x97; return true;  // Ị -> ị
    case 0x98: lowercase = 0x99; return true;  // Ọ -> ọ
    case 0x9A: lowercase = 0x9B; return true;  // Ỏ -> ỏ
    case 0x9C: lowercase = 0x9D; return true;  // Ố -> ố
    case 0x9E: lowercase = 0x9F; return true;  // Ồ -> ồ
    case 0xA1: lowercase = 0xA2; return true;  // Ổ -> ổ
    case 0xA3: lowercase = 0xA4; return true;  // Ỗ -> ỗ
    case 0xA5: lowercase = 0xA6; return true;  // Ộ -> ộ
    case 0xA7: lowercase = 0xA8; return true;  // Ớ -> ớ
    case 0xA9: lowercase = 0xAA; return true;  // Ờ -> ờ
    case 0xAB: lowercase = 0xAC; return true;  // Ở -> ở
    case 0xAE: lowercase = 0xAF; return true;  // Ỡ -> ỡ
    case 0xB0: lowercase = 0xB1; return true;  // Ợ -> ợ
    case 0xB2: lowercase = 0xB3; return true;  // Ụ -> ụ
    case 0xB4: lowercase = 0xB5; return true;  // Ủ -> ủ
    case 0xB6: lowercase = 0xB7; return true;  // Ứ -> ứ
    case 0xB8: lowercase = 0xB9; return true;  // Ừ -> ừ
    case 0xBA: lowercase = 0xBB; return true;  // Ử -> ử
    case 0xBE: lowercase = 0xBF; return true;  // Ữ -> ữ
    case 0xC3: lowercase = 0xC4; return true;  // Ỳ -> ỳ
    case 0xC5: lowercase = 0xC6; return true;  // Ỵ -> ỵ
    case 0xC7: lowercase = 0xCB; return true;  // Ỷ -> ỷ
    case 0xCE: lowercase = 0xCF; return true;  // Ỹ -> ỹ
    case 0xD7: lowercase = 0xF7; return true;  // Ự -> ự
    default:
      return false;
  }
}

inline bool isCustomUppercaseLetter(uint8_t value) {
  uint8_t lowered = 0;
  return customLowercaseByte(value, lowered);
}

inline bool isCustomLowercaseLetter(uint8_t value) {
  switch (value) {
    case 0x02: case 0x04: case 0x06: case 0x08: case 0x0F: case 0x11:
    case 0x13: case 0x15: case 0x17: case 0x19: case 0x1B: case 0x1D:
    case 0x1F: case 0x81: case 0x83: case 0x85: case 0x87: case 0x89:
    case 0x8D: case 0x8F: case 0x91: case 0x93: case 0x95: case 0x97:
    case 0x99: case 0x9B: case 0x9D: case 0x9F: case 0xA2: case 0xA4:
    case 0xA6: case 0xA8: case 0xAA: case 0xAC: case 0xAF: case 0xB1:
    case 0xB3: case 0xB5: case 0xB7: case 0xB9: case 0xBB: case 0xBF:
    case 0xC4: case 0xC6: case 0xCB: case 0xCF: case 0xF7:
      return true;
    default:
      return false;
  }
}

// Vowel slots (used by ReadingLoop / pacing heuristics that check for
// vowels to decide sentence/word timing). Includes every Vietnamese
// vowel-with-diacritic slot, upper and lower case.
inline bool isVowel(uint8_t value) {
  switch (value) {
    case 0x01: case 0x02: case 0x03: case 0x04:  // Ơơ Ưư
    case 0x05: case 0x06: case 0x07: case 0x08:  // Ạạ Ảả
    case 0x0E: case 0x0F: case 0x10: case 0x11:
    case 0x12: case 0x13: case 0x14: case 0x15:
    case 0x16: case 0x17: case 0x18: case 0x19:
    case 0x1A: case 0x1B: case 0x1C: case 0x1D:
    case 0x1E: case 0x1F:
    case 0x80: case 0x81: case 0x82: case 0x83:
    case 0x84: case 0x85: case 0x86: case 0x87:
    case 0x88: case 0x89: case 0x8C: case 0x8D:
    case 0x8E: case 0x8F: case 0x90: case 0x91:
    case 0x92: case 0x93: case 0x94: case 0x95:
    case 0x96: case 0x97: case 0x98: case 0x99:
    case 0x9A: case 0x9B: case 0x9C: case 0x9D:
    case 0x9E: case 0x9F:
    case 0xA1: case 0xA2: case 0xA3: case 0xA4:
    case 0xA5: case 0xA6: case 0xA7: case 0xA8:
    case 0xA9: case 0xAA: case 0xAB: case 0xAC:
    case 0xAE: case 0xAF: case 0xB0: case 0xB1:
    case 0xB2: case 0xB3: case 0xB4: case 0xB5:
    case 0xB6: case 0xB7: case 0xB8: case 0xB9:
    case 0xBA: case 0xBB: case 0xBE: case 0xBF:
    case 0xC3: case 0xC4: case 0xC5: case 0xC6:
    case 0xC7: case 0xCB: case 0xCE: case 0xCF:
    case 0xD7: case 0xF7:
      return true;
    default:
      return false;
  }
}

// Fallback ASCII byte for a Vietnamese slot, used only if the Vietnamese
// font glyph somehow can't render (should not normally happen since the
// Vietnamese font covers every slot in this table).
inline uint8_t fallbackAsciiByte(uint8_t value) {
  switch (value) {
    case 0x01: case 0x03: return 'U';  // Ơ Ư (rough approx)
    case 0x02: case 0x04: return 'u';
    case 0x05: case 0x07: case 0x0E: case 0x10: case 0x12: case 0x14:
    case 0x16: case 0x18: case 0x1A: case 0x1C: case 0x1E:
      return 'A';
    case 0x06: case 0x08: case 0x0F: case 0x11: case 0x13: case 0x15:
    case 0x17: case 0x19: case 0x1B: case 0x1D: case 0x1F:
      return 'a';
    case 0x80: return 'A';
    case 0x81: return 'a';
    case 0x82: case 0x84: case 0x86: case 0x88: case 0x8C: case 0x8E:
    case 0x90: case 0x92:
      return 'E';
    case 0x83: case 0x85: case 0x87: case 0x89: case 0x8D: case 0x8F:
    case 0x91: case 0x93:
      return 'e';
    case 0x94: case 0x96:
      return 'I';
    case 0x95: case 0x97:
      return 'i';
    case 0x98: case 0x9A: case 0x9C: case 0x9E: case 0xA1: case 0xA3:
    case 0xA5: case 0xA7: case 0xA9: case 0xAB: case 0xAE: case 0xB0:
      return 'O';
    case 0x99: case 0x9B: case 0x9D: case 0x9F: case 0xA2: case 0xA4:
    case 0xA6: case 0xA8: case 0xAA: case 0xAC: case 0xAF: case 0xB1:
      return 'o';
    case 0xB2: case 0xB4: case 0xB6: case 0xB8: case 0xBA: case 0xBE:
      return 'U';
    case 0xB3: case 0xB5: case 0xB7: case 0xB9: case 0xBB: case 0xBF:
      return 'u';
    case 0xC3: case 0xC5: case 0xC7: case 0xCE:
      return 'Y';
    case 0xC4: case 0xC6: case 0xCB: case 0xCF:
      return 'y';
    case 0xD7:
      return 'U';
    case 0xF7:
      return 'u';
    default:
      return static_cast<uint8_t>('?');
  }
}

}  // namespace VietnameseText
