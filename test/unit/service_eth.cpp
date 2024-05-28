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
    vcml::silkit::participant part0;
    vcml::silkit::participant part1;

    vcml::silkit::service_eth service0;
    vcml::silkit::service_eth service1;

    eth_initiator_array eth_array_tx;
    eth_target_array eth_array_rx;

    test_bench(const sc_module_name& nm):
        test_base(nm),
        part0("participant0"),
        part1("participant1"),
        service0("service_eth0", part0),
        service1("service_eth1", part1),
        eth_array_tx("eth_array_tx"),
        eth_array_rx("eth_array_rx") {
        eth_bind(*this, "eth_array_tx", 0, service0, "eth_rx");
        eth_bind(*this, "eth_array_tx", 1, service1, "eth_rx");

        eth_bind(*this, "eth_array_rx", 0, service0, "eth_tx");
        eth_bind(*this, "eth_array_rx", 1, service1, "eth_tx");

        SC_HAS_PROCESS(test_bench);
        SC_THREAD(tick);
    }

    MOCK_METHOD(void, eth_receive,
                (const eth_target_socket&, const eth_frame&), (override));

    MOCK_METHOD(void, eth_link_up, (), (override));
    MOCK_METHOD(void, eth_link_down, (), (override));

    void tick() {
        while (true) {
            wait(SC_ZERO_TIME);
        }
    }

    virtual void run_test() override {
        wait(SC_ZERO_TIME);

        stringstream ss;
        vector<u8> data = { 0x11, 0x22, 0x33, 0x44 };
        eth_frame frame("ff:ff:ff:ff:ff:ff", "12:23:34:45:56:67", data);

        sc_event ev;
        EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_array_rx[1]),
                                       eth_match_frame(frame)))
            .WillOnce([&] { ev.notify(); });
        eth_array_tx[0].send(frame);
        wait(ev);

        EXPECT_CALL(*this, eth_link_down());
        EXPECT_TRUE(execute("link_down", {}, ss));
        EXPECT_EQ(ss.str(), "");

        EXPECT_CALL(*this, eth_receive(_, _)).Times(0);
        eth_array_tx[0].send(frame);

        EXPECT_CALL(*this, eth_link_up()).Times(1);
        EXPECT_TRUE(execute("link_up", {}, ss));
        EXPECT_TRUE(execute("link_up", {}, ss)); // should not trigger
        EXPECT_EQ(ss.str(), "");

        EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_array_rx[1]),
                                       eth_match_frame(frame)))
            .WillOnce([&] { ev.notify(); });
        eth_array_tx[0].send(frame);
        wait(ev);

        EXPECT_CALL(*this, eth_receive(eth_match_socket(&eth_array_rx[0]),
                                       eth_match_frame(frame)))
            .WillOnce([&] { ev.notify(); });
        eth_array_tx[1].send(frame);
        wait(ev);
    }
};

TEST(participant, simulate) {
    test_bench bench("system");
    sc_core::sc_start();
}
