/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nss.h"
#include "pk11pub.h"
#include <memory>

#include "gtest/gtest.h"
#include "scoped_ptrs.h"

namespace nss_test {

// Test vectors from https://tools.ietf.org/html/rfc3394#section-4.1 to 4.6
unsigned char kKEK1[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

unsigned char kKD1[] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};

unsigned char kC1[] = {
  0x1F, 0xA6, 0x8B, 0x0A, 0x81, 0x12, 0xB4, 0x47,
  0xAE, 0xF3, 0x4B, 0xD8, 0xFB, 0x5A, 0x7B, 0x82,
  0x9D, 0x3E, 0x86, 0x23, 0x71, 0xD2, 0xCF, 0xE5
};

unsigned char kKEK2[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
};

unsigned char kC2[] = {
  0x96, 0x77, 0x8B, 0x25, 0xAE, 0x6C, 0xA4, 0x35,
  0xF9, 0x2B, 0x5B, 0x97, 0xC0, 0x50, 0xAE, 0xD2,
  0x46, 0x8A, 0xB8, 0xA1, 0x7A, 0xD8, 0x4E, 0x5D
};

unsigned char kKEK3[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

unsigned char kC3[] = {
  0x64, 0xE8, 0xC3, 0xF9, 0xCE, 0x0F, 0x5B, 0xA2,
  0x63, 0xE9, 0x77, 0x79, 0x05, 0x81, 0x8A, 0x2A,
  0x93, 0xC8, 0x19, 0x1E, 0x7D, 0x6E, 0x8A, 0xE7
};

unsigned char kKD4[] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
};

unsigned char kC4[] = {
  0x03, 0x1D, 0x33, 0x26, 0x4E, 0x15, 0xD3, 0x32,
  0x68, 0xF2, 0x4E, 0xC2, 0x60, 0x74, 0x3E, 0xDC,
  0xE1, 0xC6, 0xC7, 0xDD, 0xEE, 0x72, 0x5A, 0x93,
  0x6B, 0xA8, 0x14, 0x91, 0x5C, 0x67, 0x62, 0xD2
};

unsigned char kC5[] = {
  0xA8, 0xF9, 0xBC, 0x16, 0x12, 0xC6, 0x8B, 0x3F,
  0xF6, 0xE6, 0xF4, 0xFB, 0xE3, 0x0E, 0x71, 0xE4,
  0x76, 0x9C, 0x8B, 0x80, 0xA3, 0x2C, 0xB8, 0x95,
  0x8C, 0xD5, 0xD1, 0x7D, 0x6B, 0x25, 0x4D, 0xA1
};

unsigned char kKD6[] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

unsigned char kC6[] = {
  0x28, 0xC9, 0xF4, 0x04, 0xC4, 0xB8, 0x10, 0xF4,
  0xCB, 0xCC, 0xB3, 0x5C, 0xFB, 0x87, 0xF8, 0x26,
  0x3F, 0x57, 0x86, 0xE2, 0xD8, 0x0E, 0xD3, 0x26,
  0xCB, 0xC7, 0xF0, 0xE7, 0x1A, 0x99, 0xF4, 0x3B,
  0xFB, 0x98, 0x8B, 0x9B, 0x7A, 0x02, 0xDD, 0x21
};

class Pkcs11AESKeyWrapTest : public ::testing::Test {
 protected:
  CK_MECHANISM_TYPE mechanism = CKM_NSS_AES_KEY_WRAP;

  void
  WrapUnwrap(unsigned char* kek, unsigned int kekLen, unsigned char* keyData,
             unsigned int keyDataLen, unsigned char* expectedCiphertext)
  {
    unsigned char wrappedKey[40];
    unsigned int wrappedKeyLen;
    unsigned char unwrappedKey[40];
    unsigned int unwrappedKeyLen = 0;
    SECStatus rv;

    ScopedPK11SlotInfo slot(PK11_GetInternalSlot());
    ASSERT_NE(nullptr, slot);

    // Import encryption key.
    SECItem keyItem = { siBuffer, kek, kekLen };
    ScopedPK11SymKey encryptionKey(PK11_ImportSymKey(slot.get(), CKM_NSS_AES_KEY_WRAP,
                                                     PK11_OriginUnwrap, CKA_ENCRYPT,
                                                     &keyItem, nullptr));
    EXPECT_TRUE(!!encryptionKey);

    // Wrap key
    rv = PK11_Encrypt(encryptionKey.get(), mechanism, nullptr /* param */,
                      wrappedKey, &wrappedKeyLen, sizeof(wrappedKey),
                      keyData, keyDataLen);
    EXPECT_EQ(rv, SECSuccess) << "CKM_NSS_AES_KEY_WRAP encrypt failed";
    EXPECT_TRUE(!memcmp(expectedCiphertext, wrappedKey, wrappedKeyLen));

    // Unwrap key
    rv = PK11_Decrypt(encryptionKey.get(), mechanism, nullptr /* param */,
                      unwrappedKey, &unwrappedKeyLen, sizeof(unwrappedKey),
                      wrappedKey, wrappedKeyLen);
    EXPECT_EQ(rv, SECSuccess) << " CKM_NSS_AES_KEY_WRAP decrypt failed\n";
    EXPECT_TRUE(!memcmp(keyData, unwrappedKey, unwrappedKeyLen));
  }
};

TEST_F(Pkcs11AESKeyWrapTest, WrapUnwrepTest1) {
  WrapUnwrap(kKEK1, sizeof(kKEK1), kKD1, sizeof(kKD1), kC1);
}

TEST_F(Pkcs11AESKeyWrapTest, WrapUnwrepTest2) {
  WrapUnwrap(kKEK2, sizeof(kKEK2), kKD1, sizeof(kKD1), kC2);
}

TEST_F(Pkcs11AESKeyWrapTest, WrapUnwrepTest3) {
  WrapUnwrap(kKEK3, sizeof(kKEK3), kKD1, sizeof(kKD1), kC3);
}

TEST_F(Pkcs11AESKeyWrapTest, WrapUnwrepTest4) {
  WrapUnwrap(kKEK2, sizeof(kKEK2), kKD4, sizeof(kKD4), kC4);
}

TEST_F(Pkcs11AESKeyWrapTest, WrapUnwrepTest5) {
  WrapUnwrap(kKEK3, sizeof(kKEK3), kKD4, sizeof(kKD4), kC5);
}

TEST_F(Pkcs11AESKeyWrapTest, WrapUnwrepTest6) {
  WrapUnwrap(kKEK3, sizeof(kKEK3), kKD6, sizeof(kKD6), kC6);
}

} /* nss_test */