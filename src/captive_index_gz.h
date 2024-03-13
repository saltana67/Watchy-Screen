#pragma once

#include <Arduino.h>


namespace Watchy_WebServer {

const uint8_t INDEX_GZ[] PROGMEM = {
    0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xdd, 0x59, 0xeb, 0x8f, 0xe3, 0xc6, 0x91, 0xff, 0xee,
    0xbf, 0xa2, 0x42, 0x7b, 0x33, 0x52, 0x3c, 0x5d, 0x62, 0xbf, 0xf8, 0x90, 0x44, 0x19, 0xeb, 0xcd, 0x06, 0x31, 0x60,
    0x27, 0x06, 0x76, 0xe3, 0xfb, 0x60, 0x18, 0x58, 0x0e, 0xd9, 0x92, 0xda, 0xc3, 0x87, 0x42, 0xb6, 0xe6, 0x09, 0xdd,
    0xdf, 0x7e, 0xa8, 0x26, 0xa5, 0xd1, 0x6c, 0x76, 0x82, 0x6c, 0x70, 0x87, 0x4b, 0xb2, 0x8b, 0xe1, 0x14, 0xbb, 0xab,
    0xaa, 0xab, 0x7e, 0x5d, 0x55, 0xcd, 0xa9, 0x5e, 0xfe, 0xa6, 0x6c, 0x0b, 0x77, 0xbf, 0x33, 0xb0, 0x75, 0x75, 0xb5,
    0x5a, 0xd2, 0x13, 0xaa, 0xbc, 0xd9, 0x64, 0xa6, 0x59, 0x2d, 0xb7, 0x26, 0x2f, 0x57, 0xcb, 0xda, 0xb8, 0x1c, 0x8a,
    0x6d, 0xde, 0xf5, 0xc6, 0x65, 0x7f, 0x79, 0xff, 0x07, 0x96, 0xc0, 0x6c, 0xb5, 0xac, 0x6c, 0x73, 0x0d, 0x9d, 0xa9,
    0x32, 0x5b, 0xb4, 0x0d, 0x6c, 0x3b, 0xb3, 0xce, 0xca, 0xdc, 0xe5, 0x73, 0x5b, 0xe7, 0x1b, 0x43, 0x0c, 0x5e, 0xac,
    0xc9, 0x6b, 0x93, 0xdd, 0x58, 0x73, 0xbb, 0x6b, 0x3b, 0x07, 0x45, 0xdb, 0x38, 0xd3, 0xb8, 0x2c, 0xb8, 0xb5, 0xa5,
    0xdb, 0x66, 0xa5, 0xb9, 0xb1, 0x85, 0x61, 0xfe, 0xe5, 0xd2, 0x36, 0xd6, 0xd9, 0xbc, 0x62, 0x7d, 0x91, 0x57, 0x26,
    0xe3, 0x97, 0xfb, 0xde, 0x74, 0xfe, 0x25, 0xbf, 0xaa, 0x4c, 0xd6, 0xb4, 0xc1, 0x6c, 0xb5, 0xec, 0x8b, 0xce, 0xee,
    0x1c, 0x90, 0xbd, 0x59, 0xdd, 0x96, 0xfb, 0xca, 0xac, 0x66, 0xb3, 0xbc, 0xef, 0x8d, 0xeb, 0x67, 0xb6, 0x29, 0xcd,
    0x1d, 0x26, 0xa9, 0x8e, 0xa3, 0xa4, 0x8c, 0xf1, 0xd7, 0xfe, 0x8b, 0xb2, 0x2d, 0xf6, 0xb5, 0x69, 0x1c, 0x56, 0x6d,
    0x91, 0x3b, 0xdb, 0x36, 0xd8, 0x9b, 0xbc, 0x2b, 0xb6, 0x59, 0x96, 0x05, 0xdf, 0xf4, 0xf9, 0x8d, 0x09, 0x7e, 0xfb,
    0xdb, 0xc9, 0x89, 0x69, 0x63, 0xdc, 0xdb, 0xca, 0x10, 0xd9, 0x7f, 0x7b, 0xff, 0x3e, 0xdf, 0xfc, 0x29, 0xaf, 0xcd,
    0x24, 0xc8, 0x7b, 0x5b, 0x9a, 0x60, 0xfa, 0x73, 0xf8, 0x0b, 0xf6, 0xee, 0xbe, 0x32, 0x58, 0xda, 0x7e, 0x57, 0xe5,
    0xf7, 0x59, 0x70, 0x55, 0xb5, 0xc5, 0x75, 0x30, 0x5d, 0xac, 0xf7, 0x4d, 0x41, 0xca, 0xa1, 0x9f, 0x98, 0xe9, 0x63,
    0x65, 0x1c, 0xb8, 0xec, 0x87, 0xdc, 0x6d, 0xb1, 0xce, 0xef, 0x26, 0x03, 0x61, 0x9b, 0x89, 0xf8, 0xdd, 0xc4, 0x7c,
    0xcd, 0xc3, 0x70, 0x7a, 0xe9, 0x1f, 0xe1, 0x74, 0xc6, 0xc3, 0x70, 0xd1, 0x19, 0xb7, 0xef, 0x1a, 0xc8, 0x27, 0x1f,
    0x96, 0xbb, 0xdc, 0x6d, 0xa1, 0xcc, 0x82, 0x9a, 0x0b, 0x0c, 0xc3, 0x04, 0x78, 0x8a, 0x42, 0x33, 0xce, 0x51, 0x32,
    0xae, 0x8b, 0x98, 0x69, 0xe0, 0x8a, 0x69, 0x10, 0x02, 0x35, 0x84, 0x0f, 0x01, 0xac, 0x6d, 0x55, 0x65, 0x41, 0xd3,
    0x36, 0x26, 0x80, 0xde, 0x75, 0xed, 0xb5, 0xc9, 0x82, 0x62, 0xdf, 0x75, 0xa6, 0x71, 0x6f, 0xda, 0xaa, 0xed, 0x82,
    0xd9, 0xea, 0x0b, 0x78, 0xf6, 0xef, 0xb3, 0x97, 0x70, 0x5d, 0xde, 0xf4, 0xeb, 0xb6, 0xab, 0xb3, 0xc0, 0x6f, 0xca,
    0xe4, 0xab, 0x47, 0x77, 0x00, 0x7a, 0x4c, 0xcf, 0x26, 0x59, 0xdb, 0xd9, 0x8d, 0x6d, 0xb2, 0x80, 0x0b, 0xe0, 0x49,
    0x30, 0x5b, 0x7d, 0x98, 0x1e, 0x4e, 0x98, 0xe4, 0x84, 0xc9, 0xe8, 0x65, 0x3b, 0xf9, 0xf9, 0xc3, 0xb2, 0xbf, 0xd9,
    0xc0, 0x5d, 0x5d, 0x35, 0x7d, 0x16, 0x6c, 0x9d, 0xdb, 0xcd, 0x67, 0xb3, 0xdb, 0xdb, 0x5b, 0xbc, 0x95, 0xd8, 0x76,
    0x9b, 0x99, 0x08, 0xc3, 0x70, 0xd6, 0xdf, 0x6c, 0x02, 0x18, 0xe2, 0x23, 0x10, 0x2a, 0x80, 0xad, 0xb1, 0x9b, 0xad,
    0xf3, 0xf4, 0xea, 0xab, 0x47, 0x73, 0x58, 0x12, 0xc7, 0xea, 0xc3, 0x2f, 0x67, 0xab, 0x74, 0x67, 0xab, 0x98, 0x6f,
    0xce, 0xd0, 0xbc, 0xf8, 0x81, 0x8c, 0x8a, 0x73, 0x01, 0x02, 0x42, 0xff, 0x5f, 0x30, 0xa2, 0xc7, 0x37, 0xf6, 0xd1,
    0x1b, 0x9c, 0xbd, 0x81, 0x80, 0x2f, 0x00, 0x44, 0x1d, 0xb1, 0xf4, 0x24, 0xce, 0x69, 0xfa, 0x86, 0x87, 0x4f, 0x03,
    0x24, 0xf3, 0xc7, 0xe8, 0xfc, 0x9d, 0x89, 0x9f, 0x88, 0x81, 0xb0, 0x3f, 0x09, 0x31, 0xb1, 0xe5, 0x3f, 0x45, 0xb9,
    0x06, 0x3d, 0x8e, 0x68, 0x46, 0xf4, 0xe9, 0x8d, 0x56, 0x02, 0x7d, 0x23, 0xb6, 0xbc, 0x66, 0x11, 0xd3, 0xb9, 0x04,
    0x39, 0x9a, 0x24, 0x41, 0xde, 0x88, 0x6d, 0xf4, 0x53, 0x74, 0x3e, 0xc6, 0xe4, 0xc3, 0x05, 0xc1, 0x3c, 0x0f, 0x82,
    0x27, 0x0c, 0xda, 0x73, 0x0c, 0xf0, 0xd7, 0xd6, 0x36, 0x93, 0x20, 0x98, 0x1e, 0xd6, 0xc6, 0x15, 0xdb, 0x49, 0x30,
    0x2b, 0xda, 0x66, 0x6d, 0x37, 0xf8, 0x6b, 0xdf, 0x36, 0xc1, 0x14, 0xdd, 0xd6, 0x34, 0x93, 0xa3, 0x28, 0x09, 0x1a,
    0x3f, 0x33, 0xf9, 0x78, 0xc6, 0x4d, 0x1f, 0x4f, 0xf9, 0xe1, 0xac, 0xab, 0x4c, 0xe6, 0x90, 0x32, 0xfa, 0xf2, 0x34,
    0x7a, 0xd5, 0x96, 0xf7, 0x2f, 0xa4, 0xce, 0x96, 0x0f, 0x79, 0x63, 0x9b, 0xc6, 0x74, 0xef, 0xcd, 0x9d, 0xcb, 0x82,
    0xff, 0xb2, 0x7f, 0xb0, 0xf0, 0x27, 0xe3, 0x6e, 0xdb, 0xee, 0xba, 0x9f, 0x43, 0xf0, 0xf5, 0xa0, 0x6e, 0x41, 0x59,
    0x63, 0x33, 0x87, 0xf9, 0xae, 0xc7, 0xbe, 0xb2, 0x85, 0x99, 0xf0, 0x29, 0xd6, 0xf9, 0xee, 0xc9, 0x92, 0xe6, 0xe8,
    0xdc, 0x87, 0x65, 0x69, 0x6f, 0xa0, 0xa8, 0xf2, 0xbe, 0xcf, 0x82, 0x66, 0x50, 0x15, 0xc0, 0x18, 0xea, 0x6d, 0x53,
    0x54, 0xb6, 0xb8, 0xce, 0x82, 0x4f, 0x64, 0xf5, 0xb7, 0xf7, 0xdf, 0x95, 0x93, 0x8b, 0xbe, 0xb7, 0xe5, 0xc5, 0x14,
    0x6f, 0xf2, 0x6a, 0x6f, 0x20, 0x03, 0xb7, 0xb5, 0xfd, 0x93, 0x81, 0x8b, 0x17, 0xc5, 0x76, 0xfd, 0xf5, 0xc5, 0x14,
    0xd7, 0x6d, 0xb1, 0xef, 0x27, 0xd3, 0xe0, 0x98, 0x59, 0xcb, 0x7c, 0xa8, 0x7b, 0xc1, 0x97, 0xc1, 0x47, 0x16, 0xb1,
    0xca, 0xac, 0x5d, 0xf0, 0x94, 0x81, 0x5f, 0x3d, 0xf6, 0x93, 0x06, 0xbb, 0xbe, 0xb7, 0xd3, 0xc3, 0x69, 0x70, 0xd9,
    0xef, 0xf2, 0xe6, 0x63, 0x41, 0x32, 0x90, 0x02, 0xbd, 0x41, 0xa2, 0x28, 0xda, 0x77, 0x79, 0x73, 0x5a, 0x70, 0x96,
    0x1f, 0xc9, 0xaf, 0x1e, 0xbb, 0x49, 0x43, 0x95, 0xed, 0xfa, 0xa4, 0x71, 0x39, 0x2b, 0xed, 0xcd, 0xea, 0xc3, 0x61,
    0xfa, 0xe4, 0xc7, 0x5f, 0xf7, 0xa6, 0xbb, 0x7f, 0x67, 0x2a, 0x53, 0xb8, 0xb6, 0x9b, 0x04, 0x5f, 0x36, 0xc6, 0x05,
    0xd3, 0xc1, 0xe1, 0x3f, 0xbe, 0xff, 0xe1, 0xfb, 0xac, 0x9d, 0xd8, 0xe9, 0xe5, 0x4b, 0xdc, 0x54, 0xe1, 0x7f, 0xee,
    0x4c, 0xf5, 0xdf, 0xd9, 0x05, 0xd5, 0xf8, 0x8b, 0x5f, 0x82, 0x29, 0x7a, 0x7f, 0x3f, 0x3c, 0x15, 0x7a, 0x4a, 0xc6,
    0xaf, 0xef, 0xea, 0xea, 0x92, 0x3c, 0x64, 0x91, 0x9e, 0x1e, 0x3e, 0x1c, 0xa6, 0x87, 0xe9, 0x62, 0x39, 0x1b, 0x6a,
    0xf5, 0x6a, 0xe9, 0xcb, 0xe6, 0xea, 0x77, 0x8f, 0x57, 0xed, 0x1d, 0xeb, 0xed, 0x83, 0x6d, 0x36, 0x73, 0xdb, 0x6c,
    0x4d, 0x67, 0xdd, 0xa1, 0xb4, 0x37, 0x97, 0xb6, 0xd9, 0xed, 0xdd, 0xe3, 0x2e, 0x2f, 0x4b, 0x9a, 0xd1, 0xbb, 0xbb,
    0xc5, 0xba, 0x6d, 0x1c, 0x71, 0x9a, 0x39, 0x37, 0xf5, 0x61, 0x98, 0xf7, 0xf5, 0x60, 0x9e, 0xea, 0x57, 0x07, 0x0a,
    0xb8, 0x47, 0x67, 0xee, 0x1c, 0xcb, 0x2b, 0xbb, 0x69, 0xe6, 0x85, 0x69, 0x9c, 0xe9, 0x06, 0xa1, 0x75, 0x5e, 0xdb,
    0xea, 0x7e, 0xde, 0xe7, 0x4d, 0xcf, 0x7a, 0xd3, 0xd9, 0xf5, 0xe1, 0x6a, 0xef, 0x5c, 0xdb, 0x3c, 0x5e, 0xb5, 0x5d,
    0x69, 0xba, 0x79, 0xb8, 0x18, 0x08, 0xd6, 0xe5, 0xa5, 0xdd, 0xf7, 0x73, 0x94, 0x9d, 0xa9, 0x17, 0x57, 0x79, 0x71,
    0xbd, 0xe9, 0xda, 0x7d, 0x53, 0xb2, 0x82, 0x2a, 0xe7, 0xfc, 0x4b, 0xbe, 0xce, 0xa5, 0x29, 0x16, 0xe3, 0xdb, 0x7a,
    0xbd, 0x5e, 0x54, 0xb6, 0x31, 0x6c, 0xa8, 0x44, 0x73, 0x81, 0x8a, 0xc4, 0xce, 0xcc, 0x44, 0x41, 0x03, 0x83, 0x8d,
    0x3c, 0x0c, 0x5f, 0x2d, 0x8e, 0xee, 0x84, 0x8b, 0x62, 0xdf, 0xf5, 0x6d, 0x37, 0xdf, 0xb5, 0x96, 0xcc, 0x3c, 0xd4,
    0xb9, 0x6d, 0xce, 0xad, 0xa7, 0x30, 0x59, 0x8c, 0x47, 0xca, 0xdc, 0x36, 0x7e, 0x19, 0x7f, 0xb0, 0x2c, 0x6a, 0xdb,
    0x0c, 0x07, 0xe3, 0x5c, 0x44, 0xe1, 0xee, 0xee, 0x80, 0x63, 0x80, 0x3c, 0x1e, 0xb9, 0xd7, 0x95, 0xb9, 0x5b, 0xfc,
    0xba, 0xef, 0x9d, 0x5d, 0xdf, 0xb3, 0xf1, 0x60, 0x9d, 0xf7, 0xbb, 0xbc, 0x30, 0xec, 0xca, 0xb8, 0x5b, 0x63, 0x9a,
    0x85, 0x5f, 0x83, 0x59, 0x67, 0xea, 0x7e, 0xc4, 0xe9, 0xa4, 0xc6, 0x07, 0xe8, 0x73, 0x5d, 0x7f, 0x8f, 0x9b, 0x62,
    0xf1, 0xb1, 0xce, 0xbb, 0x8d, 0x6d, 0xd8, 0x55, 0xeb, 0x5c, 0x5b, 0xcf, 0x59, 0xbc, 0xbb, 0x5b, 0x8c, 0x43, 0xa4,
    0x6c, 0xce, 0xc9, 0x4c, 0x7f, 0x5e, 0x1e, 0xf1, 0xe6, 0xbb, 0x3b, 0xe8, 0xdb, 0xca, 0x96, 0x23, 0x9f, 0x67, 0x81,
    0xf0, 0x04, 0x0f, 0xd7, 0xbb, 0x3b, 0xa0, 0xb1, 0x23, 0xd4, 0x6a, 0x9d, 0xe4, 0x3c, 0xfc, 0xc4, 0x8e, 0x94, 0xeb,
    0xb5, 0xb8, 0x5a, 0x9f, 0x90, 0xa2, 0x63, 0xef, 0x60, 0xc7, 0xa8, 0x10, 0x6a, 0x77, 0xb7, 0x38, 0xee, 0x8d, 0xda,
    0xdd, 0x1d, 0xac, 0x4f, 0x8b, 0xc7, 0x33, 0x2d, 0x3e, 0x54, 0xe7, 0xfb, 0xae, 0x9a, 0x04, 0x9f, 0x08, 0xdd, 0x57,
    0xf2, 0xcd, 0xd3, 0xb1, 0x74, 0xf1, 0xf2, 0xb1, 0x74, 0x01, 0xf4, 0x21, 0xf3, 0x6d, 0x7b, 0x97, 0x5d, 0xf8, 0x03,
    0x42, 0x81, 0x50, 0x17, 0xaf, 0xe4, 0xdb, 0x57, 0xf2, 0xcd, 0xff, 0xca, 0x79, 0xf3, 0x4f, 0x1f, 0x36, 0x9f, 0x71,
    0xd2, 0x7c, 0xe6, 0x29, 0x33, 0x78, 0x47, 0xce, 0xbf, 0x92, 0x6f, 0x83, 0xe9, 0xc1, 0x62, 0x6f, 0x37, 0xfc, 0xff,
    0x02, 0xda, 0xbf, 0xc5, 0x51, 0xbe, 0xe6, 0x09, 0xa6, 0xe0, 0x1f, 0x03, 0x44, 0x28, 0x13, 0x88, 0xdf, 0x28, 0x54,
    0x1c, 0xe8, 0x13, 0x26, 0x82, 0x18, 0x93, 0x14, 0x78, 0x84, 0x32, 0x06, 0x2e, 0x40, 0x70, 0xd4, 0xdf, 0x0b, 0x89,
    0x91, 0x86, 0xf8, 0x8d, 0x08, 0x51, 0x0a, 0x18, 0x78, 0x23, 0x14, 0x02, 0x24, 0xb1, 0xc8, 0x3a, 0x04, 0x51, 0x48,
    0x0c, 0x63, 0x08, 0x21, 0xc2, 0x30, 0xc5, 0x24, 0x82, 0x04, 0x63, 0x0e, 0x02, 0x95, 0xae, 0x98, 0x46, 0x0e, 0x11,
    0xca, 0x28, 0x4f, 0x50, 0x49, 0xf0, 0x8f, 0x01, 0x8e, 0x18, 0x85, 0x60, 0x18, 0xf2, 0xef, 0x25, 0x8a, 0x18, 0x62,
    0x54, 0xea, 0x35, 0x8f, 0x30, 0x55, 0x30, 0x3c, 0x07, 0x78, 0xb9, 0x00, 0xfd, 0x12, 0x68, 0xe2, 0x3f, 0x17, 0x34,
    0x89, 0x82, 0x83, 0xc4, 0x34, 0xc9, 0xe9, 0x5b, 0x52, 0xc0, 0xf0, 0x1c, 0x70, 0xe3, 0x1c, 0xc2, 0x7f, 0x1a, 0x33,
    0xf9, 0x9f, 0x8b, 0x19, 0x27, 0x04, 0x04, 0x2a, 0xf9, 0x9a, 0x4b, 0x8c, 0xc0, 0x3f, 0x86, 0xb5, 0xb9, 0x80, 0xe4,
    0x4d, 0x0a, 0x09, 0x44, 0x18, 0x25, 0x90, 0x82, 0x26, 0x74, 0x53, 0x4c, 0xd4, 0x20, 0xc4, 0x04, 0x7e, 0x2e, 0x8c,
    0xea, 0xdf, 0x18, 0xc6, 0x97, 0x7c, 0xfa, 0x37, 0x76, 0xe9, 0xff, 0xa7, 0x04, 0x99, 0x7b, 0xf3, 0x78, 0x7e, 0x5a,
    0x33, 0xe9, 0xcf, 0xdd, 0x67, 0x9f, 0x27, 0x9f, 0xfe, 0x12, 0xb9, 0x31, 0x9d, 0xb3, 0x45, 0x5e, 0x8d, 0x9f, 0x2c,
    0xb5, 0x2d, 0xcb, 0xca, 0x2c, 0x3e, 0x17, 0xff, 0xa2, 0xb2, 0x3b, 0xd6, 0xed, 0x2b, 0x93, 0x5d, 0x98, 0x1b, 0xd3,
    0xb4, 0x65, 0x79, 0xe1, 0xff, 0x86, 0xfd, 0x78, 0x6c, 0xf8, 0x5b, 0x96, 0xd1, 0xfa, 0xf4, 0x47, 0x4c, 0x76, 0xe1,
    0xd7, 0x38, 0x8d, 0xd7, 0xd6, 0x99, 0xae, 0xb2, 0xb5, 0x75, 0xd9, 0x85, 0xf8, 0xe4, 0x81, 0xfc, 0x0f, 0xec, 0xf4,
    0xf3, 0x8d, 0xae, 0x79, 0x8c, 0x61, 0x94, 0x42, 0x84, 0x5a, 0x45, 0x20, 0x30, 0x4a, 0x28, 0xc9, 0xa4, 0x4e, 0x0b,
    0xe4, 0x4a, 0x32, 0xe4, 0x42, 0xa3, 0x14, 0x0c, 0x79, 0x12, 0xa3, 0x4a, 0xe3, 0x91, 0xe0, 0x09, 0x45, 0x43, 0xac,
    0x51, 0x2a, 0x7a, 0xc6, 0x74, 0xc4, 0xa2, 0x08, 0x63, 0x86, 0x61, 0x12, 0xa1, 0xe2, 0x8a, 0xa1, 0xd0, 0x0a, 0x75,
    0x24, 0x2a, 0xc6, 0x23, 0xa4, 0xbf, 0xad, 0x51, 0x87, 0xbc, 0x60, 0xc8, 0x95, 0x40, 0x2e, 0x22, 0x86, 0x92, 0xa7,
    0xa4, 0x8b, 0xa1, 0x4a, 0xa3, 0x91, 0xe0, 0x1a, 0x42, 0x86, 0xb1, 0x66, 0x28, 0xa5, 0x1a, 0x88, 0xd8, 0x0f, 0x89,
    0x30, 0x26, 0xc5, 0xc4, 0xa2, 0x50, 0x68, 0xc9, 0xbc, 0x66, 0x2a, 0x20, 0x09, 0x13, 0x64, 0x67, 0xc1, 0xb8, 0x5f,
    0x95, 0x23, 0x97, 0x82, 0x09, 0x8c, 0x44, 0xcc, 0x04, 0x6a, 0x2d, 0x98, 0x44, 0xad, 0x62, 0xa6, 0x30, 0x14, 0x09,
    0xc3, 0x30, 0x8d, 0x18, 0x72, 0x9d, 0x92, 0x19, 0x8a, 0x56, 0x49, 0x46, 0x4a, 0xf3, 0xb8, 0xc7, 0x50, 0xa5, 0x0c,
    0xa5, 0x4e, 0x90, 0x2b, 0xed, 0x87, 0x0a, 0x81, 0x9c, 0x73, 0x26, 0x51, 0x52, 0x21, 0x8a, 0x63, 0xcd, 0x22, 0x54,
    0x89, 0xa4, 0x7a, 0xa4, 0xe5, 0x48, 0x73, 0x4c, 0xc8, 0x6c, 0x90, 0xa8, 0x65, 0x84, 0x3a, 0x95, 0xa0, 0x31, 0x8c,
    0x39, 0x70, 0xc2, 0xf3, 0xa1, 0x16, 0xe4, 0x27, 0x31, 0xc9, 0x02, 0xd3, 0x88, 0x42, 0xd5, 0x8b, 0x84, 0x12, 0x04,
    0x86, 0xdc, 0x17, 0xc1, 0x58, 0x81, 0x44, 0x4e, 0xdb, 0x90, 0x0a, 0xe4, 0x3a, 0x42, 0x2e, 0x13, 0x94, 0x52, 0xf8,
    0xdf, 0x3a, 0x8c, 0x7b, 0x86, 0xa1, 0x8a, 0x50, 0x6a, 0xce, 0x8e, 0x43, 0x05, 0xa3, 0xec, 0x4b, 0x40, 0xa2, 0x0a,
    0x25, 0xd3, 0x18, 0x0b, 0xca, 0x1a, 0x95, 0x4a, 0x46, 0xf9, 0xa4, 0x46, 0x9a, 0xa3, 0x48, 0x63, 0xfa, 0xd2, 0x42,
    0x4d, 0x90, 0x49, 0x2e, 0x99, 0xc4, 0x58, 0xa4, 0x0c, 0x13, 0x95, 0x56, 0x1c, 0x85, 0x8a, 0x19, 0x27, 0x4c, 0x0a,
    0x8c, 0x53, 0x8d, 0x22, 0xd1, 0xc0, 0x31, 0x12, 0x11, 0x2a, 0xa5, 0xc9, 0xb0, 0x44, 0x78, 0x4a, 0xa2, 0xe6, 0x91,
    0xcf, 0x56, 0x1d, 0x47, 0x1e, 0x5b, 0x41, 0x89, 0xc9, 0x69, 0x61, 0xc2, 0x29, 0xd5, 0x0c, 0x53, 0x29, 0x18, 0x47,
    0xc9, 0x13, 0x46, 0xae, 0x0e, 0x9b, 0xa2, 0x89, 0x57, 0xc6, 0x0f, 0x35, 0x93, 0xa8, 0x24, 0x39, 0x19, 0xf2, 0xb4,
    0xc0, 0x50, 0x22, 0x4f, 0xe3, 0xc1, 0xa5, 0x34, 0xf5, 0xbf, 0xa3, 0x90, 0x10, 0x14, 0x28, 0x42, 0x92, 0x8f, 0x53,
    0x01, 0x8a, 0x29, 0xa0, 0x0d, 0x4a, 0xd4, 0x10, 0x11, 0x11, 0xc3, 0x50, 0xfb, 0xed, 0xa5, 0x18, 0xe3, 0x3a, 0xaa,
    0x38, 0x6a, 0x62, 0x96, 0x49, 0x5a, 0x50, 0xdb, 0x8b, 0x21, 0xf7, 0xa0, 0x47, 0x82, 0x33, 0x4c, 0x04, 0x91, 0x69,
    0xc8, 0x19, 0xc7, 0x28, 0xe6, 0x0f, 0x35, 0x49, 0x27, 0x4c, 0x62, 0xc2, 0x13, 0x8a, 0x14, 0x4e, 0x71, 0x1c, 0xf9,
    0x08, 0xe1, 0x64, 0x77, 0x18, 0xc7, 0x34, 0x99, 0x26, 0x27, 0x5a, 0x45, 0x14, 0x77, 0x11, 0x6a, 0x49, 0x1b, 0x15,
    0x49, 0xc1, 0x12, 0x54, 0xa1, 0x02, 0x8d, 0x9a, 0xf6, 0x1e, 0x38, 0x4a, 0x6a, 0x21, 0x61, 0xaa, 0x09, 0x29, 0x1d,
    0xc5, 0x20, 0x51, 0x48, 0xfa, 0x0e, 0xd0, 0x1e, 0xda, 0x58, 0x90, 0x2e, 0xcd, 0xd3, 0x82, 0x0d, 0x1b, 0xe7, 0x51,
    0xa2, 0x6d, 0xe0, 0x28, 0x92, 0x74, 0x20, 0x05, 0x86, 0xd2, 0x6f, 0x8f, 0xf0, 0xba, 0xe2, 0x54, 0x78, 0xaf, 0x15,
    0x60, 0x2a, 0x08, 0x10, 0x8e, 0x71, 0x1c, 0xa3, 0xe4, 0x7e, 0x33, 0xb4, 0xc6, 0x44, 0xc9, 0x87, 0x9a, 0x40, 0x25,
    0xa8, 0x12, 0x4d, 0xaa, 0xe3, 0x84, 0xa1, 0x90, 0xe4, 0xb1, 0x60, 0x28, 0x23, 0xe1, 0xf7, 0x3c, 0x3a, 0x92, 0x32,
    0x19, 0x76, 0x1f, 0x38, 0x72, 0xe1, 0x09, 0xdf, 0xdc, 0x02, 0x94, 0x11, 0x86, 0x71, 0x84, 0x71, 0xc8, 0x51, 0x70,
    0xf2, 0x26, 0xe4, 0xfc, 0xe1, 0x59, 0x45, 0x6a, 0xda, 0xe6, 0xc1, 0x74, 0xed, 0x0b, 0xb5, 0x94, 0x7e, 0xd8, 0x8d,
    0xed, 0xed, 0x55, 0x65, 0x3e, 0xfb, 0x2c, 0xfa, 0x57, 0xae, 0x85, 0x1c, 0xd3, 0x34, 0x01, 0x5d, 0x50, 0xc1, 0x88,
    0x13, 0x7f, 0xec, 0xc4, 0x4a, 0x50, 0xe8, 0x0e, 0xb9, 0xa5, 0x25, 0xf8, 0xb4, 0xf7, 0xa5, 0xe4, 0x58, 0x49, 0xf4,
    0xb1, 0x90, 0xe8, 0x63, 0x1d, 0x49, 0xc6, 0x32, 0xa2, 0x4e, 0x55, 0x84, 0x74, 0x8c, 0x55, 0x24, 0x82, 0xa7, 0x2a,
    0xa2, 0x46, 0x5a, 0x51, 0x11, 0x83, 0x10, 0x62, 0x8c, 0xd3, 0x88, 0xd1, 0x72, 0xe0, 0x33, 0x99, 0xf9, 0x4c, 0xa6,
    0xca, 0xc0, 0x8e, 0xa5, 0x81, 0x1d, 0x6b, 0x03, 0x7b, 0x2a, 0x0e, 0xec, 0x54, 0x1d, 0xd8, 0x59, 0x79, 0x60, 0x67,
    0xe5, 0x81, 0x9d, 0x95, 0x87, 0x81, 0x7e, 0xa8, 0x7d, 0x12, 0x43, 0x5c, 0xf8, 0x9c, 0x8d, 0x41, 0x60, 0x12, 0x27,
    0x4c, 0x61, 0x92, 0xc6, 0x14, 0xe7, 0x6c, 0x98, 0x26, 0xea, 0x3c, 0x1f, 0x7c, 0x75, 0x3d, 0xe6, 0x03, 0x55, 0x00,
    0x2a, 0x67, 0x31, 0x67, 0x02, 0x93, 0x28, 0x01, 0x85, 0xa9, 0x4c, 0xfd, 0xe8, 0xd3, 0x3c, 0xd5, 0x8f, 0x64, 0xac,
    0x1f, 0x29, 0x65, 0x93, 0x50, 0x70, 0xd2, 0xfd, 0x50, 0xfb, 0x75, 0x38, 0x53, 0xc5, 0x90, 0x07, 0x21, 0x28, 0x18,
    0x6b, 0x00, 0xa8, 0xfe, 0x59, 0x39, 0x50, 0x6c, 0x4c, 0x12, 0xa6, 0xce, 0xd3, 0xe5, 0xa1, 0xa6, 0x54, 0xd1, 0xc5,
    0x4b, 0x01, 0xdf, 0xd3, 0x0b, 0x1c, 0xa3, 0x7f, 0xfc, 0x61, 0xc7, 0x41, 0x76, 0x7c, 0x61, 0xc7, 0x17, 0x81, 0xfa,
    0x1f, 0xcc, 0x85, 0xe5, 0x6c, 0x68, 0xef, 0x2c, 0x67, 0xc3, 0x55, 0x01, 0x75, 0x67, 0x56, 0x4b, 0xea, 0x72, 0xac,
    0x96, 0x5b, 0x0e, 0xb6, 0xcc, 0xb6, 0x7c, 0xf5, 0xac, 0xeb, 0xb7, 0x9c, 0x6d, 0xf9, 0x6a, 0x39, 0xf4, 0x65, 0x68,
    0xbe, 0x33, 0xeb, 0xce, 0xf4, 0xdb, 0x6f, 0x87, 0x01, 0xdf, 0xd9, 0x1f, 0x26, 0x57, 0xe3, 0x0c, 0x2c, 0x6d, 0xbd,
    0x81, 0xbe, 0x2b, 0xb2, 0x63, 0xa7, 0x7f, 0x9c, 0x40, 0x55, 0x86, 0x65, 0x18, 0x26, 0x02, 0x29, 0xa7, 0xbc, 0x1d,
    0x59, 0xf0, 0xe9, 0x8f, 0x95, 0xb5, 0xad, 0x9c, 0xe9, 0xe6, 0xb6, 0xa1, 0xe9, 0x09, 0xb5, 0x68, 0xa6, 0xd0, 0x9b,
    0x9d, 0xcd, 0x27, 0x31, 0x51, 0xb9, 0xdb, 0x77, 0xb9, 0x33, 0x93, 0x58, 0xc5, 0xf2, 0xd5, 0x14, 0xb6, 0x7b, 0xc3,
    0xba, 0xd6, 0xd1, 0x88, 0x90, 0x71, 0x69, 0x36, 0x53, 0xb8, 0xea, 0xa8, 0xbd, 0xd0, 0x98, 0xbe, 0x9f, 0x70, 0x1e,
    0xbd, 0x9a, 0xfa, 0x9b, 0x8c, 0x2e, 0xef, 0xdd, 0x24, 0x8d, 0x5e, 0x4d, 0xe9, 0x6a, 0x62, 0x36, 0x5a, 0xbd, 0xf4,
    0x1d, 0x90, 0xd5, 0xfb, 0xad, 0x81, 0xb7, 0xef, 0x7e, 0x84, 0x5b, 0x5b, 0x55, 0xd0, 0xb4, 0xb7, 0xe0, 0xba, 0x7b,
    0x70, 0x2d, 0xc9, 0x35, 0xa6, 0x70, 0x44, 0xba, 0xad, 0x81, 0xb1, 0xbf, 0x82, 0x88, 0xcb, 0xab, 0x6e, 0xf5, 0x63,
    0x65, 0xf2, 0xde, 0xc0, 0xc6, 0xde, 0x18, 0xb0, 0x0e, 0xfa, 0xb6, 0x36, 0xe0, 0x2c, 0x3d, 0x4e, 0x82, 0x9e, 0x6f,
    0x39, 0x1b, 0x16, 0x19, 0x1a, 0x88, 0xb6, 0xcc, 0x1a, 0xe3, 0x56, 0x63, 0xa7, 0x70, 0xb9, 0x95, 0x03, 0xe0, 0xef,
    0x8c, 0x73, 0xb6, 0xd9, 0x10, 0xe0, 0x72, 0xb5, 0xa4, 0x4e, 0x3d, 0xe4, 0xbe, 0x97, 0x9a, 0xcd, 0x6e, 0xed, 0xda,
    0xd2, 0xfd, 0xc7, 0x6a, 0xe9, 0xbb, 0x6b, 0xa4, 0x81, 0x1a, 0x3c, 0xc3, 0x65, 0x8d, 0xa7, 0x2a, 0xd3, 0x6c, 0xdc,
    0x36, 0x93, 0x02, 0x76, 0x55, 0x5e, 0x98, 0x6d, 0x5b, 0x95, 0xa6, 0xcb, 0xde, 0xbd, 0xfb, 0xee, 0xf7, 0x74, 0xb3,
    0x43, 0x26, 0x9c, 0x24, 0x77, 0xfd, 0xf5, 0x20, 0x48, 0xc4, 0x28, 0x17, 0xa9, 0x61, 0x27, 0x77, 0x79, 0xdf, 0xdf,
    0xb6, 0x5d, 0xf9, 0x4c, 0xcb, 0x8f, 0xc7, 0xc1, 0xd9, 0x0a, 0x96, 0x76, 0xec, 0x7f, 0x9a, 0x7b, 0x43, 0xca, 0x5c,
    0xbb, 0xd9, 0x54, 0xe6, 0xc8, 0xb1, 0x5a, 0xce, 0xec, 0xb0, 0x9a, 0xff, 0x39, 0x0b, 0x91, 0x7e, 0x7f, 0x55, 0x5b,
    0xb7, 0x7a, 0x97, 0xdf, 0x98, 0x27, 0xe4, 0x67, 0xe4, 0xe4, 0xc0, 0xbb, 0x1d, 0x65, 0xb6, 0x7c, 0xf5, 0xe7, 0xf7,
    0xaf, 0xe1, 0x2f, 0xbb, 0x32, 0x77, 0x66, 0x08, 0x3d, 0x8f, 0x44, 0x6d, 0xdc, 0xb6, 0x2d, 0xb3, 0x1f, 0xff, 0xfc,
    0xee, 0xfd, 0x09, 0x95, 0xbd, 0x67, 0x02, 0xd3, 0x14, 0xc3, 0xfd, 0xd2, 0xbe, 0x72, 0x76, 0x97, 0x77, 0xce, 0xab,
    0x65, 0x54, 0xcd, 0x8f, 0x5e, 0xfb, 0xf9, 0xb5, 0xad, 0xcc, 0xe0, 0xf8, 0x28, 0x48, 0xee, 0x7c, 0xc2, 0xc6, 0xe3,
    0xda, 0x1f, 0x59, 0x39, 0x1b, 0x92, 0x64, 0xec, 0x92, 0x16, 0x6d, 0xd3, 0x53, 0x50, 0x9c, 0x7b, 0xff, 0x1d, 0x64,
    0xf0, 0x62, 0x23, 0xf7, 0x39, 0x6b, 0x30, 0x5d, 0x8c, 0x3d, 0xe0, 0x41, 0xd1, 0x11, 0xf7, 0xef, 0xbc, 0xb9, 0xf0,
    0xf7, 0x14, 0xed, 0x7a, 0xba, 0xc4, 0xfa, 0x62, 0x14, 0xff, 0xc8, 0x02, 0xcc, 0xcb, 0xf2, 0xed, 0x8d, 0x69, 0xdc,
    0xf7, 0xb6, 0x77, 0xa6, 0x31, 0xdd, 0x24, 0xf0, 0x0d, 0xf6, 0xe0, 0x12, 0x4e, 0x17, 0x0f, 0x93, 0x29, 0x3c, 0x3e,
    0xbb, 0x69, 0x9a, 0xcd, 0x46, 0x35, 0x3e, 0xc2, 0xfd, 0xdd, 0x62, 0xee, 0x5c, 0x67, 0xaf, 0xf6, 0xce, 0x3c, 0x63,
    0x1c, 0x9d, 0x26, 0x86, 0xec, 0xb9, 0xc9, 0xd4, 0x7f, 0x7f, 0x7d, 0x94, 0x99, 0x04, 0xc4, 0x12, 0x4c, 0x21, 0xcb,
    0x32, 0x08, 0x8e, 0x7c, 0x01, 0x7c, 0x03, 0x01, 0x75, 0x52, 0x03, 0x98, 0x9f, 0x8d, 0x2e, 0x9e, 0xad, 0xf0, 0x5c,
    0x69, 0xff, 0xb7, 0x4a, 0x2f, 0xfd, 0xf2, 0x4f, 0xfe, 0x7f, 0xca, 0x03, 0xea, 0x7d, 0x3f, 0x9b, 0xf6, 0x97, 0x06,
    0x3e, 0x6c, 0x09, 0x16, 0x1c, 0x58, 0x27, 0xc1, 0xd9, 0x17, 0xc1, 0xd3, 0x7e, 0x3c, 0x6b, 0x85, 0xcf, 0x86, 0xf2,
    0x38, 0xf3, 0x57, 0xad, 0xff, 0x03, 0x85, 0x35, 0x33, 0x7a, 0x7a, 0x1d, 0x00, 0x00};

}  // namespace Watchy_WebServer