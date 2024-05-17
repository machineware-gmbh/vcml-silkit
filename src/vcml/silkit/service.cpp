/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#include "vcml/silkit/service.h"

namespace vcml {
namespace silkit {

service::service(participant& part, const sc_module_name& nm):
    module(nm), m_type("unknown"), log(part.log) {
}

service::~service() {
}

} // namespace silkit
} // namespace vcml
