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

MATCHER_P(eth_match_socket, socket, "Matches an ethernet socket") {
    return &arg == socket;
}

MATCHER_P(eth_match_frame, frame, "Matches an ethernet frame") {
    return arg == frame;
}

class test_bench : public test_base, public eth_host
{
public:
    vcml::silkit::participant part;

    vcml::silkit::service_eth service;

    eth_initiator_socket eth_tx;
    eth_target_socket eth_rx;

    test_bench(const sc_module_name& nm):
        test_base(nm),
        part("participant"),
        service(mkstr("service_eth_%s", part.name.get().c_str()).c_str(),
                part),
        eth_tx("eth_tx"),
        eth_rx("eth_rx") {
        eth_bind(*this, "eth_tx", service, "eth_rx");
        eth_bind(*this, "eth_rx", service, "eth_tx");

        SC_HAS_PROCESS(test_bench);
        SC_THREAD(tick);

        EXPECT_STREQ(service.kind(), "vcml::silkit::service_eth");
    }

    MOCK_METHOD(void, eth_receive,
                (const eth_target_socket&, const eth_frame&), (override));

    MOCK_METHOD(void, eth_link_up, (), (override));
    MOCK_METHOD(void, eth_link_down, (), (override));

    void tick() {
        while (true)
            wait(SC_ZERO_TIME);
    }

    virtual void run_test() override {
        wait(SC_ZERO_TIME);

        stringstream ss;
        vector<u8> data = { 0x11, 0x22, 0x33, 0x44 };
        eth_frame frame("ff:ff:ff:ff:ff:ff", "12:23:34:45:56:67", data);
        sc_event ev;

        // in autonomous mode wait for everyone to get ready
        usleep(10000);

        if (part.name == "vcml_participant0") {
            eth_tx.send(frame);
            EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_rx),
                                           eth_match_frame(frame)))
                .WillOnce([&] { ev.notify(); });
            wait(ev);

            EXPECT_CALL(*this, eth_link_down());
            EXPECT_TRUE(execute("link_down", {}, ss));
            EXPECT_EQ(ss.str(), "");

            eth_tx.send(frame);

            EXPECT_CALL(*this, eth_link_up()).Times(1);
            EXPECT_TRUE(execute("link_up", {}, ss));
            EXPECT_TRUE(execute("link_up", {}, ss)); // should not trigger
            EXPECT_EQ(ss.str(), "");

            eth_tx.send(frame);
            EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_rx),
                                           eth_match_frame(frame)))
                .WillOnce([&] { ev.notify(); });
            wait(ev);
        } else {
            EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_rx),
                                           eth_match_frame(frame)))
                .WillOnce([&] { ev.notify(); });
            wait(ev);
            eth_tx.send(frame);

            EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_rx),
                                           eth_match_frame(frame)))
                .WillOnce([&] { ev.notify(); });
            wait(ev);
            eth_tx.send(frame);
        }
    }
};

TEST(participant, simulate) {
    test_bench bench("system");
    sc_core::sc_start();
}
