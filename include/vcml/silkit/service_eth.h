/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_SILKIT_SERVICE_ETH_H
#define VCML_SILKIT_SERVICE_ETH_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"
#include "vcml/core/module.h"
#include "vcml/core/model.h"

#include "vcml/properties/property.h"
#include "vcml/protocols/eth.h"

#include "participant.h"
#include "service.h"
#include "silkit.h"

namespace vcml {
namespace silkit {

class service_eth : public service, public eth_host
{
private:
    SilKit::Services::Ethernet::IEthernetController* m_eth_controller;

    mutable mutex m_mtx;
    queue<eth_frame> m_rx;
    sc_event m_ev;

    void eth_receive(const eth_frame& frame) override;
    void eth_transmit();

public:
    property<string> controller_name;
    property<string> network_name;
    eth_initiator_socket eth_tx;
    eth_target_socket eth_rx;

    virtual void start_of_simulation() override;

    service_eth(const sc_module_name& nm, participant& part);
    virtual ~service_eth();
    VCML_KIND(silkit::service_eth);

    virtual const char* version() const override;

    void send_to_host(const eth_frame& frame);
    void send_to_guest(eth_frame frame);

    template <typename T>
    void connect(T& device) {
        eth_tx.bind(device.eth_rx);
        device.eth_tx.bind(eth_rx);
    }
};

} // namespace silkit
} // namespace vcml

#endif
