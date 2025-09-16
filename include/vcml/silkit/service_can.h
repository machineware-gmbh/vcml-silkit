/******************************************************************************
 *                                                                            *
 * Copyright (C) 2025 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_SILKIT_SERVICE_CAN_H
#define VCML_SILKIT_SERVICE_CAN_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"
#include "vcml/core/module.h"
#include "vcml/core/model.h"

#include "vcml/properties/property.h"
#include "vcml/protocols/can.h"

#include "participant.h"
#include "service.h"
#include "silkit.h"

namespace vcml {
namespace silkit {

class service_can : public service, public can_host
{
private:
    SilKit::Services::Can::ICanController* m_can_controller;

    mutable mutex m_mtx;
    queue<can_frame> m_rx;
    sc_event m_ev;

    void can_receive(can_frame& frame) override;
    void can_transmit();

public:
    property<string> controller_name;
    property<string> network_name;
    can_initiator_socket can_tx;
    can_target_socket can_rx;

    virtual void start_of_simulation() override;

    service_can(const sc_module_name& nm, participant& part);
    virtual ~service_can();
    VCML_KIND(silkit::service_can);

    virtual const char* version() const override;

    void send_to_host(const can_frame& frame);
    void send_to_guest(can_frame frame);

    template <typename T>
    void connect(T& device) {
        can_tx.bind(device.eth_rx);
        device.can_tx.bind(can_rx);
    }
};

} // namespace silkit
} // namespace vcml

#endif
