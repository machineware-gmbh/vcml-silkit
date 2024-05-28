/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#include "testing.h"

class test_bench : public test_base
{
public:
    vcml::silkit::participant participant;

    test_bench(const sc_module_name& nm):
        test_base(nm), participant("participant") {
        // nothing to do
    }

    virtual void run_test() override {
        // nothing to do
    }
};

TEST(participant, simulate) {
    test_bench bench("system");
    sc_core::sc_start();
}
