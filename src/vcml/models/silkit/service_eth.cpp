/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#include "silkit/SilKit.hpp"
#include "silkit/services/all.hpp"

#include "vcml/models/silkit/service_eth.h"

using namespace SilKit::Services::Ethernet;

namespace vcml {
namespace silkit {

void service_eth::send_to_host(const eth_frame& frame) {
    m_eth_controller->SendFrame(EthernetFrame{ SilKit::Util::ToSpan(frame) });
}

void service_eth::send_to_guest(eth_frame frame) {
    lock_guard<mutex> guard(m_mtx);
    m_rx.push(std::move(frame));
    on_next_update([&]() -> void { m_ev.notify(SC_ZERO_TIME); });
}

void service_eth::eth_receive(const eth_frame& frame) {
    send_to_host(frame);
}

void service_eth::eth_transmit() {
    while (true) {
        wait(m_ev);

        lock_guard<mutex> guard(m_mtx);
        while (!m_rx.empty()) {
            eth_frame frame = std::move(m_rx.front());
            m_rx.pop();
            eth_tx.send(frame);
        }
    }
}

service_eth::service_eth(const sc_module_name& nm, participant& part):
    service(part, nm),
    eth_host(),
    m_mtx(),
    m_rx(),
    m_ev("rxev"),
    controller_name("controller_name", "vcml_default_controller"),
    network_name("network_name", "vcml_default_network"),
    eth_tx("eth_tx"),
    eth_rx("eth_rx") {
    SC_HAS_PROCESS(service_eth);
    SC_THREAD(eth_transmit);
    m_eth_controller = part.get_silkit_part()->CreateEthernetController(
        controller_name, network_name);

    IEthernetController::FrameHandler frameHandler =
        [this](IEthernetController*, const EthernetFrameEvent& msg) {
            send_to_guest(SilKit::Util::ToStdVector(msg.frame.raw));
        };

    m_eth_controller->AddFrameHandler(frameHandler);
    m_eth_controller->Activate();
}

service_eth::~service_eth() {
    m_eth_controller->Deactivate();
}

} // namespace silkit
} // namespace vcml
