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
#include "vcml/silkit/version.h"

namespace vcml {
namespace silkit {

service::service(const sc_module_name& nm, participant& part,
                 const char* type):
    module(nm), m_type(type), m_part(part), log(part.log) {
}

service::~service() {
}

const char* service::version() const {
    return VCML_SILKIT_VERSION_STRING;
}

} // namespace silkit
} // namespace vcml
