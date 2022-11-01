/**
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mitm_lib/common/liblte_security.h"
#include "mitm_lib/common/test_common.h"
#include "mitm_lib/srsran.h"

/*
 * Prototypes
 */

int32 arrcmp(uint8_t const* const a, uint8_t const* const b, uint32 len)
{
  uint32 i = 0;

  for (i = 0; i < len; i++) {
    if (a[i] != b[i]) {
      return a[i] - b[i];
    }
  }
  return 0;
}

/*
 * Tests
 *
 * Document Reference: 33.401 V13.1.0 Annex C.3
 * 					   Specification of the 3GPP Confidentiality and
 *					       Integrity Algorithms UEA2 & UIA2 D4 v1.0
 */

int test_set_1()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0xd3, 0xc5, 0xd5, 0x92, 0x32, 0x7f, 0xb1, 0x1c, 0x40, 0x35, 0xc6, 0x68, 0x0a, 0xf8, 0xc6, 0xd1};
  uint32_t count     = 0x398a59b4;
  uint8_t  bearer    = 0x15;
  uint8_t  direction = 1;
  uint32_t len_bits = 253, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {0x98, 0x1b, 0xa6, 0x82, 0x4c, 0x1b, 0xfb, 0x1a, 0xb4, 0x85, 0x47, 0x20, 0x29, 0xb7, 0x1d, 0x80,
                   0x8c, 0xe3, 0x3e, 0x2c, 0xc3, 0xc0, 0xb5, 0xfc, 0x1f, 0x3d, 0xe8, 0xa6, 0xdc, 0x66, 0xb1, 0xf0};
  uint8_t  ct[]  = {0x5d, 0x5b, 0xfe, 0x75, 0xeb, 0x04, 0xf6, 0x8c, 0xe0, 0xa1, 0x23, 0x77, 0xea, 0x00, 0xb3, 0x7d,
                  0x47, 0xc6, 0xa0, 0xba, 0x06, 0x30, 0x91, 0x55, 0x08, 0x6a, 0x85, 0x9c, 0x43, 0x41, 0xb3, 0x78};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

int test_set_2()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0x2b, 0xd6, 0x45, 0x9f, 0x82, 0xc4, 0x40, 0xe0, 0x95, 0x2c, 0x49, 0x10, 0x48, 0x05, 0xff, 0x48};
  uint32_t count     = 0xc675a64b;
  uint8_t  bearer    = 0x0c;
  uint8_t  direction = 1;
  uint32_t len_bits = 798, len_bytes = (len_bits + 7) / 8;
  uint8_t msg[] = {0x7e, 0xc6, 0x12, 0x72, 0x74, 0x3b, 0xf1, 0x61, 0x47, 0x26, 0x44, 0x6a, 0x6c, 0x38, 0xce, 0xd1, 0x66,
                   0xf6, 0xca, 0x76, 0xeb, 0x54, 0x30, 0x04, 0x42, 0x86, 0x34, 0x6c, 0xef, 0x13, 0x0f, 0x92, 0x92, 0x2b,
                   0x03, 0x45, 0x0d, 0x3a, 0x99, 0x75, 0xe5, 0xbd, 0x2e, 0xa0, 0xeb, 0x55, 0xad, 0x8e, 0x1b, 0x19, 0x9e,
                   0x3e, 0xc4, 0x31, 0x60, 0x20, 0xe9, 0xa1, 0xb2, 0x85, 0xe7, 0x62, 0x79, 0x53, 0x59, 0xb7, 0xbd, 0xfd,
                   0x39, 0xbe, 0xf4, 0xb2, 0x48, 0x45, 0x83, 0xd5, 0xaf, 0xe0, 0x82, 0xae, 0xe6, 0x38, 0xbf, 0x5f, 0xd5,
                   0xa6, 0x06, 0x19, 0x39, 0x01, 0xa0, 0x8f, 0x4a, 0xb4, 0x1a, 0xab, 0x9b, 0x13, 0x48, 0x80};
  uint8_t ct[]  = {0x3f, 0x67, 0x85, 0x07, 0x14, 0xb8, 0xda, 0x69, 0xef, 0xb7, 0x27, 0xed, 0x7a, 0x6c, 0x0c, 0x50, 0x71,
                  0x4a, 0xd7, 0x36, 0xc4, 0xf5, 0x60, 0x00, 0x06, 0xe3, 0x52, 0x5b, 0xe8, 0x07, 0xc4, 0x67, 0xc6, 0x77,
                  0xff, 0x86, 0x4a, 0xf4, 0x5f, 0xba, 0x09, 0xc2, 0x7c, 0xde, 0x38, 0xf8, 0x7a, 0x1f, 0x84, 0xd5, 0x9a,
                  0xb2, 0x55, 0x40, 0x8f, 0x2c, 0x7b, 0x82, 0xf9, 0xea, 0xd4, 0x1a, 0x1f, 0xe6, 0x5e, 0xab, 0xeb, 0xfb,
                  0xc1, 0xf3, 0xa4, 0xc5, 0x6c, 0x9a, 0x26, 0xfc, 0xf7, 0xb3, 0xd6, 0x6d, 0x02, 0x20, 0xee, 0x47, 0x75,
                  0xbc, 0x58, 0x17, 0x0a, 0x2b, 0x12, 0xf3, 0x43, 0x1d, 0x11, 0xb3, 0x44, 0xd6, 0xe3, 0x6c};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

int test_set_3()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0x0a, 0x8b, 0x6b, 0xd8, 0xd9, 0xb0, 0x8b, 0x08, 0xd6, 0x4e, 0x32, 0xd1, 0x81, 0x77, 0x77, 0xfb};
  uint32_t count     = 0x544d49cd;
  uint8_t  bearer    = 0x04;
  uint8_t  direction = 0;
  uint32_t len_bits = 310, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {0xfd, 0x40, 0xa4, 0x1d, 0x37, 0x0a, 0x1f, 0x65, 0x74, 0x50, 0x95, 0x68, 0x7d, 0x47,
                   0xba, 0x1d, 0x36, 0xd2, 0x34, 0x9e, 0x23, 0xf6, 0x44, 0x39, 0x2c, 0x8e, 0xa9, 0xc4,
                   0x9d, 0x40, 0xc1, 0x32, 0x71, 0xaf, 0xf2, 0x64, 0xd0, 0xf2, 0x48, 0x00};
  uint8_t  ct[]  = {0x48, 0x14, 0x8e, 0x54, 0x52, 0xa2, 0x10, 0xc0, 0x5f, 0x46, 0xbc, 0x80, 0xdc,
                  0x6f, 0x73, 0x49, 0x5b, 0x02, 0x04, 0x8c, 0x1b, 0x95, 0x8b, 0x02, 0x61, 0x02,
                  0xca, 0x97, 0x28, 0x02, 0x79, 0xa4, 0xc1, 0x8d, 0x2e, 0xe3, 0x08, 0x92, 0x1c};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

int test_set_4()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0xaa, 0x1f, 0x95, 0xae, 0xa5, 0x33, 0xbc, 0xb3, 0x2e, 0xb6, 0x3b, 0xf5, 0x2d, 0x8f, 0x83, 0x1a};
  uint32_t count     = 0x72d8c671;
  uint8_t  bearer    = 0x10;
  uint8_t  direction = 1;
  uint32_t len_bits = 1022, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {0xfb, 0x1b, 0x96, 0xc5, 0xc8, 0xba, 0xdf, 0xb2, 0xe8, 0xe8, 0xed, 0xfd, 0xe7, 0x8e, 0x57, 0xf2,
                   0xad, 0x81, 0xe7, 0x41, 0x03, 0xfc, 0x43, 0x0a, 0x53, 0x4d, 0xcc, 0x37, 0xaf, 0xce, 0xc7, 0x0e,
                   0x15, 0x17, 0xbb, 0x06, 0xf2, 0x72, 0x19, 0xda, 0xe4, 0x90, 0x22, 0xdd, 0xc4, 0x7a, 0x06, 0x8d,
                   0xe4, 0xc9, 0x49, 0x6a, 0x95, 0x1a, 0x6b, 0x09, 0xed, 0xbd, 0xc8, 0x64, 0xc7, 0xad, 0xbd, 0x74,
                   0x0a, 0xc5, 0x0c, 0x02, 0x2f, 0x30, 0x82, 0xba, 0xfd, 0x22, 0xd7, 0x81, 0x97, 0xc5, 0xd5, 0x08,
                   0xb9, 0x77, 0xbc, 0xa1, 0x3f, 0x32, 0xe6, 0x52, 0xe7, 0x4b, 0xa7, 0x28, 0x57, 0x60, 0x77, 0xce,
                   0x62, 0x8c, 0x53, 0x5e, 0x87, 0xdc, 0x60, 0x77, 0xba, 0x07, 0xd2, 0x90, 0x68, 0x59, 0x0c, 0x8c,
                   0xb5, 0xf1, 0x08, 0x8e, 0x08, 0x2c, 0xfa, 0x0e, 0xc9, 0x61, 0x30, 0x2d, 0x69, 0xcf, 0x3d, 0x44};
  uint8_t  ct[]  = {0xff, 0xcf, 0xc2, 0xfe, 0xad, 0x6c, 0x09, 0x4e, 0x96, 0xc5, 0x89, 0xd0, 0xf6, 0x77, 0x9b, 0x67,
                  0x84, 0x24, 0x6c, 0x3c, 0x4d, 0x1c, 0xea, 0x20, 0x3d, 0xb3, 0x90, 0x1f, 0x40, 0xad, 0x4f, 0xd7,
                  0x13, 0x8b, 0xc6, 0xd7, 0x7e, 0x83, 0x20, 0xcb, 0x10, 0x2f, 0x49, 0x7f, 0xdd, 0x44, 0xa2, 0x69,
                  0xa9, 0x6e, 0xcb, 0x28, 0x61, 0x77, 0x00, 0xe3, 0x32, 0xeb, 0x2f, 0x73, 0x6b, 0x34, 0xf4, 0xf2,
                  0x69, 0x30, 0x94, 0xe2, 0x2f, 0xf9, 0x4f, 0x9b, 0xe4, 0x72, 0x3d, 0xa4, 0x0c, 0x40, 0xdf, 0xd3,
                  0x93, 0x1c, 0xc1, 0xac, 0x97, 0x23, 0xf6, 0xb4, 0xa9, 0x91, 0x3e, 0x96, 0xb6, 0xdb, 0x7a, 0xbc,
                  0xac, 0xe4, 0x15, 0x17, 0x7c, 0x1d, 0x01, 0x15, 0xc5, 0xf0, 0x9b, 0x5f, 0xde, 0xa0, 0xb3, 0xad,
                  0xb8, 0xf9, 0xda, 0x6e, 0x9f, 0x9a, 0x04, 0xc5, 0x43, 0x39, 0x7b, 0x9d, 0x43, 0xf8, 0x73, 0x30};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

int test_set_5()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0x96, 0x18, 0xae, 0x46, 0x89, 0x1f, 0x86, 0x57, 0x8e, 0xeb, 0xe9, 0x0e, 0xf7, 0xa1, 0x20, 0x2e};
  uint32_t count     = 0xc675a64b;
  uint8_t  bearer    = 0x0c;
  uint8_t  direction = 1;
  uint32_t len_bits = 1245, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {0x8d, 0xaa, 0x17, 0xb1, 0xae, 0x05, 0x05, 0x29, 0xc6, 0x82, 0x7f, 0x28, 0xc0, 0xef, 0x6a, 0x12,
                   0x42, 0xe9, 0x3f, 0x8b, 0x31, 0x4f, 0xb1, 0x8a, 0x77, 0xf7, 0x90, 0xae, 0x04, 0x9f, 0xed, 0xd6,
                   0x12, 0x26, 0x7f, 0xec, 0xae, 0xfc, 0x45, 0x01, 0x74, 0xd7, 0x6d, 0x9f, 0x9a, 0xa7, 0x75, 0x5a,
                   0x30, 0xcd, 0x90, 0xa9, 0xa5, 0x87, 0x4b, 0xf4, 0x8e, 0xaf, 0x70, 0xee, 0xa3, 0xa6, 0x2a, 0x25,
                   0x0a, 0x8b, 0x6b, 0xd8, 0xd9, 0xb0, 0x8b, 0x08, 0xd6, 0x4e, 0x32, 0xd1, 0x81, 0x77, 0x77, 0xfb,
                   0x54, 0x4d, 0x49, 0xcd, 0x49, 0x72, 0x0e, 0x21, 0x9d, 0xbf, 0x8b, 0xbe, 0xd3, 0x39, 0x04, 0xe1,
                   0xfd, 0x40, 0xa4, 0x1d, 0x37, 0x0a, 0x1f, 0x65, 0x74, 0x50, 0x95, 0x68, 0x7d, 0x47, 0xba, 0x1d,
                   0x36, 0xd2, 0x34, 0x9e, 0x23, 0xf6, 0x44, 0x39, 0x2c, 0x8e, 0xa9, 0xc4, 0x9d, 0x40, 0xc1, 0x32,
                   0x71, 0xaf, 0xf2, 0x64, 0xd0, 0xf2, 0x48, 0x41, 0xd6, 0x46, 0x5f, 0x09, 0x96, 0xff, 0x84, 0xe6,
                   0x5f, 0xc5, 0x17, 0xc5, 0x3e, 0xfc, 0x33, 0x63, 0xc3, 0x84, 0x92, 0xa8};
  uint8_t  ct[]  = {0x6c, 0xdb, 0x18, 0xa7, 0xca, 0x82, 0x18, 0xe8, 0x6e, 0x4b, 0x4b, 0x71, 0x6a, 0x4d, 0x04, 0x37,
                  0x1f, 0xbe, 0xc2, 0x62, 0xfc, 0x5a, 0xd0, 0xb3, 0x81, 0x9b, 0x18, 0x7b, 0x97, 0xe5, 0x5b, 0x1a,
                  0x4d, 0x7c, 0x19, 0xee, 0x24, 0xc8, 0xb4, 0xd7, 0x72, 0x3c, 0xfe, 0xdf, 0x04, 0x5b, 0x8a, 0xca,
                  0xe4, 0x86, 0x95, 0x17, 0xd8, 0x0e, 0x50, 0x61, 0x5d, 0x90, 0x35, 0xd5, 0xd9, 0xc5, 0xa4, 0x0a,
                  0xf6, 0x02, 0x28, 0x0b, 0x54, 0x25, 0x97, 0xb0, 0xcb, 0x18, 0x61, 0x9e, 0xeb, 0x35, 0x92, 0x57,
                  0x59, 0xd1, 0x95, 0xe1, 0x00, 0xe8, 0xe4, 0xaa, 0x0c, 0x38, 0xa3, 0xc2, 0xab, 0xe0, 0xf3, 0xd8,
                  0xff, 0x04, 0xf3, 0xc3, 0x3c, 0x29, 0x50, 0x69, 0xc2, 0x36, 0x94, 0xb5, 0xbb, 0xea, 0xcd, 0xd5,
                  0x42, 0xe2, 0x8e, 0x8a, 0x94, 0xed, 0xb9, 0x11, 0x9f, 0x41, 0x2d, 0x05, 0x4b, 0xe1, 0xfa, 0x72,
                  0x72, 0xb5, 0xff, 0xb2, 0xb2, 0x57, 0x0f, 0x4f, 0x7c, 0xea, 0xf3, 0x83, 0xa8, 0xa9, 0xd9, 0x35,
                  0x72, 0xf0, 0x4d, 0x6e, 0x3a, 0x6e, 0x29, 0x37, 0x26, 0xec, 0x62, 0xc8};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

int test_set_6()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0x54, 0xf4, 0xe2, 0xe0, 0x4c, 0x83, 0x78, 0x6e, 0xec, 0x8f, 0xb5, 0xab, 0xe8, 0xe3, 0x65, 0x66};
  uint32_t count     = 0xaca4f50f;
  uint8_t  bearer    = 0x0b;
  uint8_t  direction = 0;
  uint32_t len_bits = 3861, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {
      0x40, 0x98, 0x1b, 0xa6, 0x82, 0x4c, 0x1b, 0xfb, 0x42, 0x86, 0xb2, 0x99, 0x78, 0x3d, 0xaf, 0x44, 0x2c, 0x09, 0x9f,
      0x7a, 0xb0, 0xf5, 0x8d, 0x5c, 0x8e, 0x46, 0xb1, 0x04, 0xf0, 0x8f, 0x01, 0xb4, 0x1a, 0xb4, 0x85, 0x47, 0x20, 0x29,
      0xb7, 0x1d, 0x36, 0xbd, 0x1a, 0x3d, 0x90, 0xdc, 0x3a, 0x41, 0xb4, 0x6d, 0x51, 0x67, 0x2a, 0xc4, 0xc9, 0x66, 0x3a,
      0x2b, 0xe0, 0x63, 0xda, 0x4b, 0xc8, 0xd2, 0x80, 0x8c, 0xe3, 0x3e, 0x2c, 0xcc, 0xbf, 0xc6, 0x34, 0xe1, 0xb2, 0x59,
      0x06, 0x08, 0x76, 0xa0, 0xfb, 0xb5, 0xa4, 0x37, 0xeb, 0xcc, 0x8d, 0x31, 0xc1, 0x9e, 0x44, 0x54, 0x31, 0x87, 0x45,
      0xe3, 0xfa, 0x16, 0xbb, 0x11, 0xad, 0xae, 0x24, 0x88, 0x79, 0xfe, 0x52, 0xdb, 0x25, 0x43, 0xe5, 0x3c, 0xf4, 0x45,
      0xd3, 0xd8, 0x28, 0xce, 0x0b, 0xf5, 0xc5, 0x60, 0x59, 0x3d, 0x97, 0x27, 0x8a, 0x59, 0x76, 0x2d, 0xd0, 0xc2, 0xc9,
      0xcd, 0x68, 0xd4, 0x49, 0x6a, 0x79, 0x25, 0x08, 0x61, 0x40, 0x14, 0xb1, 0x3b, 0x6a, 0xa5, 0x11, 0x28, 0xc1, 0x8c,
      0xd6, 0xa9, 0x0b, 0x87, 0x97, 0x8c, 0x2f, 0xf1, 0xca, 0xbe, 0x7d, 0x9f, 0x89, 0x8a, 0x41, 0x1b, 0xfd, 0xb8, 0x4f,
      0x68, 0xf6, 0x72, 0x7b, 0x14, 0x99, 0xcd, 0xd3, 0x0d, 0xf0, 0x44, 0x3a, 0xb4, 0xa6, 0x66, 0x53, 0x33, 0x0b, 0xcb,
      0xa1, 0x10, 0x5e, 0x4c, 0xec, 0x03, 0x4c, 0x73, 0xe6, 0x05, 0xb4, 0x31, 0x0e, 0xaa, 0xad, 0xcf, 0xd5, 0xb0, 0xca,
      0x27, 0xff, 0xd8, 0x9d, 0x14, 0x4d, 0xf4, 0x79, 0x27, 0x59, 0x42, 0x7c, 0x9c, 0xc1, 0xf8, 0xcd, 0x8c, 0x87, 0x20,
      0x23, 0x64, 0xb8, 0xa6, 0x87, 0x95, 0x4c, 0xb0, 0x5a, 0x8d, 0x4e, 0x2d, 0x99, 0xe7, 0x3d, 0xb1, 0x60, 0xde, 0xb1,
      0x80, 0xad, 0x08, 0x41, 0xe9, 0x67, 0x41, 0xa5, 0xd5, 0x9f, 0xe4, 0x18, 0x9f, 0x15, 0x42, 0x00, 0x26, 0xfe, 0x4c,
      0xd1, 0x21, 0x04, 0x93, 0x2f, 0xb3, 0x8f, 0x73, 0x53, 0x40, 0x43, 0x8a, 0xaf, 0x7e, 0xca, 0x6f, 0xd5, 0xcf, 0xd3,
      0xa1, 0x95, 0xce, 0x5a, 0xbe, 0x65, 0x27, 0x2a, 0xf6, 0x07, 0xad, 0xa1, 0xbe, 0x65, 0xa6, 0xb4, 0xc9, 0xc0, 0x69,
      0x32, 0x34, 0x09, 0x2c, 0x4d, 0x01, 0x8f, 0x17, 0x56, 0xc6, 0xdb, 0x9d, 0xc8, 0xa6, 0xd8, 0x0b, 0x88, 0x81, 0x38,
      0x61, 0x6b, 0x68, 0x12, 0x62, 0xf9, 0x54, 0xd0, 0xe7, 0x71, 0x17, 0x48, 0x78, 0x0d, 0x92, 0x29, 0x1d, 0x86, 0x29,
      0x99, 0x72, 0xdb, 0x74, 0x1c, 0xfa, 0x4f, 0x37, 0xb8, 0xb5, 0x6c, 0xdb, 0x18, 0xa7, 0xca, 0x82, 0x18, 0xe8, 0x6e,
      0x4b, 0x4b, 0x71, 0x6a, 0x4d, 0x04, 0x37, 0x1f, 0xbe, 0xc2, 0x62, 0xfc, 0x5a, 0xd0, 0xb3, 0x81, 0x9b, 0x18, 0x7b,
      0x97, 0xe5, 0x5b, 0x1a, 0x4d, 0x7c, 0x19, 0xee, 0x24, 0xc8, 0xb4, 0xd7, 0x72, 0x3c, 0xfe, 0xdf, 0x04, 0x5b, 0x8a,
      0xca, 0xe4, 0x86, 0x95, 0x17, 0xd8, 0x0e, 0x50, 0x61, 0x5d, 0x90, 0x35, 0xd5, 0xd9, 0xc5, 0xa4, 0x0a, 0xf6, 0x02,
      0x28, 0x0b, 0x54, 0x25, 0x97, 0xb0, 0xcb, 0x18, 0x61, 0x9e, 0xeb, 0x35, 0x92, 0x57, 0x59, 0xd1, 0x95, 0xe1, 0x00,
      0xe8, 0xe4, 0xaa, 0x0c, 0x38, 0xa3, 0xc2, 0xab, 0xe0, 0xf3, 0xd8, 0xff, 0x04, 0xf3, 0xc3, 0x3c, 0x29, 0x50, 0x69,
      0xc2, 0x36, 0x94, 0xb5, 0xbb, 0xea, 0xcd, 0xd5, 0x42, 0xe2, 0x8e, 0x8a, 0x94, 0xed, 0xb9, 0x11, 0x9f, 0x41, 0x2d,
      0x05, 0x4b, 0xe1, 0xfa, 0x72, 0xb0, 0x95, 0x50};
  uint8_t ct[] = {
      0x35, 0x1e, 0x30, 0xd4, 0xd9, 0x10, 0xc5, 0xdd, 0x5a, 0xd7, 0x83, 0x4c, 0x42, 0x6e, 0x6c, 0x0c, 0xab, 0x64, 0x86,
      0xda, 0x7b, 0x0f, 0xda, 0x4c, 0xd8, 0x3a, 0xf1, 0xb9, 0x64, 0x71, 0x37, 0xf1, 0xac, 0x43, 0xb4, 0x34, 0x22, 0x3b,
      0x19, 0xbe, 0x07, 0xbd, 0x89, 0xd1, 0xcc, 0x30, 0x69, 0x44, 0xd3, 0x36, 0x1e, 0xa1, 0xa2, 0xf8, 0xcd, 0xbd, 0x32,
      0x16, 0x55, 0x97, 0x63, 0x50, 0xd0, 0x0b, 0x80, 0xdd, 0x83, 0x81, 0x20, 0xa7, 0x75, 0x5c, 0x6d, 0xea, 0x2a, 0xb2,
      0xb0, 0xc9, 0x9a, 0x91, 0x3f, 0x47, 0xda, 0xe2, 0xb8, 0xde, 0xb9, 0xa8, 0x29, 0xe5, 0x46, 0x9f, 0xf2, 0xe1, 0x87,
      0x77, 0x6f, 0x6f, 0xd0, 0x81, 0xe3, 0x87, 0x1d, 0x11, 0x9a, 0x76, 0xe2, 0x4c, 0x91, 0x7e, 0xa6, 0x26, 0x48, 0xe0,
      0x2e, 0x90, 0x36, 0x75, 0x64, 0xde, 0x72, 0xae, 0x7e, 0x4f, 0x0a, 0x42, 0x49, 0xa9, 0xa5, 0xb0, 0xe4, 0x65, 0xa2,
      0xd6, 0xd9, 0xdc, 0x87, 0x84, 0x3b, 0x1b, 0x87, 0x5c, 0xc9, 0xa3, 0xbe, 0x93, 0xd8, 0xda, 0x8f, 0x56, 0xec, 0xaf,
      0x59, 0x81, 0xfe, 0x93, 0xc2, 0x84, 0x31, 0x8b, 0x0d, 0xec, 0x7a, 0x3b, 0xa1, 0x08, 0xe2, 0xcb, 0x1a, 0x61, 0xe9,
      0x66, 0xfa, 0x7a, 0xfa, 0x7a, 0xc7, 0xf6, 0x7f, 0x65, 0xbc, 0x4a, 0x2d, 0xf0, 0x70, 0xd4, 0xe4, 0x34, 0x84, 0x5f,
      0x10, 0x9a, 0xb2, 0xb6, 0x8a, 0xde, 0x3d, 0xc3, 0x16, 0xca, 0x63, 0x32, 0xa6, 0x28, 0x93, 0xe0, 0xa7, 0xec, 0x0b,
      0x4f, 0xc2, 0x51, 0x91, 0xbf, 0x2f, 0xf1, 0xb9, 0xf9, 0x81, 0x5e, 0x4b, 0xa8, 0xa9, 0x9c, 0x64, 0x3b, 0x52, 0x18,
      0x04, 0xf7, 0xd5, 0x85, 0x0d, 0xde, 0x39, 0x52, 0x20, 0x6e, 0xc6, 0xcc, 0xf3, 0x40, 0xf9, 0xb3, 0x22, 0x0b, 0x30,
      0x23, 0xbd, 0xd0, 0x63, 0x95, 0x6e, 0xa8, 0x33, 0x39, 0x20, 0xfd, 0xe9, 0x9e, 0x06, 0x75, 0x41, 0x0e, 0x49, 0xef,
      0x3b, 0x4d, 0x3f, 0xb3, 0xdf, 0x51, 0x92, 0xf9, 0x9c, 0xa8, 0x3d, 0x3b, 0x00, 0x32, 0xde, 0x08, 0xc2, 0x20, 0x77,
      0x6a, 0x58, 0x65, 0xb0, 0xe4, 0xb3, 0xb0, 0xc7, 0x5d, 0xef, 0xe7, 0x76, 0x2d, 0xff, 0x01, 0x8e, 0xa7, 0xf5, 0xbe,
      0x2b, 0x2f, 0x97, 0x2b, 0x2a, 0x8b, 0xa5, 0x97, 0x0e, 0x43, 0xbd, 0x6f, 0xdd, 0x63, 0xda, 0xe6, 0x29, 0x78, 0x4e,
      0xc4, 0x8d, 0x61, 0x00, 0x54, 0xee, 0x4e, 0x4b, 0x5d, 0xbb, 0xf1, 0xfc, 0x2f, 0xa0, 0xb8, 0x30, 0xe9, 0x4d, 0xcb,
      0xb7, 0x01, 0x4e, 0x8a, 0xb4, 0x29, 0xab, 0x10, 0x0f, 0xc4, 0x8f, 0x83, 0x17, 0x1d, 0x99, 0xfc, 0x25, 0x8b, 0x7c,
      0x2b, 0xa7, 0xc1, 0x76, 0xea, 0xea, 0xad, 0x37, 0xf8, 0x60, 0xd5, 0x97, 0xa3, 0x1c, 0xe7, 0x9b, 0x59, 0x47, 0x33,
      0xc7, 0x14, 0x1d, 0xf7, 0x91, 0x51, 0xfc, 0xa9, 0x0c, 0x08, 0x47, 0x8a, 0x5c, 0x6c, 0x2c, 0xc4, 0x81, 0xd5, 0x1f,
      0xfe, 0xce, 0x3c, 0xd7, 0xd2, 0x58, 0x13, 0x48, 0x82, 0x7a, 0x71, 0xf0, 0x91, 0x42, 0x8e, 0xbe, 0x38, 0xc9, 0x5a,
      0x3f, 0x5c, 0x63, 0xe0, 0x56, 0xdf, 0xb7, 0xcc, 0x45, 0xa9, 0xb7, 0xc0, 0x7d, 0x83, 0x4e, 0x7b, 0x20, 0xb9, 0x9e,
      0xd2, 0x02, 0x42, 0x9c, 0x14, 0xbb, 0x85, 0xff, 0xa4, 0x3b, 0x7c, 0xb6, 0x84, 0x95, 0xcd, 0x75, 0xab, 0x66, 0xd9,
      0x64, 0xd4, 0xca, 0xfe, 0x64, 0xdd, 0x94, 0x04, 0xda, 0xe2, 0xdc, 0x51, 0x10, 0x61, 0x7f, 0x19, 0x4f, 0xc3, 0xc1,
      0x84, 0xf5, 0x83, 0xcd, 0x0d, 0xef, 0x6d, 0x00};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  struct timeval t[3];

  // encryption
  gettimeofday(&t[1], NULL);
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  gettimeofday(&t[2], NULL);
  get_time_interval(t);
  printf(
      "encryption: %u bits, t=%d us, rate=%.1f Mbps/s\n", len_bits, (int)t[0].tv_usec, (float)len_bits / t[0].tv_usec);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  gettimeofday(&t[1], NULL);
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  gettimeofday(&t[2], NULL);
  get_time_interval(t);
  printf(
      "decryption: %u bits, t=%d us, rate=%.1f Mbps/s\n", len_bits, (int)t[0].tv_usec, (float)len_bits / t[0].tv_usec);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

// set len_bitsgth to multiple of 8 respectively 128
int test_set_1_block_size()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0xd3, 0xc5, 0xd5, 0x92, 0x32, 0x7f, 0xb1, 0x1c, 0x40, 0x35, 0xc6, 0x68, 0x0a, 0xf8, 0xc6, 0xd1};
  uint32_t count     = 0x398a59b4;
  uint8_t  bearer    = 0x15;
  uint8_t  direction = 1;
  uint32_t len_bits = 256, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {0x98, 0x1b, 0xa6, 0x82, 0x4c, 0x1b, 0xfb, 0x1a, 0xb4, 0x85, 0x47, 0x20, 0x29, 0xb7, 0x1d, 0x80,
                   0x8c, 0xe3, 0x3e, 0x2c, 0xc3, 0xc0, 0xb5, 0xfc, 0x1f, 0x3d, 0xe8, 0xa6, 0xdc, 0x66, 0xb1, 0xf0};
  uint8_t  ct[]  = {0x5d, 0x5b, 0xfe, 0x75, 0xeb, 0x04, 0xf6, 0x8c, 0xe0, 0xa1, 0x23, 0x77, 0xea, 0x00, 0xb3, 0x7d,
                  0x47, 0xc6, 0xa0, 0xba, 0x06, 0x30, 0x91, 0x55, 0x08, 0x6a, 0x85, 0x9c, 0x43, 0x41, 0xb3, 0x7c};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp == 0);

  free(out);
  return SRSRAN_SUCCESS;
}

// inserted bit flip in msg[0]
int test_set_1_invalid()
{
  LIBLTE_ERROR_ENUM err_lte = LIBLTE_ERROR_INVALID_INPUTS;
  int32             err_cmp = 0;

  uint8_t  key[]     = {0xd3, 0xc5, 0xd5, 0x92, 0x32, 0x7f, 0xb1, 0x1c, 0x40, 0x35, 0xc6, 0x68, 0x0a, 0xf8, 0xc6, 0xd1};
  uint32_t count     = 0x398a59b4;
  uint8_t  bearer    = 0x15;
  uint8_t  direction = 1;
  uint32_t len_bits = 253, len_bytes = (len_bits + 7) / 8;
  uint8_t  msg[] = {0x99, 0x1b, 0xa6, 0x82, 0x4c, 0x1b, 0xfb, 0x1a, 0xb4, 0x85, 0x47, 0x20, 0x29, 0xb7, 0x1d, 0x80,
                   0x8c, 0xe3, 0x3e, 0x2c, 0xc3, 0xc0, 0xb5, 0xfc, 0x1f, 0x3d, 0xe8, 0xa6, 0xdc, 0x66, 0xb1, 0xf0};
  uint8_t  ct[]  = {0x5d, 0x5b, 0xfe, 0x75, 0xeb, 0x04, 0xf6, 0x8c, 0xe0, 0xa1, 0x23, 0x77, 0xea, 0x00, 0xb3, 0x7d,
                  0x47, 0xc6, 0xa0, 0xba, 0x06, 0x30, 0x91, 0x55, 0x08, 0x6a, 0x85, 0x9c, 0x43, 0x41, 0xb3, 0x78};

  uint8_t* out = (uint8_t*)calloc(len_bytes, sizeof(uint8_t));

  // encryption
  err_lte = liblte_security_encryption_eea1(key, count, bearer, direction, msg, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(ct, out, len_bytes);
  TESTASSERT(err_cmp != 0);

  // decryption
  err_lte = liblte_security_decryption_eea1(key, count, bearer, direction, ct, len_bits, out);
  TESTASSERT(err_lte == LIBLTE_SUCCESS);

  // compare cipher text
  err_cmp = arrcmp(msg, out, len_bytes);
  TESTASSERT(err_cmp != 0);

  free(out);
  return SRSRAN_SUCCESS;
}

/*
 * Functions
 */

int main(int argc, char* argv[])
{
  TESTASSERT(test_set_1() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_2() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_3() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_4() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_5() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_6() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_1_block_size() == SRSRAN_SUCCESS);
  TESTASSERT(test_set_1_invalid() == SRSRAN_SUCCESS);
  return SRSRAN_SUCCESS;
}
