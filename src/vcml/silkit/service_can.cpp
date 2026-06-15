/******************************************************************************
 *                                                                            *
 * Copyright (C) 2025 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#include "vcml/silkit/service_can.h"
#include "vcml/silkit/version.h"

#include "silkit/SilKit.hpp"
#include "silkit/services/all.hpp"

namespace vcml {
namespace silkit {

using namespace SilKit::Services::Can;

static inline CanFrame to_silkit_can(const can_frame& vcml_frame) {
    CanFrame silkit_frame;
    silkit_frame.canId = vcml_frame.msgid;
    silkit_frame.flags = vcml_frame.flags;
    silkit_frame.dlc = vcml_frame.dlc;
    silkit_frame.sdt = 0;  // no xl format
    silkit_frame.vcid = 0; // no xl format
    silkit_frame.af = 0;   // no xl format
    silkit_frame.dataField = SilKit::Util::Span(vcml_frame.data,
                                                vcml_frame.length());

    return silkit_frame;
}

static inline can_frame to_vcml_can(const CanFrame& silkit_frame) {
    can_frame vcml_frame;
    vcml_frame.msgid = silkit_frame.canId;
    vcml_frame.flags = silkit_frame.flags;
    vcml_frame.dlc = silkit_frame.dlc;
    if (silkit_frame.dataField.size() > sizeof(vcml_frame.data))
        VCML_ERROR("silkit can frame does not fit in vcml can frame");
    memcpy(vcml_frame.data, silkit_frame.dataField.data(),
           silkit_frame.dataField.size());

    return vcml_frame;
}

void service_can::send_to_host(const can_frame& frame) {
    m_can_controller->SendFrame(to_silkit_can(frame));
}

void service_can::send_to_guest(can_frame frame) {
    lock_guard<mutex> guard(m_mtx);
    m_rx.push(frame);
    on_next_update([&]() -> void { m_ev.notify(SC_ZERO_TIME); });
}

void service_can::can_receive(can_frame& frame) {
    if (part().mode == SILKIT_MODE_OFF)
        return;

    send_to_host(frame);
}

void service_can::can_transmit() {
    if (part().mode == SILKIT_MODE_OFF)
        return;

    while (true) {
        wait(m_ev);

        lock_guard<mutex> guard(m_mtx);
        while (!m_rx.empty()) {
            can_frame& frame = m_rx.front();
            can_tx.send(frame);
            m_rx.pop();
        }
    }
}

service_can::service_can(const sc_module_name& nm, participant& part):
    service(nm, part, "service_can"),
    can_host(),
    m_can_controller(),
    m_mtx(),
    m_rx(),
    m_ev("rxev"),
    controller_name("controller_name", "vcml_default_controller"),
    network_name("network_name", "vcml_default_network"),
    can_tx("can_tx"),
    can_rx("can_rx") {
    SC_HAS_PROCESS(can_eth);
    SC_THREAD(can_transmit);
}

void service_can::start_of_simulation() {
    if (part().mode == SILKIT_MODE_OFF)
        return;

    m_can_controller = part().silkit_part()->CreateCanController(
        controller_name, network_name);
    VCML_ERROR_ON(!m_can_controller, "no silkit can controller created");

    ICanController::FrameHandler frame_handler =
        [this](ICanController*, const CanFrameEvent& msg) {
            send_to_guest(to_vcml_can(msg.frame));
        };

    ICanController::FrameTransmitHandler frame_transmit_handler =
        [this](ICanController*, const CanFrameTransmitEvent& event) {
            if (event.status !=
                SilKit::Services::Can::CanTransmitStatus::Transmitted) {
                const string time = sc_time(event.timestamp.count(), SC_NS)
                                        .to_string();
                log_error("frame not transmitted status: %d at: %s",
                          (int)event.status, time.c_str());
            }
        };

    m_can_controller->AddFrameHandler(frame_handler);
    m_can_controller->AddFrameTransmitHandler(frame_transmit_handler);
    m_can_controller->Start();
}

service_can::~service_can() {
    if (m_can_controller)
        m_can_controller->Stop();
}

const char* service_can::version() const {
    return VCML_SILKIT_VERSION_STRING;
}

VCML_EXPORT_MODEL(vcml::silkit::service_can, name, args) {
    return new service_can(name, participant::instance());
}

} // namespace silkit
} // namespace vcml
