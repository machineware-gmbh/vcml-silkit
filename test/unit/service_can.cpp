/******************************************************************************
 *                                                                            *
 * Copyright (C) 2025 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#include "testing.h"

MATCHER_P(can_match_socket, socket, "Matches a can socket") {
    return &arg == socket;
}

MATCHER_P(can_match_frame, frame, "Matches a can frame") {
    return arg == frame;
}

class test_bench : public test_base, public can_host
{
public:
    vcml::silkit::participant part;

    vcml::silkit::service_can service;

    can_initiator_socket can_tx;
    can_target_socket can_rx;

    test_bench(const sc_module_name& nm):
        test_base(nm),
        part("participant"),
        service(mkstr("service_can_%s", part.name.get().c_str()).c_str(),
                part),
        can_tx("can_tx"),
        can_rx("can_rx") {
        can_bind(*this, "can_tx", service, "can_rx");
        can_bind(*this, "can_rx", service, "can_tx");

        SC_HAS_PROCESS(test_bench);
        SC_THREAD(tick);

        EXPECT_STREQ(service.kind(), "vcml::silkit::service_can");
    }

    MOCK_METHOD(void, can_receive, (const can_target_socket&, can_frame&),
                (override));

    void tick() {
        while (true)
            wait(SC_ZERO_TIME);
    }

    virtual void run_test() override {
        wait(SC_ZERO_TIME);

        can_frame frame;
        frame.msgid = 0x123;
        frame.flags = 0;
        frame.data.resize(4);
        frame.data[0] = 0x11;
        frame.data[1] = 0x22;
        frame.data[2] = 0x33;
        frame.data[3] = 0x44;
        sc_event ev;

        if (part.name == "vcml_participant0") {
            // in autonomous mode wait for everyone to get ready
            mwr::sleep(1);

            can_tx.send(frame);
            EXPECT_CALL(*this, can_receive(can_match_socket(&can_rx),
                                           can_match_frame(frame)))
                .WillOnce([&] { ev.notify(); });
            wait(ev);

            can_tx.send(frame);
        } else {
            EXPECT_CALL(*this, can_receive(can_match_socket(&can_rx),
                                           can_match_frame(frame)))
                .WillOnce([&] { ev.notify(); });
            wait(ev);
            can_tx.send(frame);
        }

        // wait for everyone to finish
        mwr::sleep(1);
    }
};

TEST(participant, simulate) {
    test_bench bench("system");
    sc_core::sc_start();
}
