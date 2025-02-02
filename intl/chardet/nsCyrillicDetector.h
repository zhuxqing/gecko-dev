/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef nsCyrillicDetector_h__
#define nsCyrillicDetector_h__

#include "nsCyrillicClass.h"
#include "nsIStringCharsetDetector.h"

static const uint8_t *gCyrillicCls[5] =
{
   CP1251Map,
   KOI8Map,
   ISO88595Map,
   MacCyrillicMap,
   IBM866Map
};

static const char * gRussian[5] = {
  "windows-1251",
  "KOI8-R",
  "ISO-8859-5",
  "x-mac-cyrillic",
  "IBM866"
};

static const char * gUkrainian[5] = {
  "windows-1251",
  "KOI8-U",
  "ISO-8859-5",
  "x-mac-cyrillic",
  "IBM866"
};

#define NUM_CYR_CHARSET 5

class nsCyrillicDetector
{
  public:
    nsCyrillicDetector(uint8_t aItems,
                      const uint8_t ** aCyrillicClass,
                      const char **aCharsets) {
      mItems = aItems;
      mCyrillicClass = aCyrillicClass;
      mCharsets = aCharsets;
      for(unsigned i=0;i<mItems;i++)
        mProb[i] = mLastCls[i] =0;
      mDone = false;
    }
    virtual ~nsCyrillicDetector() {}
    virtual void HandleData(const char* aBuf, uint32_t aLen);
    virtual void   DataEnd();
  protected:
    virtual void Report(const char* aCharset) = 0;
    bool    mDone;

  private:
    uint8_t  mItems;
    const uint8_t ** mCyrillicClass;
    const char** mCharsets;
    uint32_t mProb[NUM_CYR_CHARSET];
    uint8_t mLastCls[NUM_CYR_CHARSET];
};

class nsCyrXPCOMDetector :
      public nsCyrillicDetector,
      public nsICharsetDetector
{
  public:
    // nsISupports interface
    NS_DECL_ISUPPORTS
    nsCyrXPCOMDetector(uint8_t aItems,
                      const uint8_t ** aCyrillicClass,
                      const char **aCharsets);
    NS_IMETHOD Init(nsICharsetDetectionObserver* aObserver) override;
    NS_IMETHOD DoIt(const char* aBuf, uint32_t aLen, bool *oDontFeedMe) override;
    NS_IMETHOD Done() override;
  protected:
    virtual ~nsCyrXPCOMDetector();
    virtual void Report(const char* aCharset) override;
  private:
    nsCOMPtr<nsICharsetDetectionObserver> mObserver;
};

class nsRUProbDetector final : public nsCyrXPCOMDetector
{
  public:
    nsRUProbDetector()
      : nsCyrXPCOMDetector(5, gCyrillicCls, gRussian) {}
};

class nsUKProbDetector final : public nsCyrXPCOMDetector
{
  public:
    nsUKProbDetector()
      : nsCyrXPCOMDetector(5, gCyrillicCls, gUkrainian) {}
};

#endif
