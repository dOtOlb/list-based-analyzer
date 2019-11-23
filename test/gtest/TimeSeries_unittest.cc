// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <limits.h>
#include <vector>
#include "../../src/utilities.h"
#include "../../src/Tx.h"
#include "../../src/TimeSeries.h"
#include "gtest/gtest.h"
namespace {

class TimeSeriesTestFixture: public ::testing::Test { 
protected:
   TimeSeries<Tx> ts {300};
   Tx obj;
   static const string test_log;

public: 
   TimeSeriesTestFixture( ): 
//    ts(300), 
    obj(tokenize(TimeSeriesTestFixture::test_log)) { 
   		
       // initialization code here
              
       ts.add(obj.timestamp, obj);       
   } 
 
   void SetUp( ) { 
       // code here will execute just before the test ensues 
   }
 
   void TearDown( ) { 
       // code here will be called just after the test completes
       // ok to through exceptions from here if need be
   }
 
   ~TimeSeriesTestFixture( )  { 
       // cleanup any pending stuff, but no exceptions allowed
   }
 
   // put in any custom data members that you need 
};

const string TimeSeriesTestFixture::test_log {"205.197.140.134 r 1528721030.415 - 6 0 0 0 0 294 127.0.0.1 GET /L/16382/536090/1m30s/mc.viutv99-i.akamaihd.net/.redirect2./265284/265284/m3u8/live1/stream1/m3u8/streamPlaylist?file-type=meta&pt-delivery-format-qs=hls 200 - meV - ipa.viutv99-i.akamaihd.net - - - - - 1 - 430 - - 0 0 - 169f27d 16382. - - - - - - 90 - - - - - - - - OW - - 464 TLj - eaa2a940 - - - - 127.0.0.1 646 - - - - - - - - - mtse=g|303538177;aid=348946;pin=TL;netp=w - - - 1891:12151:0:0 NS:NS:i:NS:: - - - - -"};

// Tests TimeSeries<Tx>.

// Tests time series for its existence-checking function.
TEST_F(TimeSeriesTestFixture, Existence) {
  // This test is named "Existence", and belongs to the "TimeSeriesTestFixture"
  // test case.
  ASSERT_TRUE(ts.exists(obj.timestamp, obj));

}

}  // namespace

